#include "memory.hpp"
#include <iostream>

using namespace std;

memory_t::memory_t() {}

void memory_t::write(uint32_t addr, uint8_t val) { m_mem[addr] = val; }

uint8_t memory_t::read(uint32_t addr) { return m_mem.at(addr); }
