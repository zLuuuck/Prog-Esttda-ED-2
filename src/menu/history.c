#include "history.h"
#include "../utils/utils.h"
#include <stdio.h>
#include <string.h>

void init_history(History *hist) {
    hist->count = 0;
}

void load_history(History *hist) {
    // Implementação simplificada - poderia carregar de um arquivo
    hist->count = 0;
}

void save_history(const History *hist) {
    // Implementação simplificada - poderia salvar em um arquivo
    (void)hist; // Silencia warning de parâmetro não usado
}

void add_to_history(History *hist, const PlayerStats *stats) {
    // Implementação simplificada
    (void)hist;  // Silencia warning de parâmetro não usado
    (void)stats; // Silencia warning de parâmetro não usado
}

void render_history() {
    int center_x = SCREEN_WIDTH / 2 - 150;
    
    draw_text("history - TOP 10", center_x, 100, 32, (SDL_Color){255, 255, 255, 255});
    draw_text("EM DESENVOLVIMENTO", center_x, 200, 24, (SDL_Color){200, 200, 200, 255});
    draw_text("Pressione ESC para voltar", center_x, 300, 20, (SDL_Color){255, 255, 255, 255});
}