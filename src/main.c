#include "global.h"
#include "game/game.h"
#include "menu/menu.h"
#include "exit/exit.h"
#include "utils/utils.h"
#include "game/stats.h"

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Tetris dos Guri",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    // Inicializações
    init_fonts();
    init_game();
    init_menu();
    
    // Carrega as estatísticas automaticamente
    load_stats(&current_player_stats);
    
    // Se já existem dados salvos, usa o nome do jogador salvo
    if (strlen(current_player_stats.name) > 0) {
        strcpy(game.player_name, current_player_stats.name);
        game.is_new_player = false;
    }
    
    // Loop principal
    while (game.running) {
        handle_events();
        update();
        render();
        SDL_Delay(16);
    }
    
    // Cleanup
    cleanup_game();
    cleanup_menu();
    cleanup_fonts();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}