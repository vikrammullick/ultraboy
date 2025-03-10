#ifndef MEMORY_H
#define MEMORY_H

#include "constants.hpp"
#include "ppu.hpp"

#include <array>
#include <iostream>
#include <vector>

// TODO: rename to address decoder
class memory_t {
    // TODO: move into external memory
    std::vector<uint8_t> m_data;

    // TODO: move into cartridge_t
    std::vector<char> m_boot;
    std::vector<char> m_rom;

    bool m_boot_rom_disable = true;

    uint8_t m_keyboard;

    ppu_t &m_ppu;

  public:
    memory_t(const std::vector<char> &boot_bytes,
             const std::vector<char> &rom_bytes,
             ppu_t &ppu);

    void write(uint16_t addr, uint8_t val);
    uint8_t read(uint16_t addr);
};

#endif // MEMORY_H
