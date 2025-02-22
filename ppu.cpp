#include "ppu.hpp"
#include "sdl_helpers.hpp"

#include <iostream>

using namespace std;

ppu_t::ppu_t() {}

void ppu_t::tick() {}

void ppu_t::write(uint16_t addr, uint8_t val) {
    if (addr == constants::LCDC) {
        m_LCDC = val;
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

    if (addr >= constants::TILE_DATA_START &&
        addr <= constants::TILE_DATA_END) {
        m_tile_data[addr - constants::TILE_DATA_START] = val;
        return;
    }

    if (addr >= constants::TILE_MAP_START && addr <= constants::TILE_MAP_END) {
        m_tile_map[addr - constants::TILE_MAP_START] = val;
        return;
    }

    cout << "addr " << hex << addr << endl;
    assert(false);
}

uint8_t ppu_t::read(uint16_t addr) {
    if (addr == constants::LCDC) {
        return m_LCDC;
    }

    if (addr == constants::SCY) {
        return m_SCY;
    }

    if (addr == constants::SCX) {
        return m_SCX;
    }

    if (addr == constants::LY) {
        // TODO: set this properly by ppu
        return m_LY;
    }

    if (addr == constants::BGP) {
        return m_BGP;
    }

    if (addr >= constants::TILE_DATA_START &&
        addr <= constants::TILE_DATA_END) {
        return m_tile_data[addr - constants::TILE_DATA_START];
    }

    if (addr >= constants::TILE_MAP_START && addr <= constants::TILE_MAP_END) {
        return m_tile_map[addr - constants::TILE_MAP_START];
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

    sdl_update_tile_map(m_LCDC & (1 << 4),
                        m_LCDC & (1 << 3) ? &m_tile_map[1024] : &m_tile_map[0],
                        (std::array<uint8_t, 16> *)&m_tile_data[0],
                        m_BGP);
}
