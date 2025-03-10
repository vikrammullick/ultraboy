#ifndef PPU_H
#define PPU_H

#include "constants.hpp"

#include <array>
#include <cstdint>
#include <vector>

class ppu_t {
    uint8_t m_LCDC = 0;
    uint8_t m_STAT = 0;
    uint8_t m_SCY = 0;
    uint8_t m_SCX = 0;
    uint8_t m_LY = 0;
    uint8_t m_BGP = 0;
    uint8_t m_OBP0 = 0;
    uint8_t m_OBP1 = 0;
    uint8_t m_WX = 0;
    uint8_t m_WY = 0;

    uint32_t m_ticks = 0;
    bool m_vblank_interrupt = false;

    std::vector<uint8_t> m_tile_data;
    std::vector<uint8_t> m_tile_map;
    std::vector<uint8_t> m_oam;

    bool ppu_enable() { return m_LCDC & (1 << 7); }
    bool bg_win_tile_data_area() { return m_LCDC & (1 << 4); }
    bool bg_tile_map_area() { return m_LCDC & (1 << 3); }

  public:
    ppu_t();

    void tick();

    void write(uint16_t addr, uint8_t val);
    uint8_t read(uint16_t addr);

    void redraw(uint16_t addr, uint8_t val);

    bool check_vblank_interrupt() {
        bool ret = m_vblank_interrupt;
        m_vblank_interrupt = false;
        return ret;
    }
};

#endif // PPU_H
