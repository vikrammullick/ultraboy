#ifndef CPU_H
#define CPU_H

#include "memory.hpp"

enum inst_type_t {
    NOP,
    JR,
    JP,
    JP_HL,
    LD8,
    LD8_HL,
    INC,
    INC_HL,
    DEC,
    INC16,
    ADD16,
    DEC16,
    LD_REG_REG,
    LD_REG_FROM_HL,
    LD16,
    LDMEM16,
    LDFROMMEM,
    LD_INTO_C_OFFSET,
    LD_FROM_C_OFFSET,
    LD_INTO_IMM_OFFSET,
    LD_FROM_IMM_OFFSET,
    LD_INTO_IMM,
    LD_FROM_IMM,
    LD_HL,
    ADD,
    AND,
    XOR,
    OR,
    SUB,
    BIT,
    CALL,
    RST,
    PUSH,
    POP,
    RET,
    RETI,
    ADD_IMM,
    SUB_IMM,
    AND_IMM,
    CP_IMM,
    CP_HL,
    ADD_HL,
    XOR_HL,
    CPL,
    DI,
    EI,

    RLC,
    RRC,
    RL,
    RR,
    SLA,
    SRA,
    SWAP,
    SRL,
    RLC_HL,
    RRC_HL,
    RL_HL,
    RR_HL,
    SLA_HL,
    SRA_HL,
    SWAP_HL,
    SRL_HL,

    RLCA,
    RRCA,
    RLA,
    RRA,
};

class cpu_t {
    // TODO: temporary to aid in development
    bool m_failed = false;
    uint16_t m_fetch_pc;

    memory_t &m_memory;

    std::array<uint8_t, 12> m_reg_file;

    bool m_IME = false;
    bool m_pending_IME = false;
    uint8_t m_IF = 0;
    uint8_t m_IE = 0;

    uint8_t m_TMA = 0;
    uint8_t m_TAC = 0;

    uint8_t m_lo;
    uint8_t m_hi;

    uint8_t m_opcode;
    bool prefixed;
    inst_type_t m_inst_type;

    void fetch();
    void decode();
    void execute();

    void set_inst_type();

    void write(uint16_t addr, uint8_t val);
    uint8_t read(uint16_t addr);

    // operands
    uint16_t n16() { return (m_hi << 8) + m_lo; }

    // opcode components
    uint8_t opx() { return m_opcode >> 4; }
    uint8_t opy() { return m_opcode & 0xF; }

    // registers
    uint16_t &r16x() {
        switch (opx()) {
        case 0x0:
        case 0xC:
            return BC();
        case 0x1:
        case 0xD:
            return DE();
        case 0x2:
        case 0xE:
            return HL();
        case 0x3:
            return SP();
        case 0xF:
            return AF();
        default:
            assert(false);
        }
    }

    uint8_t &ordered_reg(uint8_t order) {
        switch (order) {
        case 0x0:
            return B();
        case 0x1:
            return C();
        case 0x2:
            return D();
        case 0x3:
            return E();
        case 0x4:
            return H();
        case 0x5:
            return L();
        case 0x7:
            return A();
        default:
            assert(false);
        }
    }

    uint8_t &r8x() { return ordered_reg((opx() % 4) * 2 + opy() / 8); }
    uint8_t &r8y() { return ordered_reg(opy() & 0b111); }

    void setFlag(bool s, size_t i) {
        if (s) {
            F() |= (1 << i);
        } else {
            F() &= ~(1 << i);
        }
    };
    void setZ(bool s) { setFlag(s, 7); }
    void setN(bool s) { setFlag(s, 6); }
    void setH(bool s) { setFlag(s, 5); }
    void setC(bool s) { setFlag(s, 4); }
    bool z() { return F() & (1 << 7); }
    bool n() { return F() & (1 << 6); }
    bool h() { return F() & (1 << 5); }
    bool c() { return F() & (1 << 4); }

    bool flag_cond() {
        switch (m_opcode) {
        case 0x18:
        case 0xC3:
        case 0xC9:
        case 0xCD:
            return true;
        case 0x20:
        case 0xC0:
        case 0xC2:
        case 0xC4:
            return !z();
        case 0x28:
        case 0xC8:
        case 0xCA:
        case 0xCC:
            return z();
        case 0x38:
        case 0xD8:
        case 0xDA:
        case 0xDC:
            return c();
        case 0x30:
        case 0xD0:
        case 0xD2:
        case 0xD4:
            return !c();
        default:
            assert(false);
        }
    }

    uint16_t &ith_r16(size_t i) { return *(uint16_t *)&m_reg_file[i * 2]; }
    uint8_t &ith_r8(size_t i) { return m_reg_file[i]; }

    uint8_t &F() { return ith_r8(0); }
    uint8_t &A() { return ith_r8(1); }
    uint8_t &C() { return ith_r8(2); }
    uint8_t &B() { return ith_r8(3); }
    uint8_t &E() { return ith_r8(4); }
    uint8_t &D() { return ith_r8(5); }
    uint8_t &L() { return ith_r8(6); }
    uint8_t &H() { return ith_r8(7); }
    uint16_t &AF() { return ith_r16(0); }
    uint16_t &BC() { return ith_r16(1); }
    uint16_t &DE() { return ith_r16(2); }
    uint16_t &HL() { return ith_r16(3); }
    uint16_t &SP() { return ith_r16(4); }
    uint16_t &PC() { return ith_r16(5); }

    void call(uint16_t v);

    void alu_cp(uint8_t operand);
    void alu_add(uint8_t operand);
    void alu_and(uint8_t operand);
    void alu_sub(uint8_t operand);
    void alu_xor(uint8_t operand);

    uint8_t alu_inc(uint8_t operand);

    uint8_t alu_rlc(uint8_t operand);
    uint8_t alu_rrc(uint8_t operand);
    uint8_t alu_rl(uint8_t operand);
    uint8_t alu_rr(uint8_t operand);
    uint8_t alu_sla(uint8_t operand);
    uint8_t alu_sra(uint8_t operand);
    uint8_t alu_swap(uint8_t operand);
    uint8_t alu_srl(uint8_t operand);

  public:
    cpu_t(memory_t &memory);

    void tick();

    void request_vblank_interrupt() { m_IF |= 0b1; }
};

#endif // CPU_H
