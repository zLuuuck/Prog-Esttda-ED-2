#include "global.h"
#include "game/stats.h"

// Definições das variáveis globais
Game game = {MENU_MAIN, true, "", {0, 0, 1, 0}, true};
SDL_Renderer* renderer = NULL;
PlayerStats current_player_stats; // Definição única da variável