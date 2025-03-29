#ifndef CPU_H
#define CPU_H

#include "memory.hpp"

#include <array>

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
    MOV_000000 = 0b000000,
};

class cpu_t {
    cpu_state_t m_state;
    memory_t &m_memory;

  public:
    cpu_t(memory_t &memory);

    void tick();

    void load_state(const cpu_state_t &state) { m_state = state; }

    cpu_state_t dump_state() { return m_state; }
};

#endif // CPU_H
