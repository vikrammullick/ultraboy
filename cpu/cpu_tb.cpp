#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <set>

#include "../cpu.hpp"
#include "memory.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;
using namespace std;

cpu_state_t parse_cpu(const json &data) {
    return cpu_state_t{data["pc"],
                       data["sp"],
                       data["a"],
                       data["b"],
                       data["c"],
                       data["d"],
                       data["e"],
                       data["f"],
                       data["h"],
                       data["l"]};
}
memory_t parse_mem(const json &data) {
    memory_t mem;
    for (const auto &entry : data["ram"]) {
        mem.write(entry[0], entry[1]);
    }
    return mem;
}

bool do_single(const json &test) {
    cpu_state_t initial = parse_cpu(test["initial"]);
    cpu_state_t expected = parse_cpu(test["final"]);
    memory_t test_memory = parse_mem(test["initial"]);
    memory_t expected_memory = parse_mem(test["final"]);

    cpu_t test_cpu(test_memory);
    test_cpu.load_state(initial);
    test_cpu.tick();

    bool registers_match = expected == test_cpu.dump_state();
    bool mem_match = expected_memory == test_memory;

    if (!registers_match || !mem_match) {
        cout << test["name"] << " " << registers_match << " " << mem_match
             << endl;
        cout << test << endl;
        cout << test_cpu.dump_state() << endl;
        return false;
    }
    return true;
}

bool do_test(const string &test_path) {
    // TODO remove
    if (test_path != "sm83/v1/1f.json")
        return true;

    ifstream input_file(test_path);
    json instruction_tests;
    input_file >> instruction_tests;

    for (const auto &test : instruction_tests) {
        if (!do_single(test)) {
            return false;
        }
    }
    return true;
}

int main() {
    set<string> failed;
    string dir_path = "sm83/v1";
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
