#ifndef MEMORY_H
#define MEMORY_H

#include <vector>

class memory_t {
    std::vector<uint8_t> m_mem;

  public:
    memory_t();

    void write(uint16_t addr, uint8_t val);
    uint8_t read(uint16_t addr);

    bool operator==(const memory_t &) const = default;
};

#endif // MEMORY_H
