#include "cpu.hpp"

#include <bitset>
#include <iostream>
#include <sstream>

using namespace std;

cpu_t::cpu_t(memory_t &memory) : m_memory(memory) {}

template <size_t N> int32_t sign_extend(const std::bitset<N> &bits) {
    static_assert(N <= 32, "Can only sign-extend up to 32 bits");

    uint32_t value = static_cast<uint32_t>(bits.to_ulong());

    // If the sign bit (bit N-1) is set, extend with 1s
    if (bits[N - 1]) {
        // Set upper bits to 1
        uint32_t mask = ~((1u << N) - 1);
        return static_cast<int32_t>(value | mask);
    } else {
        return static_cast<int32_t>(value);
    }
}

void cpu_t::tick() {
    // fetch
    uint8_t inst_lo = m_memory.read(m_state.pc++);
    uint8_t inst_hi = m_memory.read(m_state.pc++);
    uint16_t inst = (inst_hi << 8) + inst_lo;

    uint8_t opcode_u8 = inst >> 10;
    op_type_t opcode = static_cast<op_type_t>(opcode_u8);

    std::bitset<5> five_0 = (inst >> 5) & 0b11111;
    std::bitset<5> five_1 = (inst >> 0) & 0b11111;

    auto &reg2 = m_state.regs[five_0.to_ulong()];
    auto &reg1 = m_state.regs[five_1.to_ulong()];

    switch (opcode) {
    case op_type_t::MOV_010000:
        reg2 = sign_extend(five_1);
        break;
    case op_type_t::MOV_000000:
        reg2 = reg1;
        break;
    default:
        assert(false);
    }
}
