#include "sdl_helpers.hpp"
#include "constants.hpp"

#include <iostream>

SDL_Renderer *td_renderer = nullptr;
SDL_Window *td_window = nullptr;
SDL_Texture *td_texture = nullptr;

SDL_Renderer *tm_renderer = nullptr;
SDL_Window *tm_window = nullptr;
SDL_Texture *tm_texture = nullptr;

using namespace std;

bool g_running = true;

void sdl_poll_keyboard() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT ||
            (event.type == SDL_WINDOWEVENT &&
             event.window.event == SDL_WINDOWEVENT_CLOSE)) {
            g_running = false;
            continue;
        }
    }
}

void sdl_init_windows() {
    SDL_Init(SDL_INIT_EVERYTHING);

    constexpr uint32_t TILE_DATA_SCREEN_WIDTH =
        constants::TILE_SIZE * constants::TILE_DATA_VIEWER_X_LEN *
        constants::DISPLAY_SCALE;
    constexpr uint32_t TILE_DATA_SCREEN_HEIGHT =
        constants::TILE_SIZE * constants::TILE_DATA_VIEWER_Y_LEN *
        constants::DISPLAY_SCALE;
    td_window = SDL_CreateWindow("tile data",
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 TILE_DATA_SCREEN_WIDTH,
                                 TILE_DATA_SCREEN_HEIGHT,
                                 SDL_WINDOW_SHOWN);
    td_renderer = SDL_CreateRenderer(td_window, -1, SDL_RENDERER_ACCELERATED);
    td_texture = SDL_CreateTexture(td_renderer,
                                   SDL_PIXELFORMAT_RGB888,
                                   SDL_TEXTUREACCESS_STREAMING,
                                   TILE_DATA_SCREEN_WIDTH,
                                   TILE_DATA_SCREEN_HEIGHT);

    constexpr uint32_t TILE_MAP_SCREEN_WIDTH =
        constants::TILE_SIZE * constants::TILE_MAP_VIEWER_X_LEN *
        constants::DISPLAY_SCALE;
    constexpr uint32_t TILE_MAP_SCREEN_HEIGHT =
        constants::TILE_SIZE * constants::TILE_MAP_VIEWER_Y_LEN *
        constants::DISPLAY_SCALE;
    tm_window = SDL_CreateWindow("tile maps",
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 TILE_MAP_SCREEN_WIDTH,
                                 TILE_MAP_SCREEN_HEIGHT,
                                 SDL_WINDOW_SHOWN);
    tm_renderer = SDL_CreateRenderer(tm_window, -1, SDL_RENDERER_ACCELERATED);
    tm_texture = SDL_CreateTexture(tm_renderer,
                                   SDL_PIXELFORMAT_RGB888,
                                   SDL_TEXTUREACCESS_STREAMING,
                                   TILE_MAP_SCREEN_WIDTH,
                                   TILE_MAP_SCREEN_HEIGHT);
}

void sdl_destroy_windows() {
    SDL_DestroyTexture(td_texture);
    SDL_DestroyRenderer(td_renderer);
    SDL_DestroyWindow(td_window);
    SDL_DestroyTexture(tm_texture);
    SDL_DestroyRenderer(tm_renderer);
    SDL_DestroyWindow(tm_window);
    SDL_Quit();
}

// TODO: clean up and optimize
void sdl_update_tile_row(uint16_t addr, uint8_t byte1, uint8_t byte2) {
    void *pixels;
    int pitch;

    SDL_LockTexture(td_texture, NULL, &pixels, &pitch);
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

    SDL_UnlockTexture(td_texture);

    SDL_RenderClear(td_renderer);
    SDL_RenderCopy(td_renderer, td_texture, NULL, NULL);
    SDL_RenderPresent(td_renderer);
}

void sdl_update_tile_map(bool data_addressing_mode,
                         uint8_t *map,
                         std::array<uint8_t, 16> *data,
                         uint8_t palette) {
    void *pixels;
    int pitch;

    SDL_LockTexture(tm_texture, NULL, &pixels, &pitch);
    Uint8 *pixelBuffer = (Uint8 *)pixels;

    for (size_t i = 0; i < 64; ++i) {
        for (size_t j = 0; j < 32; ++j) {
            uint32_t map_index = i * 32 + j;

            uint8_t tile_index = map[map_index];

            std::array<uint8_t, 16> *tile_data_ptr;
            if (data_addressing_mode) {
                tile_data_ptr = &data[tile_index];
            } else {
                int8_t signed_tile_index = tile_index;
                int32_t ind = 0x0100;
                ind += signed_tile_index;
                tile_data_ptr = &data[ind];
            }
            const auto &tile_data = *tile_data_ptr;

            for (uint32_t y = 0; y < 8; ++y) {
                for (uint32_t x = 0; x < 8; ++x) {
                    uint32_t real_y = i * 8 + y;
                    uint32_t real_x = j * 8 + x;

                    uint8_t byte1 = tile_data[2 * y];
                    uint8_t byte2 = tile_data[2 * y + 1];

                    uint32_t offset2 = 7 - x;
                    bool bit1 = byte1 & (1 << offset2);
                    bool bit2 = byte2 & (1 << offset2);
                    uint8_t shade = ((bit2 ? 1 : 0) << 1) + (bit1 ? 1 : 0);

                    uint8_t palette_shade = (palette >> (shade * 2)) & 0b11;
                    uint8_t s = 255 - (palette_shade * 85);

                    for (size_t xx = 0; xx < constants::DISPLAY_SCALE; ++xx) {
                        for (size_t yy = 0; yy < constants::DISPLAY_SCALE;
                             ++yy) {
                            uint32_t pixel_x =
                                real_x * constants::DISPLAY_SCALE + xx;
                            uint32_t pixel_y =
                                real_y * constants::DISPLAY_SCALE + yy;

                            uint32_t index =
                                pixel_y * pitch +
                                pixel_x * 4;        // Correct pixel location
                            pixelBuffer[index] = s; // B
                            pixelBuffer[index + 1] = s; // G
                            pixelBuffer[index + 2] = s; // R
                        }
                    }
                }
            }
        }
    }

    SDL_UnlockTexture(tm_texture);

    SDL_RenderClear(tm_renderer);
    SDL_RenderCopy(tm_renderer, tm_texture, NULL, NULL);
    SDL_RenderPresent(tm_renderer);
}
