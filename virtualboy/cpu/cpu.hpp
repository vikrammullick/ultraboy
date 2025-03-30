#ifndef CPU_H
#define CPU_H

#include "memory.hpp"

#include <array>
#include <bitset>

struct cpu_state_t {
    uint32_t pc;
    std::array<uint32_t, 32> regs;

    bool psw_zero;
    bool psw_sign;
    bool psw_overflow;
    bool psw_carry;

    bool operator==(const cpu_state_t &) const = default;
};

enum class op_type_t : uint8_t {
    MOV_010000 = 0b010000,
    MOV_000000 = 0b000000,
    MOVEA_101000 = 0b101000,
    MOVHI_101111 = 0b101111,

    INB_111000 = 0b111000,
    INH_111001 = 0b111001,
    INW_111011 = 0b111011,
    LDB_110000 = 0b110000,
    LDH_110001 = 0b110001,
    LDW_110011 = 0b110011,

    OUTB_111100 = 0b111100,
    OUTH_111101 = 0b111101,
    OUTW_111111 = 0b111111,
    STB_110100 = 0b110100,
    STH_110101 = 0b110101,
    STW_110111 = 0b110111,

    ADD_010001 = 0b010001,
    ADD_000001 = 0b000001,
    ADDI_101001 = 0b101001,
    CMP_010011 = 0b010011,
    CMP_000011 = 0b000011,
};

class cpu_t {
    cpu_state_t m_state;
    memory_t &m_memory;

  public:
    cpu_t(memory_t &memory);

    void tick();

    uint32_t read_word(uint32_t addr);
    void write_word(uint32_t addr, uint32_t value);
    uint16_t read_pc_halfword();

    uint32_t add(uint32_t op1, uint32_t op2);
    uint32_t sub(uint32_t op1, uint32_t op2);

    void load_state(const cpu_state_t &state) { m_state = state; }

    cpu_state_t dump_state() { return m_state; }
};

#endif // CPU_H
