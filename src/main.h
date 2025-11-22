#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>
#include <stdbool.h>

// Configurações da janela
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 729
#define BLOCK_SIZE 30
#define GRID_WIDTH 10
#define GRID_HEIGHT 20

// Estados do jogo
typedef enum {
    MENU_MAIN,
    MENU_PLAYER_INPUT,
    GAME_ACTIVE,
    GAME_PAUSED,
    HISTORY,
    CREDITS
} GameState;

// Estrutura para estatísticas do jogo atual
typedef struct {
    int score;
    int lines_cleared;
    int level;
} GameStats;

// Estrutura para o jogo
typedef struct {
    GameState current_state;
    bool running;
    char player_name[50];
    GameStats current_game;
    bool is_new_player;
} Game;

// Variáveis globais
extern Game game;
extern SDL_Renderer* renderer;

// Declarações de funções globais
void start_game(void);
void reset_current_game(void);
bool player_file_exists(const char *player_name);

#endif