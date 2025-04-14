#ifndef MEMORY_BUS_H
#define MEMORY_BUS_H

#include "game_pak.hpp"
#include "link.hpp"
#include "vip.hpp"
#include "wram.hpp"

namespace misc_hardware_registers {
constexpr size_t CDTR_ADDR = 0x20000008;
} // namespace misc_hardware_registers

class memory_bus_t {
    vip_t m_vip;
    link_t m_link;
    wram_t m_wram;
    game_pak_t m_game_pak;

  public:
    memory_bus_t(const std::vector<char> &rom_bytes);

    void write_b(uint32_t addr, uint8_t val);
    uint8_t read_b(uint32_t addr);
    void write_h(uint32_t addr, uint16_t val);
    uint16_t read_h(uint32_t addr);
};

#endif // MEMORY_BUS_H
