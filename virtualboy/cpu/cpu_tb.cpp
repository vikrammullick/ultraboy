#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <set>

#include "cpu.hpp"
#include "memory.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;
using namespace std;

std::ostream &operator<<(std::ostream &os, const memory_t &obj) {
    os << std::hex;
    os << "{";
    bool first = true;
    for (const auto &[key, value] : obj.m_mem) {
        if (!first)
            os << ", ";
        first = false;
        os << key << ": "
           << static_cast<int>(value); // cast u8 to int for readable output
    }
    os << "}";
    return os;
}

std::ostream &operator<<(std::ostream &os, const cpu_state_t &state) {
    os << "PC: 0x" << std::hex << std::setw(8) << std::setfill('0') << state.pc
       << "\n";

    os << "Registers:\n";
    for (size_t i = 0; i < state.regs.size(); ++i) {
        os << "  r" << std::dec << std::setw(2) << i << ": 0x" << std::hex
           << std::setw(8) << std::setfill('0') << state.regs[i];
        if ((i + 1) % 4 == 0)
            os << "\n"; // print 4 regs per line
        else
            os << "  ";
    }

    os << "PSW flags: "
       << "Z=" << state.psw_zero << " "
       << "S=" << state.psw_sign << " "
       << "O=" << state.psw_overflow << " "
       << "C=" << state.psw_carry << "\n";

    return os;
}

cpu_state_t parse_cpu(const json &data) {
    return cpu_state_t{data["reg_pc"],
                       data["reg_gpr"],
                       data["psw_zero"],
                       data["psw_sign"],
                       data["psw_overflow"],
                       data["psw_carry"]};
}

memory_t parse_mem(const json &data1, const json &data2) {
    memory_t mem;
    for (const auto &[addr, value] : data1.items()) {
        uint32_t addr_val = std::stoul(addr);
        uint8_t value_val = value;
        mem.write_b(addr_val, value_val);
    }
    for (const auto &[addr, value] : data2.items()) {
        uint32_t addr_val = std::stoul(addr);
        uint8_t value_val = value;
        mem.write_b(addr_val, value_val);
    }
    return mem;
}

bool do_single(const json &test) {
    cpu_state_t initial = parse_cpu(test["before"]);
    cpu_state_t expected = parse_cpu(test["after"]);

    memory_t test_memory = parse_mem(test["reads"], test["reads"]);
    memory_t expected_memory = parse_mem(test["reads"], test["writes"]);

    cpu_t test_cpu(test_memory);
    test_cpu.load_state(initial);
    test_cpu.tick();

    auto actual_state = test_cpu.dump_state();

    bool registers_match = expected == actual_state;
    bool mem_match = expected_memory == test_memory;

    if (!registers_match || !mem_match) {
        cout << "got: \n" << test_memory << "\n" << actual_state << endl;
        cout << "expected: \n" << expected_memory << "\n" << expected << endl;
        return false;
    }
    return true;
}

bool do_test(const string &test_path) {
    ifstream input_file(test_path);
    json instruction_tests;
    input_file >> instruction_tests;

    // cout << instruction_tests.size() << endl;
    for (const auto &test : instruction_tests) {
        if (!do_single(test)) {
            return false;
        }
    }
    return true;
}

int main() {
    set<string> failed;
    string dir_path = "json_tests";
    for (const auto &entry : fs::directory_iterator(dir_path)) {
        if (!do_test(entry.path())) {
            failed.emplace(entry.path());
        }
    }

    for (const auto &fail : failed) {
        cout << fail << endl;
    }

    return 0;
}
