#include "memory.hpp"
#include "sdl_helpers.hpp"

using namespace std;

memory_t::memory_t(const vector<char> &boot_bytes,
                   const vector<char> &rom_bytes)
    : m_boot(boot_bytes), m_rom(rom_bytes) {}

void memory_t::write(uint16_t addr, uint8_t val) {
    m_data[addr] = val;

    if (addr < 0x8000 || addr >= 0x9800) {
        return;
    }

    if (addr % 2 == 0) {
        sdl_update_tile_row(addr, val, m_data[addr + 1]);
    } else {
        sdl_update_tile_row(addr - 1, m_data[addr - 1], val);
    }
}

uint8_t memory_t::read(uint16_t addr) {
    if (addr < 0x0100) {
        return m_boot[addr];
    } else if (addr < 8000) {
        return m_rom[addr];
    } else {
        return m_data[addr];
    }
}
