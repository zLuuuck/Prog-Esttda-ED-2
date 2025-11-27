#include "game.h"
#include <time.h>
#include "../utils/utils.h"
#include "../global.h"
#include <stdio.h> 
#include <string.h>

// Matriz do grid do jogo
BlockType grid[GRID_HEIGHT][GRID_WIDTH];

// Peças do Tetris (formato I, J, L, O, S, T, Z)
const int tetrominos[7][4][4] = {
    // I
    {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}},
    // J
    {{2,0,0,0}, {2,2,2,0}, {0,0,0,0}, {0,0,0,0}},
    // L
    {{0,0,3,0}, {3,3,3,0}, {0,0,0,0}, {0,0,0,0}},
    // O
    {{0,4,4,0}, {0,4,4,0}, {0,0,0,0}, {0,0,0,0}},
    // S
    {{0,5,5,0}, {5,5,0,0}, {0,0,0,0}, {0,0,0,0}},
    // T
    {{0,6,0,0}, {6,6,6,0}, {0,0,0,0}, {0,0,0,0}},
    // Z
    {{7,7,0,0}, {0,7,7,0}, {0,0,0,0}, {0,0,0,0}}
};

// Variáveis da peça atual
int current_piece[4][4];
int current_x, current_y;
int current_type;
Uint32 last_time;

// Funções do jogo
void init_grid() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            grid[y][x] = EMPTY;
        }
    }
}

int merge_piece() {
    int blocks_placed = 0;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (current_piece[y][x]) {
                grid[current_y + y][current_x + x] = current_piece[y][x];
                blocks_placed++;
            }
        }
    }
    return blocks_placed;
}



void new_piece() {
    current_type = rand() % 7;
    current_x = GRID_WIDTH / 2 - 2;
    current_y = 0;
    
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            current_piece[y][x] = tetrominos[current_type][y][x];
        }
    }
}

bool check_collision() {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (current_piece[y][x]) {
                int new_x = current_x + x;
                int new_y = current_y + y;
                
                if (new_x < 0 || new_x >= GRID_WIDTH || 
                    new_y >= GRID_HEIGHT || 
                    (new_y >= 0 && grid[new_y][new_x] != EMPTY)) {
                    return true;
                }
            }
        }
    }
    return false;
}

void rotate_piece() {
    int temp[4][4];
    
    // Cria cópia temporária
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            temp[y][x] = current_piece[y][x];
        }
    }
    
    // Rotaciona
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            current_piece[x][3 - y] = temp[y][x];
        }
    }
    
    // Verifica se a rotação é válida
    if (check_collision()) {
        // Se não for, desfaz a rotação
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                current_piece[y][x] = temp[y][x];
            }
        }
    }
}

int check_lines() {
    int lines_cleared = 0;
    for (int y = GRID_HEIGHT - 1; y >= 0; y--) {
        bool complete = true;
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[y][x] == EMPTY) {
                complete = false;
                break;
            }
        }
        
        if (complete) {
            lines_cleared++;
            for (int ny = y; ny > 0; ny--) {
                for (int x = 0; x < GRID_WIDTH; x++) {
                    grid[ny][x] = grid[ny - 1][x];
                }
            }
            for (int x = 0; x < GRID_WIDTH; x++) {
                grid[0][x] = EMPTY;
            }
            y++; // Reverifica a mesma linha
        }
    }
    return lines_cleared;
}

int calculate_score(int lines_cleared) {
    switch (lines_cleared) {
        case 1: return SCORE_1_LINE;
        case 2: return SCORE_2_LINES;
        case 3: return SCORE_3_LINES;
        case 4: return SCORE_4_LINES;
        default: return 0;
    }
}

