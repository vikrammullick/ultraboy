#include "link.hpp"

#include <iostream>

using namespace std;

namespace io_registers {
constexpr size_t CDTR_ADDR = 0x00000008;
} // namespace io_registers

#define ADD_REGISTER_OP(regaddr, reg, op)                                      \
    if (addr == io_registers::regaddr##_ADDR) {                                \
        m_##reg.op(val);                                                       \
        return;                                                                \
    }

void link_t::write_b(uint32_t addr, uint8_t val) {
    ADD_REGISTER_OP(CDTR, CDTR, write);

    cout << std::hex << addr << ": " << static_cast<uint16_t>(val) << endl;
    assert(false);
}

#define ADD_REGISTER_READ(reg)                                                 \
    if (addr == io_registers::reg##_ADDR) {                                    \
        return m_##reg.read();                                                 \
    }

uint8_t link_t::read_b(uint32_t addr) {
    ADD_REGISTER_READ(CDTR);

    cout << std::hex << addr << endl;
    assert(false);
}
