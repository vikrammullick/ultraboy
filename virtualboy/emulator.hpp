#ifndef EMULATOR_H
#define EMULATOR_H

#include "cpu.hpp"
#include "memory_bus.hpp"

#include <vector>

class emulator_t {
    memory_bus_t m_memory_bus;
    cpu_t m_cpu;

  public:
    emulator_t(const std::vector<char> &rom_bytes);

    void run();
};

#endif // EMULATOR_H
