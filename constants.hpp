#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

namespace constants {

constexpr uint32_t MEM_SIZE = 1 << 16;

constexpr uint64_t NANOS_PER_TICK = 238; // 4.194 MHz

constexpr uint16_t PC_START = 0x0000;

constexpr uint8_t PREFIX = 0xCB;

constexpr uint16_t BOOT_ROM_DISABLE = 0xFF50;

constexpr uint16_t IF = 0xFF0F;
constexpr uint16_t IE = 0xFFFF;

// timer and div
constexpr uint16_t TMA = 0xFF06;
constexpr uint16_t TAC = 0xFF07;

// keyboard
constexpr uint16_t KEYBOARD = 0xFF00;

// ppu constants
constexpr uint16_t LCDC = 0xFF40;
constexpr uint16_t STAT = 0xFF41;
constexpr uint16_t SCY = 0xFF42;
constexpr uint16_t SCX = 0xFF43;
constexpr uint16_t LY = 0xFF44;
constexpr uint16_t OAM_TRANSFER = 0xFF46;
constexpr uint16_t BGP = 0xFF47;
constexpr uint16_t OBP0 = 0xFF48;
constexpr uint16_t OBP1 = 0xFF49;
constexpr uint16_t WX = 0xFF4A;
constexpr uint16_t WY = 0xFF4B;

constexpr uint16_t TILE_DATA_START = 0x8000;
constexpr uint16_t TILE_DATA_END = 0x97FF;
constexpr uint32_t TILE_DATA_SIZE = TILE_DATA_END - TILE_DATA_START + 1;
constexpr uint16_t TILE_MAP_START = 0x9800;
constexpr uint16_t TILE_MAP_END = 0x9FFF;
constexpr uint32_t TILE_MAP_SIZE = TILE_MAP_END - TILE_MAP_START + 1;
constexpr uint16_t OAM_START = 0xFE00;
constexpr uint16_t OAM_END = 0xFE9F;
constexpr uint32_t OAM_SIZE = OAM_END - OAM_START + 1;

constexpr uint32_t DISPLAY_SCALE = 1;

constexpr uint32_t TILE_SIZE = 8;
constexpr uint32_t TILE_DATA_VIEWER_X_LEN = 16;
constexpr uint32_t TILE_DATA_VIEWER_Y_LEN = 24;

constexpr uint32_t TILE_MAP_VIEWER_X_LEN = 32;
constexpr uint32_t TILE_MAP_VIEWER_Y_LEN = 64;

} // namespace constants

#endif // CONSTANTS_H
