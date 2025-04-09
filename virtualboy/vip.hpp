#ifndef VIP_H
#define VIP_H

#include <vector>

class memory_block_t {
    std::vector<char> m_data;

  public:
    memory_block_t(size_t size);

    void write_h(uint32_t addr, uint16_t val);
    uint16_t read_h(uint32_t addr);
};

class frame_buffer_t : public memory_block_t {
  public:
    frame_buffer_t();
};

class character_table_t : public memory_block_t {
  public:
    character_table_t();
};

class world_attributes_t : public memory_block_t {
  public:
    world_attributes_t();
};

struct interrupt_register_t {
    bool m_TIMEERR = false;
    bool m_XPEND = false;
    bool m_SBHIT = false;
    bool m_FRAMESTART = false;
    bool m_GAMESTART = false;
    bool m_RFBEND = false;
    bool m_LFBEND = false;
    bool m_SCANERR = false;

    uint16_t read() {
        return (m_TIMEERR << 15) | (m_XPEND << 14) | (m_SBHIT << 13) |
               (m_FRAMESTART << 4) | (m_GAMESTART << 3) | (m_RFBEND << 2) |
               (m_LFBEND << 1) | (m_SCANERR << 0);
    }
};

class vip_t {
    frame_buffer_t m_frame_buffer_left_0;
    character_table_t m_character_table_0;
    frame_buffer_t m_frame_buffer_left_1;
    character_table_t m_character_table_1;
    frame_buffer_t m_frame_buffer_right_0;
    character_table_t m_character_table_2;
    frame_buffer_t m_frame_buffer_right_1;
    character_table_t m_character_table_3;
    world_attributes_t m_world_attributes;

    interrupt_register_t m_INTPND;
    interrupt_register_t m_INTENB;

  public:
    void write_h(uint32_t addr, uint16_t val);
    uint16_t read_h(uint32_t addr);
};

#endif // VIP_H
