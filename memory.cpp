#include "memory.hpp"

using namespace std;

memory_t::memory_t(const vector<char> &rom_bytes) {
    // load rom
    std::copy(rom_bytes.begin(), rom_bytes.end(), m_data.begin());
}

void memory_t::write(uint16_t addr, uint8_t val) { m_data[addr] = val; }

uint8_t memory_t::read(uint16_t addr) { return m_data[addr]; }
