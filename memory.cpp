#include "memory.hpp"

using namespace std;

memory_t::memory_t(const vector<char> &boot_bytes,
                   const vector<char> &rom_bytes,
                   ppu_t &ppu)
    : m_boot(boot_bytes), m_rom(rom_bytes), m_ppu(ppu) {
    m_data = std::vector<uint8_t>(constants::MEM_SIZE, 0);

    cout << int(rom_bytes[0x147]) << endl;
}

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

    if (addr == 0xFF01 || addr == 0xFF02) {
        // TODO: serial
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

    if (addr == constants::KEYBOARD) {
        // TODO: keyboard
        return true;
    }

    // TODO: properly address unmapped hardware registers
    if ((addr >= 0xFF51 && addr <= 0xFF7F) || addr == 0xFF4F) {
        return true;
    }

    if (addr == constants::OAM_TRANSFER) {
        // TODO: should actually invoke the oam transfer
        return true;
    }

    // I guess this is allowed?
    if (addr >= 0xFEA0 && addr <= 0xFEFF) {
        // nintendo prohibited
        return true;
    }

    cout << "addr " << hex << addr << endl;
    assert(false);
    return false;
}

bool ppu_addr(uint16_t addr) {
    if (addr == constants::LCDC || addr == constants::STAT ||
        addr == constants::SCY || addr == constants::SCX ||
        addr == constants::LY || addr == constants::BGP ||
        addr == constants::OBP0 || addr == constants::OBP1 ||
        addr == constants::WX || addr == constants::WY) {
        return true;
    }

    if (addr >= constants::TILE_DATA_START &&
        addr <= constants::TILE_DATA_END) {
        return true;
    }

    if (addr >= constants::TILE_MAP_START && addr <= constants::TILE_MAP_END) {
        return true;
    }

    if (addr >= constants::OAM_START && addr <= constants::OAM_END) {
        return true;
    }

    return false;
}

void memory_t::write(uint16_t addr, uint8_t val) {
    if (addr == constants::BOOT_ROM_DISABLE && val) {
        m_boot_rom_disable = true;
        return;
    }

    if (addr == constants::KEYBOARD) {
        m_keyboard = val;
    }

    if (addr >= 0xFEA0 && addr <= 0xFEFF) {
        return;
    }

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
    if (addr >= 0xFEA0 && addr <= 0xFEFF) {
        return 0x00;
    }

    if (addr == constants::KEYBOARD) {
        // TODO: actually return proper keyboard state
        return m_keyboard | 0xF;
    }

    if (ppu_addr(addr)) {
        return m_ppu.read(addr);
    }

    if (!supported(addr)) {
        cout << "addr " << hex << addr << endl;
        assert(false);
    }

    if (addr < 0x0100 && !m_boot_rom_disable) {
        return m_boot[addr];
    } else if (addr < 0x8000) {
        return m_rom[addr];
    } else {
        return m_data[addr];
    }
}
