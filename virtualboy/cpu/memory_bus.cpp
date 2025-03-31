#include "memory_bus.hpp"
#include <iostream>

using namespace std;

memory_bus_t::memory_bus_t() {}

void memory_bus_t::write_b(uint32_t addr, uint8_t val) { m_mem[addr] = val; }

uint8_t memory_bus_t::read_b(uint32_t addr) { return m_mem.at(addr); }

void memory_bus_t::write_h(uint32_t addr, uint16_t val) {
    addr = addr & 0xFFFFFFFE;
    m_mem[addr] = val & 0xFF;
    m_mem[addr + 1] = (val >> 8) & 0xFF;
}

uint16_t memory_bus_t::read_h(uint32_t addr) {
    addr = addr & 0xFFFFFFFE;
    uint8_t lo = m_mem.at(addr);
    uint8_t hi = m_mem.at(addr + 1);
    return (hi << 8) + lo;
}
