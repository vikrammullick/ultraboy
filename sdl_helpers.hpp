#ifndef SDL_HELPERS_HPP
#define SDL_HELPERS_HPP

#include <SDL2/SDL.h>

#include <array>

extern SDL_Renderer *td_renderer;
extern SDL_Window *td_window;
extern SDL_Texture *td_texture;

extern SDL_Renderer *tm_renderer;
extern SDL_Window *tm_window;
extern SDL_Texture *tm_texture;

extern bool g_running;

extern void sdl_poll_keyboard();

extern void sdl_init_windows();
extern void sdl_destroy_windows();

extern void sdl_update_tile_row(uint16_t addr, uint8_t byte1, uint8_t byte2);
extern void sdl_update_tile_map(bool data_addressing_mode,
                                uint8_t *map,
                                std::array<uint8_t, 16> *data);

#endif
