#ifndef TIMER_H
#define TIMER_H

#include <cstdint>

class timer_t {
    bool m_T_Clk_Sel = false;
    bool m_Tim_Z_Int = false;
    bool m_Z_Stat = false;
    bool m_T_Enb = false;

    uint16_t m_counter = 0xFFFF;
    uint16_t m_reload = 0x0000;

  public:
    void write_misc_b(uint32_t addr, uint8_t val);
    uint8_t read_misc_b(uint32_t addr);
};

#endif // TIMER_H
