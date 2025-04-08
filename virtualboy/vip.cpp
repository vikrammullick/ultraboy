#include "vip.hpp"

#include <iostream>

using namespace std;

memory_block_t::memory_block_t(size_t size) : m_data(size) {}

void memory_block_t::write_h(uint32_t addr, uint16_t val) {
    char *byte_ptr = m_data.data() + addr;
    uint16_t *half_word_ptr = reinterpret_cast<uint16_t *>(byte_ptr);
    *half_word_ptr = val;
}

constexpr size_t FRAME_BUFFER_SIZE = 0x6000;
frame_buffer_t::frame_buffer_t() : memory_block_t(FRAME_BUFFER_SIZE) {}

constexpr size_t CHARACTER_TABLE_SIZE = 0x2000;
character_table_t::character_table_t() : memory_block_t(CHARACTER_TABLE_SIZE) {}

constexpr size_t WORLD_ATTRIBUTES_SIZE = 0x400;
world_attributes_t::world_attributes_t()
    : memory_block_t(WORLD_ATTRIBUTES_SIZE) {}

constexpr size_t FRAME_BUFFER_LEFT_0_START = 0x00000000;
constexpr size_t CHARACTER_TABLE_0_START = 0x00006000;
constexpr size_t WORLD_ATTRIBUTES_START = 0x0003D800;

#define ADD_WRITE_H(buf, base)                                                 \
    if (addr >= base##_START && addr < (base##_START + base##_SIZE)) {         \
        buf.write_h(addr - base##_START, val);                                 \
        return;                                                                \
    }

#define ADD_WRITE_H_SUFFIX(buf, base, suffix)                                  \
    if (addr >= base##_##suffix##_START &&                                     \
        addr < (base##_##suffix##_START + base##_SIZE)) {                      \
        buf.write_h(addr - base##_##suffix##_START, val);                      \
        return;                                                                \
    }

void vip_t::write_h(uint32_t addr, uint16_t val) {
    ADD_WRITE_H_SUFFIX(m_frame_buffer_left_0, FRAME_BUFFER, LEFT_0);
    ADD_WRITE_H_SUFFIX(m_character_table_0, CHARACTER_TABLE, 0);
    ADD_WRITE_H(m_world_attributes, WORLD_ATTRIBUTES);

    cout << std::hex << addr << endl;
    assert(false);
}
