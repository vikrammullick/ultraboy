#include "vip.hpp"

#include <iostream>

using namespace std;

constexpr uint32_t VIP_MEMORY_MASK = 0x0007FFFF;

memory_block_t::memory_block_t(size_t size) : m_data(size) {}

void memory_block_t::write_h(uint32_t addr, uint16_t val) {
    char *byte_ptr = m_data.data() + addr;
    uint16_t *half_word_ptr = reinterpret_cast<uint16_t *>(byte_ptr);
    *half_word_ptr = val;
}

uint16_t memory_block_t::read_h(uint32_t addr) {
    const char *byte_ptr = m_data.data() + addr;
    const uint16_t *half_word_ptr =
        reinterpret_cast<const uint16_t *>(byte_ptr);
    return *half_word_ptr;
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
constexpr size_t FRAME_BUFFER_LEFT_1_START = 0x00008000;
constexpr size_t CHARACTER_TABLE_1_START = 0x0000E000;
constexpr size_t FRAME_BUFFER_RIGHT_0_START = 0x00010000;
constexpr size_t CHARACTER_TABLE_2_START = 0x00016000;
constexpr size_t FRAME_BUFFER_RIGHT_1_START = 0x00018000;
constexpr size_t CHARACTER_TABLE_3_START = 0x0001E000;

constexpr size_t WORLD_ATTRIBUTES_START = 0x0003D800;

constexpr size_t CHARACTER_TABLE_MIRROR_0_START = 0x00078000;
constexpr size_t CHARACTER_TABLE_MIRROR_1_START = 0x0007A000;
constexpr size_t CHARACTER_TABLE_MIRROR_2_START = 0x0007C000;
constexpr size_t CHARACTER_TABLE_MIRROR_3_START = 0x0007E000;

#define ADD_RW_BOUNDS(addr, base, start)                                       \
    (addr >= start && addr < (start + base##_SIZE))

#define ADD_WRITE_H(buf, base)                                                 \
    if (ADD_RW_BOUNDS(addr, base, base##_START)) {                             \
        buf.write_h(addr - base##_START, val);                                 \
        return;                                                                \
    }

#define ADD_WRITE_H_SUFFIX(buf, base, suffix)                                  \
    if (ADD_RW_BOUNDS(addr, base, base##_##suffix##_START)) {                  \
        buf.write_h(addr - base##_##suffix##_START, val);                      \
        return;                                                                \
    }

void vip_t::write_h(uint32_t addr, uint16_t val) {
    addr &= VIP_MEMORY_MASK;

    ADD_WRITE_H_SUFFIX(m_frame_buffer_left_0, FRAME_BUFFER, LEFT_0);
    ADD_WRITE_H_SUFFIX(m_character_table_0, CHARACTER_TABLE, 0);
    ADD_WRITE_H_SUFFIX(m_frame_buffer_left_1, FRAME_BUFFER, LEFT_1);
    ADD_WRITE_H_SUFFIX(m_character_table_1, CHARACTER_TABLE, 1);
    ADD_WRITE_H_SUFFIX(m_frame_buffer_right_0, FRAME_BUFFER, RIGHT_0);
    ADD_WRITE_H_SUFFIX(m_character_table_2, CHARACTER_TABLE, 2);
    ADD_WRITE_H_SUFFIX(m_frame_buffer_right_1, FRAME_BUFFER, RIGHT_1);
    ADD_WRITE_H_SUFFIX(m_character_table_3, CHARACTER_TABLE, 3);

    ADD_WRITE_H(m_world_attributes, WORLD_ATTRIBUTES);

    ADD_WRITE_H_SUFFIX(m_character_table_0, CHARACTER_TABLE, MIRROR_0);
    ADD_WRITE_H_SUFFIX(m_character_table_1, CHARACTER_TABLE, MIRROR_1);
    ADD_WRITE_H_SUFFIX(m_character_table_2, CHARACTER_TABLE, MIRROR_2);
    ADD_WRITE_H_SUFFIX(m_character_table_3, CHARACTER_TABLE, MIRROR_3);

    cout << std::hex << addr << endl;
    assert(false);
}

#define ADD_READ_H(buf, base)                                                  \
    if (ADD_RW_BOUNDS(addr, base, base##_START)) {                             \
        return buf.read_h(addr - base##_START);                                \
    }

#define ADD_READ_H_SUFFIX(buf, base, suffix)                                   \
    if (ADD_RW_BOUNDS(addr, base, base##_##suffix##_START)) {                  \
        return buf.read_h(addr - base##_##suffix##_START);                     \
    }

uint16_t vip_t::read_h(uint32_t addr) {
    addr &= VIP_MEMORY_MASK;

    ADD_READ_H_SUFFIX(m_frame_buffer_left_0, FRAME_BUFFER, LEFT_0);
    ADD_READ_H_SUFFIX(m_character_table_0, CHARACTER_TABLE, 0);
    ADD_READ_H_SUFFIX(m_frame_buffer_left_1, FRAME_BUFFER, LEFT_1);
    ADD_READ_H_SUFFIX(m_character_table_1, CHARACTER_TABLE, 1);
    ADD_READ_H_SUFFIX(m_frame_buffer_right_0, FRAME_BUFFER, RIGHT_0);
    ADD_READ_H_SUFFIX(m_character_table_2, CHARACTER_TABLE, 2);
    ADD_READ_H_SUFFIX(m_frame_buffer_right_1, FRAME_BUFFER, RIGHT_1);
    ADD_READ_H_SUFFIX(m_character_table_3, CHARACTER_TABLE, 3);

    ADD_READ_H(m_world_attributes, WORLD_ATTRIBUTES);

    ADD_READ_H_SUFFIX(m_character_table_0, CHARACTER_TABLE, MIRROR_0);
    ADD_READ_H_SUFFIX(m_character_table_1, CHARACTER_TABLE, MIRROR_1);
    ADD_READ_H_SUFFIX(m_character_table_2, CHARACTER_TABLE, MIRROR_2);
    ADD_READ_H_SUFFIX(m_character_table_3, CHARACTER_TABLE, MIRROR_3);

    cout << std::hex << addr << endl;
    assert(false);
}
