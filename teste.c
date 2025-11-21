#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>

// Configurações da janela
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 729
#define BLOCK_SIZE 30
#define GRID_WIDTH 10
#define GRID_HEIGHT 20

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

// Matriz do grid do jogo
BlockType grid[GRID_HEIGHT][GRID_WIDTH];

// Peças do Tetris (formato I, J, L, O, S, T, Z)
const int tetrominos[7][4][4] = {
    // I
    {{0,0,0,0},
     {1,1,1,1},
     {0,0,0,0},
     {0,0,0,0}},
    // J
    {{2,0,0,0},
     {2,2,2,0},
     {0,0,0,0},
     {0,0,0,0}},
    // L
    {{0,0,3,0},
     {3,3,3,0},
     {0,0,0,0},
     {0,0,0,0}},
    // O
    {{0,4,4,0},
     {0,4,4,0},
     {0,0,0,0},
     {0,0,0,0}},
    // S
    {{0,5,5,0},
     {5,5,0,0},
     {0,0,0,0},
     {0,0,0,0}},
    // T
    {{0,6,0,0},
     {6,6,6,0},
     {0,0,0,0},
     {0,0,0,0}},
    // Z
    {{7,7,0,0},
     {0,7,7,0},
     {0,0,0,0},
     {0,0,0,0}}
};

// Variáveis da peça atual
int current_piece[4][4];
int current_x, current_y;
int current_type;

// Função para inicializar o grid
void init_grid() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            grid[y][x] = EMPTY;
        }
    }
}

// Função para gerar uma nova peça
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

// Função para verificar colisões
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

// Função para fundir a peça ao grid
void merge_piece() {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (current_piece[y][x]) {
                grid[current_y + y][current_x + x] = current_piece[y][x];
            }
        }
    }
}

// Função para rotacionar a peça
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

// Função para verificar linhas completas
void check_lines() {
    for (int y = GRID_HEIGHT - 1; y >= 0; y--) {
        bool complete = true;
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[y][x] == EMPTY) {
                complete = false;
                break;
            }
        }
        
        if (complete) {
            // Move todas as linhas para baixo
            for (int ny = y; ny > 0; ny--) {
                for (int x = 0; x < GRID_WIDTH; x++) {
                    grid[ny][x] = grid[ny - 1][x];
                }
            }
            // Limpa a linha do topo
            for (int x = 0; x < GRID_WIDTH; x++) {
                grid[0][x] = EMPTY;
            }
            y++; // Reverifica a mesma linha
        }
    }
}

// Função para desenhar o grid
void draw_grid(SDL_Renderer* renderer) {
    // Desenha o fundo do grid
    SDL_SetRenderDrawColor(renderer, 0x40, 0x40, 0x40, 0xFF);
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            SDL_Rect rect = {
                x * BLOCK_SIZE + 50,
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
                    x * BLOCK_SIZE + 50,
                    y * BLOCK_SIZE + 50,
                    BLOCK_SIZE - 1,
                    BLOCK_SIZE - 1
                };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

// Função para desenhar a peça atual
void draw_current_piece(SDL_Renderer* renderer) {
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
                    (current_x + x) * BLOCK_SIZE + 50,
                    (current_y + y) * BLOCK_SIZE + 50,
                    BLOCK_SIZE - 1,
                    BLOCK_SIZE - 1
                };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Tetris",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    srand(time(NULL));
    init_grid();
    new_piece();
    
    Uint32 last_time = SDL_GetTicks();
    bool running = true;
    
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
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
                        // Queda rápida
                        while (!check_collision()) {
                            current_y++;
                        }
                        current_y--;
                        break;
                }
            }
        }
        
        // Movimento automático (queda)
        Uint32 current_time = SDL_GetTicks();
        if (current_time - last_time > 500) { // A cada 500ms
            current_y++;
            if (check_collision()) {
                current_y--;
                merge_piece();
                check_lines();
                new_piece();
                if (check_collision()) {
                    // Game over
                    running = false;
                }
            }
            last_time = current_time;
        }
        
        // Renderização
        SDL_SetRenderDrawColor(renderer, 0x1E, 0x1E, 0x1E, 0xFF);
        SDL_RenderClear(renderer);
        
        draw_grid(renderer);
        draw_current_piece(renderer);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}