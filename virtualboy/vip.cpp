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

constexpr size_t WORLD_ATTRIBUTES_SIZE = 0x400;
world_attributes_t::world_attributes_t()
    : memory_block_t(WORLD_ATTRIBUTES_SIZE) {}

constexpr size_t FRAME_BUFFER_LEFT_0_START = 0x00000000;
constexpr size_t FRAME_BUFFER_LEFT_0_END = 0x00005FFF;

constexpr size_t WORLD_ATTRIBUTES_START = 0x0003D800;
constexpr size_t WORLD_ATTRIBUTES_END = 0x0003DBFF;

void vip_t::write_h(uint32_t addr, uint16_t val) {
    if (addr >= FRAME_BUFFER_LEFT_0_START && addr <= FRAME_BUFFER_LEFT_0_END) {
        m_frame_buffer_left_0.write_h(addr - FRAME_BUFFER_LEFT_0_START, val);
        return;
    }

    if (addr >= WORLD_ATTRIBUTES_START && addr <= WORLD_ATTRIBUTES_END) {
        m_world_attributes.write_h(addr - WORLD_ATTRIBUTES_START, val);
        return;
    }

    cout << std::hex << addr << endl;
    assert(false);
}
