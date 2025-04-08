#ifndef VIP_H
#define VIP_H

#include <vector>

class memory_block_t {
    std::vector<char> m_data;

  public:
    memory_block_t(size_t size);

    void write_h(uint32_t addr, uint16_t val);
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

  public:
    void write_h(uint32_t addr, uint16_t val);
};

#endif // VIP_H
