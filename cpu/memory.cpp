#include "memory.hpp"
#include <iostream>

using namespace std;

memory_t::memory_t() { m_mem = std::vector<uint8_t>(1 << 16, 0); }

void memory_t::write(uint16_t addr, uint8_t val) { m_mem[addr] = val; }

uint8_t memory_t::read(uint16_t addr) { return m_mem[addr]; }
