#include "global.h"  // Mude de main.h para global.h
#include "game/game.h"
#include "menu/menu.h"
#include "exit/exit.h"
#include "utils/utils.h"
#include "game/stats.h"

// REMOVA as definições de game e renderer daqui - elas estão em global.c agora

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
    
    // Carrega o último jogador
    load_last_player(game.player_name);
    if (strlen(game.player_name) > 0) {
        game.is_new_player = false;
        // Carrega as estatísticas do jogador
        load_stats(&current_player_stats, game.player_name);
    }
    
    // Verificar se existem jogadores
    check_existing_players();
    
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