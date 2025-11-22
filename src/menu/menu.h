#ifndef MENU_H
#define MENU_H

#include "../main.h"
#include "history.h"

// Botões do menu
typedef struct {
    SDL_Rect rect;
    char* text;
    bool hovered;
} Button;

// Funções do menu
void init_menu();
void handle_menu_events(SDL_Event* e);
void render_menu();
void cleanup_menu();
void render_player_input();
void handle_player_input_events(SDL_Event* e);
void render_pause_menu();
void handle_pause_events(SDL_Event* e);
void check_existing_players();

#endif