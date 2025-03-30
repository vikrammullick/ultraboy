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

uint32_t word_mask(uint32_t addr) { return addr & 0xFFFFFFFC; }

uint32_t cpu_t::read_word(uint32_t addr) {
    addr = word_mask(addr);
    uint16_t lo = m_memory.read_h(addr);
    uint16_t hi = m_memory.read_h(addr + 2);
    return (hi << 16) + lo;
}
void cpu_t::write_word(uint32_t addr, uint32_t value) {
    addr = word_mask(addr);
    m_memory.write_h(addr, value & 0xFFFF);
    m_memory.write_h(addr + 2, (value >> 16) & 0xFFFF);
}

uint16_t cpu_t::read_pc_halfword() {
    uint16_t ret = m_memory.read_h(m_state.pc);
    m_state.pc += 2;
    return ret;
}

uint32_t cpu_t::add(uint32_t op1, uint32_t op2) {
    uint32_t out = op1 + op2;
    m_state.psw_zero = out == 0;
    m_state.psw_sign = out & 0x80000000;
    m_state.psw_carry = op1 > out;
    m_state.psw_overflow = (~(op1 ^ op2) & (out ^ op1)) & 0x80000000;
    return out;
}

uint32_t cpu_t::sub(uint32_t op1, uint32_t op2) {
    uint32_t out = op1 - op2;
    m_state.psw_zero = out == 0;
    m_state.psw_sign = out & 0x80000000;
    m_state.psw_carry = op2 > op1;
    m_state.psw_overflow = ((op1 ^ op2) & (out ^ op1)) & 0x80000000;
    return out;
}

void cpu_t::tick() {
    // fetch
    uint16_t inst = read_pc_halfword();

    op_type_t opcode = static_cast<op_type_t>(inst >> 10);

    std::bitset<5> five_0 = (inst >> 5) & 0b11111;
    std::bitset<5> five_1 = (inst >> 0) & 0b11111;

    uint32_t &reg2 = m_state.regs[five_0.to_ulong()];
    uint32_t &reg1 = m_state.regs[five_1.to_ulong()];

    switch (opcode) {
    // register transfer
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
    // load and input
    case op_type_t::INB_111000:
        reg2 = m_memory.read_b(reg1 + sign_extend_16(read_pc_halfword()));
        break;
    case op_type_t::INH_111001:
        reg2 = m_memory.read_h(reg1 + sign_extend_16(read_pc_halfword()));
        break;
    case op_type_t::LDB_110000:
        reg2 = sign_extend_8(
            m_memory.read_b(reg1 + sign_extend_16(read_pc_halfword())));
        break;
    case op_type_t::LDH_110001:
        reg2 = sign_extend_16(
            m_memory.read_h(reg1 + sign_extend_16(read_pc_halfword())));
        break;
    case op_type_t::INW_111011:
    case op_type_t::LDW_110011:
        reg2 = read_word(reg1 + sign_extend_16(read_pc_halfword()));
        break;
    // store and output
    case op_type_t::OUTB_111100:
    case op_type_t::STB_110100:
        m_memory.write_b(reg1 + sign_extend_16(read_pc_halfword()),
                         reg2 & 0xFF);
        break;
    case op_type_t::OUTH_111101:
    case op_type_t::STH_110101:
        m_memory.write_h(reg1 + sign_extend_16(read_pc_halfword()),
                         reg2 & 0xFFFF);
        break;
    case op_type_t::OUTW_111111:
    case op_type_t::STW_110111:
        write_word(reg1 + sign_extend_16(read_pc_halfword()), reg2);
        break;
    // arithmetic
    case op_type_t::ADD_010001:
        reg2 = add(reg2, sign_extend(five_1));
        break;
    case op_type_t::ADD_000001:
        reg2 = add(reg2, reg1);
        break;
    case op_type_t::ADDI_101001:
        reg2 = add(reg1, sign_extend_16(read_pc_halfword()));
        break;
    case op_type_t::CMP_010011:
        sub(reg2, sign_extend(five_1));
        break;
    default:
        assert(false);
    }
}
