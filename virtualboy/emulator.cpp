#include "emulator.hpp"

#include <iostream>

using namespace std;

emulator_t::emulator_t(const vector<char> &rom_bytes)
    : m_memory_bus(m_vip, rom_bytes), m_cpu(m_memory_bus) {}

void emulator_t::run() {
    while (1) {
        // TODO: count and align ticks correctly
        m_cpu.tick();
        m_vip.tick();
    }
}
