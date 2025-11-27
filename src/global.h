#ifndef GLOBAL_H
#define GLOBAL_H

#include "main.h"
#include "game/stats.h"

// Declarações extern das variáveis globais
extern Game game;
extern SDL_Renderer* renderer;
extern PlayerStats current_player_stats;

// Declarações de funções globais
void start_game(void);
void reset_current_game(void);
bool player_file_exists(const char *player_name);

#endif