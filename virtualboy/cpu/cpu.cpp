#include "cpu.hpp"

#include <iostream>
#include <sstream>

using namespace std;

cpu_t::cpu_t(memory_t &memory) : m_memory(memory) {}

void cpu_t::tick() {
    uint8_t inst_lo = m_memory.read(m_state.pc++);
    uint8_t inst_hi = m_memory.read(m_state.pc++);
    uint16_t inst = (inst_hi << 8) + inst_lo;

    // cout << std::bitset<16>(inst) << endl;

    uint8_t opcode_u8 = inst >> 10;
    op_type_t opcode = static_cast<op_type_t>(opcode_u8);

    uint8_t reg2 = (inst >> 5) & 0b11111;
    uint8_t reg1 = (inst >> 0) & 0b11111;

    switch (opcode) {
    case op_type_t::MOV_000000:
        m_state.regs[reg2] = m_state.regs[reg1];
        break;
    default:
        assert(false);
    }
}
