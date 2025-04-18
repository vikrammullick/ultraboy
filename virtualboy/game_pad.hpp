#ifndef GAME_PAD_H
#define GAME_PAD_H

#include <cstdint>

class game_pad_t {
  public:
    void write_misc_b(uint32_t addr, uint8_t val);
    uint8_t read_misc_b(uint32_t addr);
};

#endif // GAME_PAD_H
