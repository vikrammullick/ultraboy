#include "memory_bus.hpp"

#include <iostream>

using namespace std;

constexpr uint32_t MEMORY_MASK = 0x07FFFFFF;
constexpr uint32_t HALFWORD_MASK = 0xFFFFFFFE;

constexpr uint32_t WRAM_START = 0x05000000;
constexpr uint32_t WRAM_END = 0x05FFFFFF;
constexpr uint32_t GAME_PAK_ROM_START = 0x07000000;
constexpr uint32_t GAME_PAK_ROM_END = 0x07FFFFFF;

memory_bus_t::memory_bus_t(const std::vector<char> &rom_bytes)
    : m_game_pak(rom_bytes) {}

void memory_bus_t::write_b(uint32_t addr, uint8_t val) {
    addr &= MEMORY_MASK;
    assert(false);
}

uint8_t memory_bus_t::read_b(uint32_t addr) {
    addr &= MEMORY_MASK;
    assert(false);
}

void memory_bus_t::write_h(uint32_t addr, uint16_t val) {
    addr &= MEMORY_MASK;
    addr &= HALFWORD_MASK;

    if (addr >= WRAM_START && addr <= WRAM_END) {
        m_wram.write_h(addr - WRAM_START, val);
        return;
    }

    cout << std::hex << addr << endl;
    assert(false);
}

uint16_t memory_bus_t::read_h(uint32_t addr) {
    addr &= MEMORY_MASK;
    addr &= HALFWORD_MASK;

    if (addr >= GAME_PAK_ROM_START && addr <= GAME_PAK_ROM_END) {
        return m_game_pak.rom_read_h(addr - GAME_PAK_ROM_START);
    }

    cout << std::hex << addr << endl;
    assert(false);
}
