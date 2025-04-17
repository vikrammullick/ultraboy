#ifndef TIMER_H
#define TIMER_H

#include <cstdint>

struct timer_control_register_t {
    bool m_T_Clk_Sel = false;
    bool m_Tim_Z_Int = false;
    bool m_Z_Stat = false;
    bool m_T_Enb = false;

    uint8_t read() {
        return (m_T_Clk_Sel << 4) | (m_Tim_Z_Int << 3) | (true << 2) |
               (m_Z_Stat << 1) | (m_T_Enb << 0);
    }

    void write(uint8_t val) {
        m_T_Clk_Sel = val & (1 << 4);
        m_Tim_Z_Int = val & (1 << 3);

        bool try_clear_Z_Stat = val & (1 << 2);
        bool new_T_Enb = val & (1 << 0);

        // From Sacred Tech Scroll: If both T-Enb is cleared and Z-Stat-Clr is
        // set while the timer is enabled, then the timer is disabled, but zero
        // status is not cleared (nor is an interrupt acknowledged).
        if (try_clear_Z_Stat && m_T_Enb && !new_T_Enb) {
            m_Z_Stat = false;
            // TODO: acknowledge timer zero interrupt: Z-Stat-Clr will clear
            // Z-Stat and acknowledge a timer zero interrupt if the counter is
            // non-zero or, if the counter is zero, if the timer is disabled.
            // This bit is always set when read.
        }

        m_T_Enb = new_T_Enb;
    }
};

struct timer_t {
    timer_control_register_t m_TCR;

    void write_misc_b(uint32_t addr, uint8_t val);
    uint8_t read_misc_b(uint32_t addr);
};

#endif // TIMER_H
