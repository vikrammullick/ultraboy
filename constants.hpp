#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

namespace constants {

constexpr uint32_t MEM_SIZE = 1 << 16;

constexpr uint64_t NANOS_PER_TICK = 238; // 4.194 MHz

constexpr uint16_t PC_START = 0x0000;

constexpr uint8_t PREFIX = 0xCB;

constexpr uint16_t BOOT_ROM_DISABLE = 0xFF50;

// ppu constants
constexpr uint16_t LCDC = 0xFF40;
constexpr uint16_t SCY = 0xFF42;
constexpr uint16_t SCX = 0xFF43;
constexpr uint16_t LY = 0xFF44;
constexpr uint16_t BGP = 0xFF47;

constexpr uint16_t TILE_DATA_START = 0x8000;
constexpr uint16_t TILE_DATA_END = 0x97FF;
constexpr uint32_t TILE_DATA_SIZE = TILE_DATA_END - TILE_DATA_START + 1;
constexpr uint16_t TILE_MAP_START = 0x9800;
constexpr uint16_t TILE_MAP_END = 0x9FFF;
constexpr uint32_t TILE_MAP_SIZE = TILE_MAP_END - TILE_MAP_START + 1;

constexpr uint32_t DISPLAY_SCALE = 1;

constexpr uint32_t TILE_SIZE = 8;
constexpr uint32_t TILE_DATA_VIEWER_X_LEN = 16;
constexpr uint32_t TILE_DATA_VIEWER_Y_LEN = 24;

constexpr uint32_t TILE_MAP_VIEWER_X_LEN = 32;
constexpr uint32_t TILE_MAP_VIEWER_Y_LEN = 64;

} // namespace constants

#endif // CONSTANTS_H
