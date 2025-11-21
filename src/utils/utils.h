#ifndef UTILS_H
#define UTILS_H

#include "../main.h"
#include "../menu/menu.h"
#include <SDL2/SDL_ttf.h>

// Funções utilitárias
void init_fonts(void);
void cleanup_fonts(void);
void draw_text(const char* text, int x, int y, int size, SDL_Color color);
void draw_button(Button* button);
bool is_point_in_rect(int x, int y, SDL_Rect rect);
void handle_events(void);
void update(void);
void render(void);

#endif