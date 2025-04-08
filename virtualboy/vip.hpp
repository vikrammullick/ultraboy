#ifndef VIP_H
#define VIP_H

#include <array>
#include <vector>

class frame_buffer_t {
    std::vector<char> m_data;

  public:
    frame_buffer_t();

    void write_h(uint32_t addr, uint16_t val);
};

class world_attributes_t {
    // 32 worlds
    std::array<char, 1024> m_data;

  public:
    void write_h(uint32_t addr, uint16_t val);
};

class vip_t {
    frame_buffer_t m_frame_buffer_left_0;
    world_attributes_t m_world_attributes;

  public:
    void write_h(uint32_t addr, uint16_t val);
};

#endif // VIP_H
