#include "timer.hpp"
#include "memory_bus.hpp"

#include <iostream>

using namespace std;

void timer_t::write_misc_b(uint32_t addr, uint8_t val) {
    if (addr == misc_hardware_registers::TLR_ADDR) {
        // TODO: reset the current timer tick
        m_reload = (m_reload & 0xFF00) + (val << 0);
        m_counter = m_reload;
        return;
    }

    if (addr == misc_hardware_registers::THR_ADDR) {
        // TODO: reset the current timer tick
        m_reload = (val << 8) + (m_reload & 0x00FF);
        m_counter = m_reload;
        return;
    }

    if (addr == misc_hardware_registers::TCR_ADDR) {
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
        return;
    }

    cout << std::hex << addr << ": " << static_cast<uint16_t>(val) << endl;
    assert(false);
}

uint8_t timer_t::read_misc_b(uint32_t addr) {
    if (addr == misc_hardware_registers::TLR_ADDR) {
        return m_counter & 0xFF;
    }

    if (addr == misc_hardware_registers::THR_ADDR) {
        return m_counter >> 8;
    }

    if (addr == misc_hardware_registers::TCR_ADDR) {
        return (m_T_Clk_Sel << 4) | (m_Tim_Z_Int << 3) | (true << 2) |
               (m_Z_Stat << 1) | (m_T_Enb << 0);
    }

    cout << std::hex << addr << endl;
    assert(false);
}
