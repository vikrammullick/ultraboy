#ifndef GAME_PAK_H
#define GAME_PAK_H

#include <vector>

class game_pak_t {
    const std::vector<char> &m_rom_bytes;

  public:
    game_pak_t(const std::vector<char> &rom_bytes);

    uint8_t rom_read_b(uint32_t addr);
    uint16_t rom_read_h(uint32_t addr);
};

#endif // GAME_PAK_H
