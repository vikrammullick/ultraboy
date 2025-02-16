#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

namespace constants {

constexpr uint32_t TILE_SIZE = 8;
constexpr uint32_t TILE_DATA_VIEWER_X_LEN = 16;
constexpr uint32_t TILE_DATA_VIEWER_Y_LEN = 24;
constexpr uint32_t DISPLAY_SCALE = 3;

constexpr uint32_t MEM_SIZE = 1 << 16;

constexpr uint64_t NANOS_PER_TICK = 238;

constexpr uint16_t PC_START = 0x0000;

constexpr uint8_t PREFIX = 0xCB;

} // namespace constants

#endif // CONSTANTS_H
