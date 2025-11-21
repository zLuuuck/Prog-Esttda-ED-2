#include "ranking.h"
#include "../utils/utils.h"
#include <stdio.h>
#include <string.h>

void init_ranking(Ranking *rank) {
    rank->count = 0;
}

void load_ranking(Ranking *rank) {
    // Implementação simplificada - poderia carregar de um arquivo
    rank->count = 0;
}

void save_ranking(const Ranking *rank) {
    // Implementação simplificada - poderia salvar em um arquivo
    (void)rank; // Silencia warning de parâmetro não usado
}

void add_to_ranking(Ranking *rank, const PlayerStats *stats) {
    // Implementação simplificada
    (void)rank;  // Silencia warning de parâmetro não usado
    (void)stats; // Silencia warning de parâmetro não usado
}

void render_ranking() {
    int center_x = SCREEN_WIDTH / 2 - 150;
    
    draw_text("RANKING - TOP 10", center_x, 100, 32, (SDL_Color){255, 255, 255, 255});
    draw_text("EM DESENVOLVIMENTO", center_x, 200, 24, (SDL_Color){200, 200, 200, 255});
    draw_text("Pressione ESC para voltar", center_x, 300, 20, (SDL_Color){255, 255, 255, 255});
}