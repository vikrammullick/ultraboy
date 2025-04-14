#ifndef VSU_H
#define VSU_H

#include <cstdint>

struct vsu_t {
    void write_b(uint32_t addr, uint8_t val);
};

#endif // VSU_H
