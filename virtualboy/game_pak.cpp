#include "game_pak.hpp"
#include "memory_bus.hpp"

#include <iostream>

using namespace std;

// TODO: use more accurate sram size per game?
constexpr size_t SRAM_SIZE = 1 << 24;

game_pak_t::game_pak_t(const std::vector<char> &rom_bytes)
    : m_ram_bytes(SRAM_SIZE), m_rom_bytes(rom_bytes) {
    assert(0x01000000 % rom_bytes.size() == 0);
}

#define ADD_MISC_REGISTER_OP(regaddr, reg, op)                                 \
    if (addr == misc_hardware_registers::regaddr##_ADDR) {                     \
        m_##reg.op(val);                                                       \
        return;                                                                \
    }

void game_pak_t::write_misc_b(uint32_t addr, uint8_t val) {
    ADD_MISC_REGISTER_OP(WCR, WCR, write);

    cout << std::hex << addr << ": " << static_cast<uint16_t>(val) << endl;
    assert(false);
}

#define ADD_MISC_REGISTER_READ(reg)                                            \
    if (addr == misc_hardware_registers::reg##_ADDR) {                         \
        return m_##reg.read();                                                 \
    }

uint8_t game_pak_t::read_misc_b(uint32_t addr) {
    ADD_MISC_REGISTER_READ(WCR);

    cout << std::hex << addr << endl;
    assert(false);
}

// TODO: de-dup below logic
void game_pak_t::ram_write_b(uint32_t addr, uint8_t val) {
    m_ram_bytes[addr] = val;
}

uint8_t game_pak_t::ram_read_b(uint32_t addr) { return m_ram_bytes[addr]; }

void game_pak_t::ram_write_h(uint32_t addr, uint16_t val) {
    // TODO: research cartridge ram sizes and mask address appropriately
    char *byte_ptr = m_ram_bytes.data() + addr;
    uint16_t *half_word_ptr = reinterpret_cast<uint16_t *>(byte_ptr);
    *half_word_ptr = val;
}

uint16_t game_pak_t::ram_read_h(uint32_t addr) {
    const char *byte_ptr = m_ram_bytes.data() + addr;
    const uint16_t *half_word_ptr =
        reinterpret_cast<const uint16_t *>(byte_ptr);
    return *half_word_ptr;
}

uint8_t game_pak_t::rom_read_b(uint32_t addr) {
    return m_rom_bytes[addr % m_rom_bytes.size()];
}

uint16_t game_pak_t::rom_read_h(uint32_t addr) {
    const char *byte_ptr = m_rom_bytes.data() + (addr % m_rom_bytes.size());
    const uint16_t *half_word_ptr =
        reinterpret_cast<const uint16_t *>(byte_ptr);
    return *half_word_ptr;
}
