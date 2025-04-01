#include "emulator.hpp"

using namespace std;

emulator_t::emulator_t(const vector<char> &rom_bytes)
    : m_memory_bus(rom_bytes), m_cpu(m_memory_bus) {}

void emulator_t::run() {}
