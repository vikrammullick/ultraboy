#include "memory_bus.hpp"

#include <iostream>

using namespace std;

constexpr uint32_t MEMORY_MASK = 0x07FFFFFF;
constexpr uint32_t HALFWORD_MASK = 0xFFFFFFFE;

constexpr uint32_t VIP_START = 0x00000000;
constexpr uint32_t VIP_END = 0x00FFFFFF;
constexpr uint32_t VSU_START = 0x01000000;
constexpr uint32_t VSU_END = 0x01FFFFFF;
constexpr uint32_t WRAM_START = 0x05000000;
constexpr uint32_t WRAM_END = 0x05FFFFFF;
constexpr uint32_t GAME_PAK_RAM_START = 0x06000000;
constexpr uint32_t GAME_PAK_RAM_END = 0x06FFFFFF;
constexpr uint32_t GAME_PAK_ROM_START = 0x07000000;
constexpr uint32_t GAME_PAK_ROM_END = 0x07FFFFFF;

memory_bus_t::memory_bus_t(const std::vector<char> &rom_bytes)
    : m_game_pak(rom_bytes) {}

void memory_bus_t::write_b(uint32_t addr, uint8_t val) {
    addr &= MEMORY_MASK;

    if (addr == misc_hardware_registers::CDTR_ADDR) {
        m_link.write_misc_b(addr, val);
        return;
    }
    if (addr == misc_hardware_registers::TLR_ADDR ||
        addr == misc_hardware_registers::THR_ADDR ||
        addr == misc_hardware_registers::TCR_ADDR) {
        m_timer.write_misc_b(addr, val);
        return;
    }
    if (addr == misc_hardware_registers::WCR_ADDR) {
        m_game_pak.write_misc_b(addr, val);
        return;
    }

    if (addr >= VSU_START && addr <= VSU_END) {
        m_vsu.write_b(addr - VSU_START, val);
        return;
    }

    if (addr >= WRAM_START && addr <= WRAM_END) {
        m_wram.write_b(addr - WRAM_START, val);
        return;
    }
    if (addr >= GAME_PAK_RAM_START && addr <= GAME_PAK_RAM_END) {
        m_game_pak.ram_write_b(addr - GAME_PAK_RAM_START, val);
        return;
    }

    cout << std::hex << addr << ": " << static_cast<uint16_t>(val) << endl;
    assert(false);
}

uint8_t memory_bus_t::read_b(uint32_t addr) {
    addr &= MEMORY_MASK;

    if (addr == misc_hardware_registers::CDTR_ADDR) {
        return m_link.read_misc_b(addr);
    }
    if (addr == misc_hardware_registers::TLR_ADDR ||
        addr == misc_hardware_registers::THR_ADDR ||
        addr == misc_hardware_registers::TCR_ADDR) {
        return m_timer.read_misc_b(addr);
    }
    if (addr == misc_hardware_registers::WCR_ADDR) {
        return m_game_pak.read_misc_b(addr);
    }

    if (addr >= WRAM_START && addr <= WRAM_END) {
        return m_wram.read_b(addr - WRAM_START);
    }
    if (addr >= GAME_PAK_RAM_START && addr <= GAME_PAK_RAM_END) {
        return m_game_pak.ram_read_b(addr - GAME_PAK_RAM_START);
    }
    if (addr >= GAME_PAK_ROM_START && addr <= GAME_PAK_ROM_END) {
        return m_game_pak.rom_read_b(addr - GAME_PAK_ROM_START);
    }

    cout << std::hex << addr << endl;
    assert(false);
}

void memory_bus_t::write_h(uint32_t addr, uint16_t val) {
    addr &= MEMORY_MASK;
    addr &= HALFWORD_MASK;

    if (addr >= VIP_START && addr <= VIP_END) {
        m_vip.write_h(addr - VIP_START, val);
        return;
    }
    if (addr >= WRAM_START && addr <= WRAM_END) {
        m_wram.write_h(addr - WRAM_START, val);
        return;
    }
    if (addr >= GAME_PAK_RAM_START && addr <= GAME_PAK_RAM_END) {
        m_game_pak.ram_write_h(addr - GAME_PAK_RAM_START, val);
        return;
    }

    cout << std::hex << addr << ": " << val << endl;
    assert(false);
}

uint16_t memory_bus_t::read_h(uint32_t addr) {
    addr &= MEMORY_MASK;
    addr &= HALFWORD_MASK;

    if (addr >= VIP_START && addr <= VIP_END) {
        return m_vip.read_h(addr - VIP_START);
    }
    if (addr >= WRAM_START && addr <= WRAM_END) {
        return m_wram.read_h(addr - WRAM_START);
    }
    if (addr >= GAME_PAK_RAM_START && addr <= GAME_PAK_RAM_END) {
        return m_game_pak.ram_read_h(addr - GAME_PAK_RAM_START);
    }
    if (addr >= GAME_PAK_ROM_START && addr <= GAME_PAK_ROM_END) {
        return m_game_pak.rom_read_h(addr - GAME_PAK_ROM_START);
    }

    cout << std::hex << addr << endl;
    assert(false);
}
