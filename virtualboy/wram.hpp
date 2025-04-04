#ifndef WRAM_H
#define WRAM_H

#include <vector>

class wram_t {
    std::vector<char> m_bytes;

  public:
    wram_t();

    void write_h(uint32_t addr, uint16_t val);
};

#endif // WRAM_H
