#include "link.hpp"
#include "memory_bus.hpp"

#include <iostream>

using namespace std;

#define ADD_MISC_REGISTER_OP(regaddr, reg, op)                                 \
    if (addr == misc_hardware_registers::regaddr##_ADDR) {                     \
        m_##reg.op(val);                                                       \
        return;                                                                \
    }

void link_t::write_misc_b(uint32_t addr, uint8_t val) {
    ADD_MISC_REGISTER_OP(CDTR, CDTR, write);

    cout << std::hex << addr << ": " << static_cast<uint16_t>(val) << endl;
    assert(false);
}

#define ADD_MISC_REGISTER_READ(reg)                                            \
    if (addr == misc_hardware_registers::reg##_ADDR) {                         \
        return m_##reg.read();                                                 \
    }

uint8_t link_t::read_misc_b(uint32_t addr) {
    ADD_MISC_REGISTER_READ(CDTR);

    cout << std::hex << addr << endl;
    assert(false);
}
