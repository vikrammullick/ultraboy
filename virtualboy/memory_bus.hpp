#ifndef MEMORY_H
#define MEMORY_H

#include <map>

class memory_bus_t {
  public:
    memory_bus_t(const std::vector<char> &rom_bytes);

    void write_b(uint32_t addr, uint8_t val);
    uint8_t read_b(uint32_t addr);
    void write_h(uint32_t addr, uint16_t val);
    uint16_t read_h(uint32_t addr);
};

#endif // MEMORY_H
