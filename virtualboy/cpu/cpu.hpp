#ifndef CPU_H
#define CPU_H

#include "memory_bus.hpp"

#include <array>
#include <bitset>

struct cpu_state_t {
    uint32_t pc;
    std::array<uint32_t, 32> regs;

    bool psw_zero;
    bool psw_sign;
    bool psw_overflow;
    bool psw_carry;
    bool psw_fp_precision_degredation;
    bool psw_fp_underflow;
    bool psw_fp_overflow;
    bool psw_fp_zero_division;
    bool psw_fp_invalid_operation;
    bool psw_fp_reserved_operand;
    bool psw_interrupt_disable;
    bool psw_address_trap_enable;
    bool psw_exception_pending;
    bool psw_nmi_pending;
    uint8_t psw_interrupt_mask_level;

    bool operator==(const cpu_state_t &) const = default;
};

enum class system_register_t : uint8_t {
    CHCW = 24,
    PSW = 5,
};

constexpr uint8_t BCOND_100 = 0b100;

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
    // TODO: DIV/MUL
    SUB_000010 = 0b000010,

    AND_001101 = 0b001101,
    ANDI_101101 = 0b101101,

    // BCOND_100 defined above
    JAL_101011 = 0b101011,
    JMP_000110 = 0b000110,
    JR_101010 = 0b101010,
    LDSR_011100 = 0b011100,

    SEI_011110 = 0b011110,
};

class cpu_t {
    cpu_state_t m_state;
    memory_bus_t &m_memory_bus;

    uint32_t read_word(uint32_t addr);
    void write_word(uint32_t addr, uint32_t value);
    uint16_t read_pc_halfword();
    int32_t get_disp_26(uint16_t hi);

    void set_zero_and_sign(uint32_t res);
    void set_psw(uint32_t val);
    uint32_t add(uint32_t op1, uint32_t op2);
    uint32_t sub(uint32_t op1, uint32_t op2);

  public:
    cpu_t(memory_bus_t &memory_bus);

    void tick();

    void load_state(const cpu_state_t &state) { m_state = state; }

    cpu_state_t dump_state() { return m_state; }
};

#endif // CPU_H
