#include "memory_bus.hpp"

using namespace std;

memory_bus_t::memory_bus_t(const std::vector<char> &rom_bytes) {}

void memory_bus_t::write_b(uint32_t addr, uint8_t val) { assert(false); }

uint8_t memory_bus_t::read_b(uint32_t addr) { assert(false); }

void memory_bus_t::write_h(uint32_t addr, uint16_t val) { assert(false); }

uint16_t memory_bus_t::read_h(uint32_t addr) { assert(false); }
