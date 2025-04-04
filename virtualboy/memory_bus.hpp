#ifndef MEMORY_BUS_H
#define MEMORY_BUS_H

#include "game_pak.hpp"
#include "wram.hpp"

class memory_bus_t {
    game_pak_t m_game_pak;
    wram_t m_wram;

  public:
    memory_bus_t(const std::vector<char> &rom_bytes);

    void write_b(uint32_t addr, uint8_t val);
    uint8_t read_b(uint32_t addr);
    void write_h(uint32_t addr, uint16_t val);
    uint16_t read_h(uint32_t addr);
};

#endif // MEMORY_BUS_H
