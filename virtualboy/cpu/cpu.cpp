#include "cpu.hpp"

#include <bitset>
#include <iostream>
#include <sstream>

using namespace std;

cpu_t::cpu_t(memory_bus_t &memory_bus) : m_memory_bus(memory_bus) {
    // initial cpu state (will add more as more fields added)
    m_state.pc = 0xFFFFFFF0;
    m_state.regs[0] = 0;

    m_state.psw_zero = false;
    m_state.psw_sign = false;
    m_state.psw_overflow = false;
    m_state.psw_carry = false;
    m_state.psw_fp_precision_degredation = false;
    m_state.psw_fp_underflow = false;
    m_state.psw_fp_overflow = false;
    m_state.psw_fp_zero_division = false;
    m_state.psw_fp_invalid_operation = false;
    m_state.psw_fp_reserved_operand = false;
    m_state.psw_interrupt_disable = false;
    m_state.psw_address_trap_enable = false;
    m_state.psw_exception_pending = false;
    m_state.psw_nmi_pending = true;
    m_state.psw_interrupt_mask_level = 0;
}

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

uint32_t combine_into_word(uint16_t hi, uint16_t lo) { return (hi << 16) + lo; }

uint32_t cpu_t::read_word(uint32_t addr) {
    addr = word_mask(addr);
    uint16_t lo = m_memory_bus.read_h(addr);
    uint16_t hi = m_memory_bus.read_h(addr + 2);
    return combine_into_word(hi, lo);
}
void cpu_t::write_word(uint32_t addr, uint32_t value) {
    addr = word_mask(addr);
    m_memory_bus.write_h(addr, value & 0xFFFF);
    m_memory_bus.write_h(addr + 2, (value >> 16) & 0xFFFF);
}

uint16_t cpu_t::read_pc_halfword() {
    uint16_t ret = m_memory_bus.read_h(m_state.pc);
    m_state.pc += 2;
    return ret;
}

int32_t cpu_t::get_disp_26(uint16_t hi) {
    std::bitset<26> twenty_six_0 =
        combine_into_word(hi, read_pc_halfword()) & ~(0b111111 << 26);
    return sign_extend(twenty_six_0);
}

void cpu_t::set_zero_and_sign(uint32_t res) {
    m_state.psw_zero = res == 0;
    m_state.psw_sign = res & 0x80000000;
}

void cpu_t::set_psw(uint32_t val) {
    m_state.psw_zero = (val >> 0) & 0b1;
    m_state.psw_sign = (val >> 1) & 0b1;
    m_state.psw_overflow = (val >> 2) & 0b1;
    m_state.psw_carry = (val >> 3) & 0b1;
    m_state.psw_fp_precision_degredation = (val >> 4) & 0b1;
    m_state.psw_fp_underflow = (val >> 5) & 0b1;
    m_state.psw_fp_overflow = (val >> 6) & 0b1;
    m_state.psw_fp_zero_division = (val >> 7) & 0b1;
    m_state.psw_fp_invalid_operation = (val >> 8) & 0b1;
    m_state.psw_fp_reserved_operand = (val >> 9) & 0b1;
    m_state.psw_interrupt_disable = (val >> 12) & 0b1;
    m_state.psw_address_trap_enable = (val >> 13) & 0b1;
    m_state.psw_exception_pending = (val >> 14) & 0b1;
    m_state.psw_nmi_pending = (val >> 15) & 0b1;
    m_state.psw_interrupt_mask_level = (val >> 16) & 0b1111;
}

uint32_t cpu_t::add(uint32_t op1, uint32_t op2) {
    uint32_t out = op1 + op2;
    set_zero_and_sign(out);
    m_state.psw_carry = op1 > out;
    m_state.psw_overflow = (~(op1 ^ op2) & (out ^ op1)) & 0x80000000;
    return out;
}

uint32_t cpu_t::sub(uint32_t op1, uint32_t op2) {
    uint32_t out = op1 - op2;
    set_zero_and_sign(out);
    m_state.psw_carry = op2 > op1;
    m_state.psw_overflow = ((op1 ^ op2) & (out ^ op1)) & 0x80000000;
    return out;
}

