#ifndef MEMORY_H
#define MEMORY_H

#include <map>

class memory_t {

  public:
    memory_t();

    void write(uint32_t addr, uint8_t val);
    uint8_t read(uint32_t addr);

    bool operator==(const memory_t &) const = default;

    std::map<uint32_t, uint8_t> m_mem;
};

#endif // MEMORY_H