void draw_grid() {
    // Desenha o fundo do grid
    SDL_SetRenderDrawColor(renderer, 0x40, 0x40, 0x40, 0xFF);
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            SDL_Rect rect = {
                x * BLOCK_SIZE + 30,
                y * BLOCK_SIZE + 50,
                BLOCK_SIZE - 1,
                BLOCK_SIZE - 1
            };
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    
    // Desenha as peças fixadas
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[y][x] != EMPTY) {
                SDL_Color color;
                switch (grid[y][x]) {
                    case CYAN: color = (SDL_Color){0, 255, 255, 255}; break;
                    case BLUE: color = (SDL_Color){0, 0, 255, 255}; break;
                    case ORANGE: color = (SDL_Color){255, 127, 0, 255}; break;
                    case YELLOW: color = (SDL_Color){255, 255, 0, 255}; break;
                    case GREEN: color = (SDL_Color){0, 255, 0, 255}; break;
                    case PURPLE: color = (SDL_Color){128, 0, 128, 255}; break;
                    case RED: color = (SDL_Color){255, 0, 0, 255}; break;
                    default: color = (SDL_Color){255, 255, 255, 255};
                }
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                SDL_Rect rect = {
                    x * BLOCK_SIZE + 30,
                    y * BLOCK_SIZE + 50,
                    BLOCK_SIZE - 1,
                    BLOCK_SIZE - 1
                };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

void draw_current_piece() {
    SDL_Color color;
    switch (current_type + 1) {
        case CYAN: color = (SDL_Color){0, 255, 255, 255}; break;
        case BLUE: color = (SDL_Color){0, 0, 255, 255}; break;
        case ORANGE: color = (SDL_Color){255, 127, 0, 255}; break;
        case YELLOW: color = (SDL_Color){255, 255, 0, 255}; break;
        case GREEN: color = (SDL_Color){0, 255, 0, 255}; break;
        case PURPLE: color = (SDL_Color){128, 0, 128, 255}; break;
        case RED: color = (SDL_Color){255, 0, 0, 255}; break;
        default: color = (SDL_Color){255, 255, 255, 255};
    }
    
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (current_piece[y][x]) {
                SDL_Rect rect = {
                    (current_x + x) * BLOCK_SIZE + 30,
                    (current_y + y) * BLOCK_SIZE + 50,
                    BLOCK_SIZE - 1,
                    BLOCK_SIZE - 1
                };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

void display_game_stats() {
    int panel_x = SCREEN_WIDTH - 350;
    int panel_y = 50;
    
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color gray = {200, 200, 200, 255};
    
    // Título
    draw_text("ESTATISTICAS", panel_x, panel_y, 24, white);
    
    // Nome do jogador
    char name_text[100];
    snprintf(name_text, sizeof(name_text), "Jogador: %s", game.player_name);
    draw_text(name_text, panel_x, panel_y + 40, 20, gray);
    
    // Número do jogo
    char game_text[100];
    snprintf(game_text, sizeof(game_text), "Jogo: %d", current_player_stats.total_games + 1);
    draw_text(game_text, panel_x, panel_y + 70, 20, gray);
    
    // Linhas fechadas no jogo atual
    char lines_text[100];
    snprintf(lines_text, sizeof(lines_text), "Linhas: %d", game.current_game.lines_cleared);
    draw_text(lines_text, panel_x, panel_y + 100, 20, gray);
    
    // Blocos colocados no jogo atual
    char blocks_text[100];
    snprintf(blocks_text, sizeof(blocks_text), "Blocos: %d", game.current_game.blocks_placed);
    draw_text(blocks_text, panel_x, panel_y + 130, 20, gray);
    
    // Pontuação atual
    char score_text[100];
    snprintf(score_text, sizeof(score_text), "Pontuacao: %d", game.current_game.score);
    draw_text(score_text, panel_x, panel_y + 160, 20, gray);
    
    // Melhor pontuação
    char best_text[100];
    snprintf(best_text, sizeof(best_text), "Melhor: %d", current_player_stats.best_score);
    draw_text(best_text, panel_x, panel_y + 190, 20, gray);
    
    // Controles
    draw_text("CONTROLES", panel_x, panel_y + 240, 24, white);
    draw_text("Setas: Mover", panel_x, panel_y + 270, 18, gray);
    draw_text("Cima: Rotacionar", panel_x, panel_y + 295, 18, gray);
    draw_text("Espaco: Queda rapida", panel_x, panel_y + 320, 18, gray);
    draw_text("P: Pausar", panel_x, panel_y + 345, 18, gray);
}

void init_game() {
    srand(time(NULL));
    init_grid();
    new_piece();
    last_time = SDL_GetTicks();
}

void handle_game_events(SDL_Event* e) {
    if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
            case SDLK_LEFT:
                current_x--;
                if (check_collision()) current_x++;
                break;
            case SDLK_RIGHT:
                current_x++;
                if (check_collision()) current_x--;
                break;
            case SDLK_DOWN:
                current_y++;
                if (check_collision()) current_y--;
                break;
            case SDLK_UP:
                rotate_piece();
                break;
            case SDLK_SPACE:
                while (!check_collision()) {
                    current_y++;
                }
                current_y--;
                break;
            case SDLK_p:
                game.current_state = GAME_PAUSED;
                break;
        }
    }
}

// Modifique a função update_game:
void update_game() {
    Uint32 current_time = SDL_GetTicks();
    if (current_time - last_time > 500) {
        current_y++;
        if (check_collision()) {
            current_y--;
            int blocks_placed = merge_piece();
            game.current_game.blocks_placed += blocks_placed;
            
            int lines_cleared = check_lines();
            if (lines_cleared > 0) {
                int score = calculate_score(lines_cleared);
                game.current_game.score += score;
                game.current_game.lines_cleared += lines_cleared;
            }
            new_piece();
            if (check_collision()) {
                // Game over
                update_stats(&current_player_stats, game.current_game.score, game.current_game.blocks_placed);
                save_stats(&current_player_stats);
                game.current_state = MENU_MAIN;
                reset_current_game();
            }
        }
        last_time = current_time;
    }
}

void render_game() {
    draw_grid();
    draw_current_piece();
    display_game_stats();
}

void cleanup_game() {
    // Limpeza específica do jogo, se necessário
}

void start_game() {
    init_grid();
    new_piece();
    last_time = SDL_GetTicks();
    reset_current_game();
    
    // Carregar estatísticas do jogador se existirem
    if (!game.is_new_player) {
        load_stats(&current_player_stats, game.player_name);
    } else {
        // Novo jogador - inicializar estatísticas
        strcpy(current_player_stats.name, game.player_name);
        current_player_stats.total_games = 0;
        current_player_stats.total_lines = 0;
        current_player_stats.total_score = 0;
        current_player_stats.best_score = 0;
    }
}

void reset_current_game() {
    game.current_game.score = 0;
    game.current_game.lines_cleared = 0;
    game.current_game.level = 1;
    game.current_game.blocks_placed = 0; // Adicione esta linha
}