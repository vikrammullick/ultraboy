#ifndef LINK_H
#define LINK_H

#include <cstdint>

struct transmitted_data_register_t {
    uint8_t m_data = 0;

    uint8_t read() { return m_data; }

    void write(uint8_t val) { m_data = val; }
};

struct link_t {
    transmitted_data_register_t m_CDTR;

    void write_b(uint32_t addr, uint8_t val);
    uint8_t read_b(uint32_t addr);
};

#endif // LINK_H
