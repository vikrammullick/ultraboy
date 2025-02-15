#ifndef EMULATOR_H
#define EMULATOR_H

#include "constants.hpp"
#include "cpu.hpp"
#include "memory.hpp"

class emulator_t {
    memory_t m_memory;
    cpu_t m_cpu;

  public:
    emulator_t(const std::vector<char> &rom_bytes);
    ~emulator_t();

    void run();
};

#endif // EMULATOR_H
