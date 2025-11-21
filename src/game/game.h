#ifndef GAME_H
#define GAME_H

#include "../main.h"
#include "stats.h"

// Cores
#define COLOR_BG 0x1E1E1EFF
#define COLOR_GRID 0x404040FF
#define COLOR_CYAN 0x00FFFFFF
#define COLOR_BLUE 0x0000FFFF
#define COLOR_ORANGE 0xFF7F00FF
#define COLOR_YELLOW 0xFFFF00FF
#define COLOR_GREEN 0x00FF00FF
#define COLOR_PURPLE 0x800080FF
#define COLOR_RED 0xFF0000FF

// Sistema de pontuação
#define SCORE_1_LINE 100
#define SCORE_2_LINES 300
#define SCORE_3_LINES 500
#define SCORE_4_LINES 800

// Estrutura para as peças do Tetris
typedef enum {
    EMPTY,
    CYAN,
    BLUE,
    ORANGE,
    YELLOW,
    GREEN,
    PURPLE,
    RED
} BlockType;

// Declarações de funções do jogo
void init_game(void);
void handle_game_events(SDL_Event* e);
void update_game(void);
void render_game(void);
void cleanup_game(void);
void start_game(void);
int calculate_score(int lines_cleared);
void reset_current_game(void);
bool player_file_exists(const char *player_name);

// Variáveis globais do jogo (declaradas em game.c)
extern BlockType grid[GRID_HEIGHT][GRID_WIDTH];
extern int current_piece[4][4];
extern int current_x, current_y, current_type;

#endif