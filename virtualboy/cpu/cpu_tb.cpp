#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <set>

#include "cpu.hpp"
#include "memory_bus.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;
using namespace std;

std::ostream &operator<<(std::ostream &os, const memory_bus_t &bus) {
    os << "Memory Contents (" << bus.m_mem.size() << " bytes):\n";

    if (bus.m_mem.empty()) {
        os << "  <empty>\n";
        return os;
    }

    // Track where we are in memory to align 16-byte rows
    uint32_t current_line = 0;
    for (auto it = bus.m_mem.begin(); it != bus.m_mem.end();) {
        uint32_t addr = it->first;
        current_line = addr & ~0xF; // align to 16-byte boundary

        // Start of a line
        os << "0x" << std::hex << std::setw(8) << std::setfill('0')
           << current_line << ": ";

        // Print 16 bytes for this line
        for (int i = 0; i < 16; ++i) {
            uint32_t byte_addr = current_line + i;
            if (bus.m_mem.contains(byte_addr)) {
                os << std::setw(2) << std::setfill('0')
                   << static_cast<int>(bus.m_mem.at(byte_addr)) << " ";
            } else {
                os << "   "; // blank space for missing memory
            }
        }

        os << "\n";

        // Advance iterator to the next 16-byte line
        do {
            ++it;
        } while (it != bus.m_mem.end() && (it->first & ~0xF) == current_line);
    }

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
            os << "\n";
        else
            os << "  ";
    }

    os << "PSW flags:\n"
       << "  Z (Zero)                   = " << state.psw_zero << "\n"
       << "  S (Sign)                   = " << state.psw_sign << "\n"
       << "  O (Overflow)               = " << state.psw_overflow << "\n"
       << "  C (Carry)                  = " << state.psw_carry << "\n"
       << "  FP Precision Degradation   = "
       << state.psw_fp_precision_degredation << "\n"
       << "  FP Underflow               = " << state.psw_fp_underflow << "\n"
       << "  FP Overflow                = " << state.psw_fp_overflow << "\n"
       << "  FP Zero Division           = " << state.psw_fp_zero_division
       << "\n"
       << "  FP Invalid Operation       = " << state.psw_fp_invalid_operation
       << "\n"
       << "  FP Reserved Operand        = " << state.psw_fp_reserved_operand
       << "\n"
       << "  Interrupt Disable          = " << state.psw_interrupt_disable
       << "\n"
       << "  Address Trap Enable        = " << state.psw_address_trap_enable
       << "\n"
       << "  Exception Pending          = " << state.psw_exception_pending
       << "\n"
       << "  NMI Pending                = " << state.psw_nmi_pending << "\n"
       << "  Interrupt Mask Level       = " << std::dec
       << static_cast<int>(state.psw_interrupt_mask_level) << "\n";

    return os;
}

cpu_state_t parse_cpu(const json &data) {
    return cpu_state_t{data["reg_pc"],
                       data["reg_gpr"],
                       data["psw_zero"],
                       data["psw_sign"],
                       data["psw_overflow"],
                       data["psw_carry"],
                       data["psw_fp_precision_degredation"],
                       data["psw_fp_underflow"],
                       data["psw_fp_overflow"],
                       data["psw_fp_zero_division"],
                       data["psw_fp_invalid_operation"],
                       data["psw_fp_reserved_operand"],
                       data["psw_interrupt_disable"],
                       data["psw_address_trap_enable"],
                       data["psw_exception_pending"],
                       data["psw_nmi_pending"],
                       data["psw_interrupt_mask_level"]};
}

memory_bus_t parse_mem(const json &data1, const json &data2) {
    memory_bus_t mem;
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

    memory_bus_t test_memory_bus = parse_mem(test["reads"], test["reads"]);
    memory_bus_t expected_memory_bus = parse_mem(test["reads"], test["writes"]);

    cpu_t test_cpu(test_memory_bus);
    test_cpu.load_state(initial);
    test_cpu.tick();

    auto actual_state = test_cpu.dump_state();

    bool registers_match = expected == actual_state;
    bool mem_match = expected_memory_bus == test_memory_bus;

    if (!registers_match || !mem_match) {
        cout << "got: \n" << test_memory_bus << "\n" << actual_state << endl;
        cout << "expected: \n"
             << expected_memory_bus << "\n"
             << expected << endl;
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
