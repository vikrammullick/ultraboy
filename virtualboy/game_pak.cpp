#include "game_pak.hpp"
#include "memory_bus.hpp"

#include <iostream>

using namespace std;

game_pak_t::game_pak_t(const std::vector<char> &rom_bytes)
    : m_rom_bytes(rom_bytes) {
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

uint8_t game_pak_t::rom_read_b(uint32_t addr) {
    return m_rom_bytes[addr % m_rom_bytes.size()];
}

uint16_t game_pak_t::rom_read_h(uint32_t addr) {
    const char *byte_ptr = m_rom_bytes.data() + (addr % m_rom_bytes.size());
    const uint16_t *half_word_ptr =
        reinterpret_cast<const uint16_t *>(byte_ptr);
    return *half_word_ptr;
}
