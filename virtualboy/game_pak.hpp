#ifndef GAME_PAK_H
#define GAME_PAK_H

#include <vector>

struct wait_control_register_t {
    bool m_EXP1W;
    bool m_ROM1W;

    uint8_t read() { return (m_EXP1W << 1) | (m_ROM1W << 0); }

    void write(uint8_t val) {
        m_EXP1W = val & (1 << 1);
        m_ROM1W = val & (1 << 0);
    }
};

class game_pak_t {
    const std::vector<char> &m_rom_bytes;

    wait_control_register_t m_WCR;

  public:
    game_pak_t(const std::vector<char> &rom_bytes);

    void write_misc_b(uint32_t addr, uint8_t val);
    uint8_t read_misc_b(uint32_t addr);

    uint8_t rom_read_b(uint32_t addr);
    uint16_t rom_read_h(uint32_t addr);
};

#endif // GAME_PAK_H
