#include "memory.hpp"

using namespace std;

memory_t::memory_t(const vector<char> &boot_bytes,
                   const vector<char> &rom_bytes,
                   ppu_t &ppu)
    : m_boot(boot_bytes), m_rom(rom_bytes), m_ppu(ppu) {}

bool supported(uint16_t addr) {
    if (addr <= 0x7FFF) {
        // rom
        return true;
    }

    if (addr >= 0xC000 && addr <= 0xDFFF) {
        // wram
        return true;
    }

    if (addr >= 0xFF80 && addr <= 0xFFFE) {
        // TODO: hram move into cpu
        return true;
    }

    if (addr >= 0xFF10 && addr <= 0xFF26) {
        // TODO: audio
        return true;
    }
    if (addr >= 0xFF30 && addr <= 0xFF3F) {
        // TODO: wave pattern
        return true;
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

bool ppu_addr(uint16_t addr) {
    if (addr == constants::LCDC || addr == constants::SCY ||
        addr == constants::SCX || addr == constants::LY ||
        addr == constants::BGP) {
        return true;
    }

    if (addr >= constants::TILE_DATA_START &&
        addr <= constants::TILE_DATA_END) {
        return true;
    }

    if (addr >= constants::TILE_MAP_START && addr <= constants::TILE_MAP_END) {
        return true;
    }

    return false;
}

void memory_t::write(uint16_t addr, uint8_t val) {
    if (ppu_addr(addr)) {
        m_ppu.write(addr, val);
        m_ppu.redraw(addr, val);
        return;
    }

    if (!supported(addr)) {
        cout << "addr " << hex << addr << endl;
        assert(false);
    }

    m_data[addr] = val;
}

uint8_t memory_t::read(uint16_t addr) {
    if (ppu_addr(addr)) {
        return m_ppu.read(addr);
    }

    if (!supported(addr)) {
        cout << "addr " << hex << addr << endl;
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
