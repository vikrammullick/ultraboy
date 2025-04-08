#include "vip.hpp"

#include <iostream>

using namespace std;

void world_attributes_t::write_h(uint32_t addr, uint16_t val) {
    char *byte_ptr = m_data.data() + addr;
    uint16_t *half_word_ptr = reinterpret_cast<uint16_t *>(byte_ptr);
    *half_word_ptr = val;
}

constexpr size_t WORLD_ATTRIBUTES_START = 0x0003D800;
constexpr size_t WORLD_ATTRIBUTES_END = 0x0003DBFF;

void vip_t::write_h(uint32_t addr, uint16_t val) {
    if (addr >= WORLD_ATTRIBUTES_START && addr <= WORLD_ATTRIBUTES_END) {
        m_world_attributes.write_h(addr - WORLD_ATTRIBUTES_START, val);
        return;
    }

    cout << std::hex << addr << endl;
    assert(false);
}
