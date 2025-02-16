#include "memory.hpp"
#include "sdl_helpers.hpp"

using namespace std;

memory_t::memory_t(const vector<char> &boot_bytes,
                   const vector<char> &rom_bytes)
    : m_boot(boot_bytes), m_rom(rom_bytes) {}

bool supported(uint16_t addr) {
    if (addr <= 0xDFFF) {
        // rom, vram, wram
        return true;
    }

    if (addr == 0xFF47) {
        // color palette
        return true;
    }

    if (addr >= 0xFF80 && addr <= 0xFFFE) {
        // hram
        return true;
    }

    if (addr >= 0xFF10 && addr <= 0xFF26) {
        // TODO: audio
        return false;
    }
    if (addr >= 0xFF30 && addr <= 0xFF3F) {
        // TODO: wave pattern
        return false;
    }

    if (addr >= 0xE000 && addr <= 0xFDFF) {
        // nintendo prohibited
        assert(false);
    }

    if (addr >= 0xFEA0 && addr <= 0xFEFF) {
        // nintendo prohibited
        assert(false);
    }

    cout << "addr " << hex << addr << endl;
    assert(false);
    return false;
}

void memory_t::write(uint16_t addr, uint8_t val) {
    if (!supported(addr)) {
        return;
    }

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
    if (!supported(addr)) {
        cout << "write addr " << hex << addr << endl;
        assert(false);
    }

    if (addr < 0x0100) {
        return m_boot[addr];
    } else if (addr < 8000) {
        return m_rom[addr];
    } else {
        return m_data[addr];
    }
}
