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
    LDB_110000 = 0b110000,
    LDH_110001 = 0b110001,
};

class cpu_t {
    cpu_state_t m_state;
    memory_t &m_memory;

  public:
    cpu_t(memory_t &memory);

    void tick();

    uint16_t read_halfword(uint32_t addr);
    uint16_t read_pc_halfword();

    void load_state(const cpu_state_t &state) { m_state = state; }

    cpu_state_t dump_state() { return m_state; }
};

#endif // CPU_H
