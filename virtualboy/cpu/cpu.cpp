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

int32_t sign_extend_16(uint16_t value) {
    return sign_extend(std::bitset<16>(value));
}

int32_t sign_extend_8(uint8_t value) {
    return sign_extend(std::bitset<8>(value));
}

uint16_t cpu_t::read_pc_halfword() {
    uint8_t lo = m_memory.read(m_state.pc++);
    uint8_t hi = m_memory.read(m_state.pc++);
    return (hi << 8) + lo;
}

void cpu_t::tick() {
    // fetch
    uint16_t inst = read_pc_halfword();

    op_type_t opcode = static_cast<op_type_t>(inst >> 10);

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
    case op_type_t::MOVEA_101000:
        reg2 = reg1 + sign_extend_16(read_pc_halfword());
        break;
    case op_type_t::MOVHI_101111:
        reg2 = reg1 + (read_pc_halfword() << 16);
        break;
    case op_type_t::INB_111000:
        reg2 = m_memory.read(reg1 + sign_extend_16(read_pc_halfword()));
        break;
    case op_type_t::LDB_110000:
        reg2 = sign_extend_8(
            m_memory.read(reg1 + sign_extend_16(read_pc_halfword())));
        break;
    default:
        assert(false);
    }
}
