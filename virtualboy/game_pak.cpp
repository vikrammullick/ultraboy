#include "game_pak.hpp"

#include <iostream>

using namespace std;

game_pak_t::game_pak_t(const std::vector<char> &rom_bytes)
    : m_rom_bytes(rom_bytes) {
    assert(0x01000000 % rom_bytes.size() == 0);
}

uint16_t game_pak_t::rom_read_h(uint32_t addr) {
    const char *byte_ptr = m_rom_bytes.data() + (addr % m_rom_bytes.size());
    const uint16_t *half_word_ptr =
        reinterpret_cast<const uint16_t *>(byte_ptr);
    return *half_word_ptr;
}
