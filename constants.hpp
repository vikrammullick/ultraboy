#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

namespace constants {

constexpr uint32_t MEM_SIZE = 1 << 16;

constexpr uint64_t NANOS_PER_TICK = 238;

constexpr uint16_t PC_START = 0x0000;

constexpr uint8_t PREFIX = 0xCB;

} // namespace constants

#endif // CONSTANTS_H
