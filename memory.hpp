#ifndef MEMORY_H
#define MEMORY_H

#include "constants.hpp"

#include <array>
#include <iostream>
#include <vector>

class memory_t {
    std::array<uint8_t, constants::MEM_SIZE> m_data{};

    void write(uint16_t addr, uint8_t val);
    uint8_t read(uint16_t addr);

  public:
    memory_t(const std::vector<char> &rom_bytes);
};

#endif // MEMORY_H
