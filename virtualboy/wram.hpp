#ifndef WRAM_H
#define WRAM_H

#include <vector>

class wram_t {
    std::vector<char> m_bytes;

  public:
    wram_t();

    void write_b(uint32_t addr, uint8_t val);
    uint8_t read_b(uint32_t addr);
    void write_h(uint32_t addr, uint16_t val);
    uint16_t read_h(uint32_t addr);
};

#endif // WRAM_H
