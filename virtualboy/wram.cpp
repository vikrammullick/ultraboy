#include "wram.hpp"

#include <iostream>

using namespace std;

constexpr size_t WRAM_SIZE = 0x01000000;

wram_t::wram_t() : m_bytes(WRAM_SIZE) {}

void wram_t::write_h(uint32_t addr, uint16_t val) {
    char *byte_ptr = m_bytes.data() + addr;
    uint16_t *half_word_ptr = reinterpret_cast<uint16_t *>(byte_ptr);
    *half_word_ptr = val;
}
