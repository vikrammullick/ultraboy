#ifndef VIP_H
#define VIP_H

#include <array>

class world_attributes_t {
    // 32 worlds
    std::array<char, 1024> m_data;

  public:
    void write_h(uint32_t addr, uint16_t val);
};

class vip_t {
    world_attributes_t m_world_attributes;

  public:
    void write_h(uint32_t addr, uint16_t val);
};

#endif // VIP_H
