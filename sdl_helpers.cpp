#include "sdl_helpers.hpp"
#include "constants.hpp"

#include <iostream>

SDL_Renderer *renderer = nullptr;
SDL_Window *window = nullptr;
SDL_Texture *texture = nullptr;

using namespace std;

bool g_running = true;

void sdl_poll_keyboard() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            g_running = false;
            continue;
        }
    }
}

void sdl_refresh_screen() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void sdl_init_window() {
    SDL_Init(SDL_INIT_EVERYTHING);

    constexpr uint16_t TILE_DATA_SCREEN_WIDTH =
        constants::TILE_SIZE * constants::TILE_DATA_VIEWER_X_LEN *
        constants::DISPLAY_SCALE;
    constexpr uint16_t TILE_DATA_SCREEN_HEIGHT =
        constants::TILE_SIZE * constants::TILE_DATA_VIEWER_Y_LEN *
        constants::DISPLAY_SCALE;
    window = SDL_CreateWindow("tile data",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              TILE_DATA_SCREEN_WIDTH,
                              TILE_DATA_SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_RGB888,
                                SDL_TEXTUREACCESS_STREAMING,
                                TILE_DATA_SCREEN_WIDTH,
                                TILE_DATA_SCREEN_HEIGHT);
}

void sdl_destroy_window() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// TODO: clean up and optimizear
void sdl_update_tile_row(uint16_t addr, uint8_t byte1, uint8_t byte2) {
    void *pixels;
    int pitch;

    SDL_LockTexture(texture, NULL, &pixels, &pitch);
    Uint8 *pixelBuffer = (Uint8 *)pixels;

    uint32_t tile_index = (addr - 0x8000) / 16;

    uint32_t tile_y_index = tile_index / constants::TILE_DATA_VIEWER_X_LEN;
    uint32_t tile_x_index = tile_index % constants::TILE_DATA_VIEWER_X_LEN;

    uint32_t tile_row = ((addr - 0x8000) % 16) / 2;

    uint32_t y_pos = tile_y_index * constants::TILE_SIZE + tile_row;
    uint32_t x_pos = tile_x_index * constants::TILE_SIZE;

    for (uint32_t offset = 0; offset < 8; ++offset) {
        for (size_t i = 0; i < constants::DISPLAY_SCALE; ++i) {
            for (size_t j = 0; j < constants::DISPLAY_SCALE; ++j) {
                uint32_t pixel_x = x_pos * constants::DISPLAY_SCALE + j;
                uint32_t pixel_y = y_pos * constants::DISPLAY_SCALE + i;

                uint32_t index =
                    pixel_y * pitch + pixel_x * 4; // Correct pixel location

                uint32_t offset2 = 7 - offset;
                bool bit1 = byte1 & (1 << offset2);
                bool bit2 = byte2 & (1 << offset2);
                uint8_t shade = ((bit2 ? 1 : 0) << 1) + (bit1 ? 1 : 0);

                uint8_t s = 255 - (shade * 85);

                pixelBuffer[index] = s;     // B
                pixelBuffer[index + 1] = s; // G
                pixelBuffer[index + 2] = s; // R
            }
        }

        x_pos++;
    }

    SDL_UnlockTexture(texture);
    sdl_refresh_screen();
}