void cpu_t::tick() {
    // fetch
    uint16_t inst = read_pc_halfword();

    op_type_t opcode = static_cast<op_type_t>(inst >> 10);

    if ((inst >> 13) == BCOND_100) {
        uint8_t cond = (inst >> 9) & 0b0111;
        bool invert = (inst >> 9) & 0b1000;
        bool cond_eval = [this, cond]() {
            switch (cond) {
            case 0:
                return m_state.psw_overflow;
            case 1:
                return m_state.psw_carry;
            case 2:
                return m_state.psw_zero;
            case 3:
                return m_state.psw_carry || m_state.psw_zero;
            case 4:
                return m_state.psw_sign;
            case 5:
                return true;
            case 6:
                return m_state.psw_overflow != m_state.psw_sign;
            case 7:
                return (m_state.psw_overflow != m_state.psw_sign) ||
                       m_state.psw_zero;
            default:
                return false;
            }
        }();
        if (cond_eval != invert) {
            std::bitset<9> nine_0 = (inst >> 0) & 0b111111111;
            int32_t disp = sign_extend(nine_0);
            m_state.pc += disp - 2;
        }
        return;
    }

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
        reg2 = m_memory_bus.read_b(reg1 + sign_extend_16(read_pc_halfword()));
        break;
    case op_type_t::INH_111001:
        reg2 = m_memory_bus.read_h(reg1 + sign_extend_16(read_pc_halfword()));
        break;
    case op_type_t::LDB_110000:
        reg2 = sign_extend_8(
            m_memory_bus.read_b(reg1 + sign_extend_16(read_pc_halfword())));
        break;
    case op_type_t::LDH_110001:
        reg2 = sign_extend_16(
            m_memory_bus.read_h(reg1 + sign_extend_16(read_pc_halfword())));
        break;
    case op_type_t::INW_111011:
    case op_type_t::LDW_110011:
        reg2 = read_word(reg1 + sign_extend_16(read_pc_halfword()));
        break;
    // store and output
    case op_type_t::OUTB_111100:
    case op_type_t::STB_110100:
        m_memory_bus.write_b(reg1 + sign_extend_16(read_pc_halfword()),
                             reg2 & 0xFF);
        break;
    case op_type_t::OUTH_111101:
    case op_type_t::STH_110101:
        m_memory_bus.write_h(reg1 + sign_extend_16(read_pc_halfword()),
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
    case op_type_t::CMP_000011:
        sub(reg2, reg1);
        break;
    case op_type_t::SUB_000010:
        reg2 = sub(reg2, reg1);
        break;
    // bitwise
    case op_type_t::AND_001101:
        reg2 &= reg1;
        set_zero_and_sign(reg2);
        m_state.psw_overflow = false;
        break;
    case op_type_t::ANDI_101101:
        reg2 = reg1 & read_pc_halfword();
        set_zero_and_sign(reg2);
        m_state.psw_overflow = false;
        break;
    // cpu control
    case op_type_t::JAL_101011: {
        int32_t disp = get_disp_26(inst);
        m_state.regs[31] = m_state.pc;
        m_state.pc += disp - 4;
        break;
    }
    case op_type_t::JMP_000110:
        m_state.pc = reg1;
        break;
    case op_type_t::JR_101010:
        m_state.pc += get_disp_26(inst) - 4;
        break;
    case op_type_t::LDSR_011100:
        switch (static_cast<system_register_t>(five_1.to_ulong())) {
        case system_register_t::CHCW:
            // TODO: configure instruction cache
            break;
        case system_register_t::PSW:
            set_psw(reg2);
            break;
        default:
            cout << "ldsr not implemented: 0b" << five_1 << endl;
            assert(false);
        }
        break;
    // standalone
    case op_type_t::SEI_011110:
        m_state.psw_interrupt_disable = true;
        break;
    default:
        cout << "opcode unimplemented: 0b"
             << std::bitset<6>(static_cast<uint8_t>(opcode)) << endl;
        assert(false);
    }
}
