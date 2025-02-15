#ifndef CPU_H
#define CPU_H

#include "memory.hpp"

#include <queue>

class cpu_t {
    memory_t &m_memory;

  public:
    cpu_t(memory_t &memory);

    void tick();
};

#endif // CPU_H
