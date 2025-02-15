#include "cpu.hpp"

#include <iostream>
#include <sstream>

using namespace std;

cpu_t::cpu_t(memory_t &memory) : m_memory(memory) {}

void cpu_t::tick() {
    constexpr uint16_t NO_OPCODE = 0x0000;

    stringstream oss;
    oss << "unknown opcode: 0x" << std::hex << NO_OPCODE;
    throw std::runtime_error(oss.str());
}
