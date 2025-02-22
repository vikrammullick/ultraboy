#ifndef PPU_H
#define PPU_H

#include "constants.hpp"

#include <array>
#include <cstdint>

class ppu_t {
    uint8_t m_LCDC;
    uint8_t m_SCY;
    uint8_t m_SCX;
    uint8_t m_LY;
    uint8_t m_BGP;

    std::array<uint8_t, constants::TILE_DATA_SIZE> m_tile_data{};
    std::array<uint8_t, constants::TILE_MAP_SIZE> m_tile_map{};

  public:
    ppu_t();

    void tick();

    void write(uint16_t addr, uint8_t val);
    uint8_t read(uint16_t addr);

    void redraw(uint16_t addr, uint8_t val);
};

#endif // PPU_H
