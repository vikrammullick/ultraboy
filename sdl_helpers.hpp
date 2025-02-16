#ifndef SDL_HELPERS_HPP
#define SDL_HELPERS_HPP

#include <SDL2/SDL.h>

extern SDL_Renderer *renderer;
extern SDL_Window *window;
extern SDL_Texture *texture;

extern bool g_running;

extern void sdl_poll_keyboard();
extern void sdl_refresh_screen();
extern void sdl_init_window();
extern void sdl_destroy_window();

extern void sdl_update_tile_row(uint16_t addr, uint8_t byte1, uint8_t byte2);

#endif
