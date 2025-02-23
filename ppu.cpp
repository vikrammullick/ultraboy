#include "ppu.hpp"
#include "sdl_helpers.hpp"

#include <iostream>

using namespace std;

ppu_t::ppu_t() {}

void ppu_t::tick() {
    if (!ppu_enable()) {
        return;
    }

    m_ticks++;
    if (m_ticks == 70224) {
        m_ticks = 0;
    }

    m_LY = m_ticks / 456;

    if (m_LY == 144) {
        m_vblank_interrupt = true;
    }
}

void ppu_t::write(uint16_t addr, uint8_t val) {
    if (addr == constants::LCDC) {
        m_LCDC = val;
        return;
    }

    if (addr == constants::STAT) {
        m_STAT = val & 0b11111100;
        return;
    }

    if (addr == constants::SCY) {
        m_SCY = val;
        return;
    }

    if (addr == constants::SCX) {
        m_SCX = val;
        return;
    }

    if (addr == constants::LY) {
        // read only
        assert(false);
    }

    if (addr == constants::BGP) {
        m_BGP = val;
        return;
    }

    if (addr == constants::OBP0) {
        m_OBP0 = val;
        return;
    }

    if (addr == constants::OBP1) {
        m_OBP1 = val;
        return;
    }

    if (addr == constants::WX) {
        m_WY = val;
        return;
    }

    if (addr == constants::WY) {
        m_WX = val;
        return;
    }

    if (addr >= constants::TILE_DATA_START &&
        addr <= constants::TILE_DATA_END) {
        m_tile_data[addr - constants::TILE_DATA_START] = val;
        return;
    }

    if (addr >= constants::TILE_MAP_START && addr <= constants::TILE_MAP_END) {
        m_tile_map[addr - constants::TILE_MAP_START] = val;
        return;
    }

    if (addr >= constants::OAM_START && addr <= constants::OAM_END) {
        m_oam[addr - constants::OAM_START] = val;
        return;
    }

    cout << "addr " << hex << addr << endl;
    assert(false);
}

uint8_t ppu_t::read(uint16_t addr) {
    if (addr == constants::LCDC) {
        return m_LCDC;
    }

    if (addr == constants::STAT) {
        // TODO: set lower bits correctly (LY = LYC and PPU mode)
        return m_STAT;
    }

    if (addr == constants::SCY) {
        return m_SCY;
    }

    if (addr == constants::SCX) {
        return m_SCX;
    }

    if (addr == constants::LY) {
        return m_LY;
    }

    if (addr == constants::BGP) {
        return m_BGP;
    }

    if (addr == constants::OBP0) {
        return m_OBP0;
    }

    if (addr == constants::OBP1) {
        return m_OBP1;
    }

    if (addr == constants::WX) {
        return m_WY;
    }

    if (addr == constants::WY) {
        return m_WX;
    }

    if (addr >= constants::TILE_DATA_START &&
        addr <= constants::TILE_DATA_END) {
        return m_tile_data[addr - constants::TILE_DATA_START];
    }

    if (addr >= constants::TILE_MAP_START && addr <= constants::TILE_MAP_END) {
        return m_tile_map[addr - constants::TILE_MAP_START];
    }

    if (addr >= constants::OAM_START && addr <= constants::OAM_END) {
        return m_oam[addr - constants::OAM_START];
    }

    cout << "addr " << hex << addr << endl;
    assert(false);
}

// TODO: clean up and optimize
void ppu_t::redraw(uint16_t addr, uint8_t val) {
    if (addr >= constants::TILE_DATA_START &&
        addr <= constants::TILE_DATA_END) {
        if (addr % 2 == 0) {
            sdl_update_tile_row(
                addr, val, m_tile_data[addr - constants::TILE_DATA_START + 1]);
        } else {
            sdl_update_tile_row(
                addr - 1,
                m_tile_data[addr - constants::TILE_DATA_START - 1],
                val);
        }
    }

    sdl_update_tile_map(bg_win_tile_data_area(),
                        bg_tile_map_area() ? &m_tile_map[1024] : &m_tile_map[0],
                        (std::array<uint8_t, 16> *)&m_tile_data[0],
                        m_BGP);
}
