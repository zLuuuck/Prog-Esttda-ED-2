#include "history.h"
#include "../global.h"
#include "../utils/utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Botão de reset
Button reset_button;

void render_history() {
    int center_x = SCREEN_WIDTH / 2 - 200;
    int y = 100;
    
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};
    SDL_Color gray = {200, 200, 200, 255};
    SDL_Color red = {255, 100, 100, 255};

    // Título
    draw_text("HISTORICO DE PARTIDAS", center_x, y, 32, white);
    y += 50;

    // Estatísticas gerais
    char total_info[100];
    snprintf(total_info, sizeof(total_info), "Jogador: %s", current_player_stats.name);
    draw_text(total_info, center_x, y, 24, gray);
    y += 35;

    snprintf(total_info, sizeof(total_info), "Total de partidas: %d", current_player_stats.total_games);
    draw_text(total_info, center_x, y, 24, gray);
    y += 35;

    snprintf(total_info, sizeof(total_info), "Total de blocos: %d", current_player_stats.total_blocks);
    draw_text(total_info, center_x, y, 24, gray);
    y += 35;

    snprintf(total_info, sizeof(total_info), "Melhor pontuacao: %d", current_player_stats.best_score);
    draw_text(total_info, center_x, y, 24, gray);
    y += 50;

    // Cabeçalho da tabela
    if (current_player_stats.total_games > 0) {
        draw_text("Partida  Blocos  Pontuacao", center_x, y, 20, yellow);
        y += 30;

        // Ordena as partidas por pontuação (decrescente)
        GameRecord sorted_games[MAX_GAMES];
        int game_count = current_player_stats.total_games > MAX_GAMES ? MAX_GAMES : current_player_stats.total_games;
        
        // Copia os jogos para array temporário
        for (int i = 0; i < game_count; i++) {
            sorted_games[i] = current_player_stats.games[i];
        }

        // Ordenação por pontuação (bubble sort simples)
        for (int i = 0; i < game_count - 1; i++) {
            for (int j = 0; j < game_count - i - 1; j++) {
                if (sorted_games[j].score < sorted_games[j + 1].score) {
                    GameRecord temp = sorted_games[j];
                    sorted_games[j] = sorted_games[j + 1];
                    sorted_games[j + 1] = temp;
                }
            }
        }

        // Mostra as partidas (máximo 15 para caber na tela)
        int max_display = game_count > 15 ? 15 : game_count;
        for (int i = 0; i < max_display; i++) {
            char line[100];
            snprintf(line, sizeof(line), "  %3d      %3d      %5d", 
                     sorted_games[i].game_number, 
                     sorted_games[i].blocks_placed, 
                     sorted_games[i].score);
            draw_text(line, center_x, y, 20, white);
            y += 25;
        }
    } else {
        draw_text("Nenhuma partida registrada", center_x, y, 24, gray);
        y += 40;
    }

    // Botão de reset
    reset_button.rect = (SDL_Rect){center_x, y + 20, 200, 50};
    reset_button.text = "Resetar Dados";
    reset_button.hovered = false;
    draw_button(&reset_button);

    draw_text("Pressione ESC para voltar", center_x, y + 80, 20, white);
}

void handle_history_events(SDL_Event* e) {
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    reset_button.hovered = is_point_in_rect(mouse_x, mouse_y, reset_button.rect);

    if (e->type == SDL_MOUSEBUTTONDOWN) {
        if (reset_button.hovered) {
            // Reseta os dados do jogador
            reset_player_data();
            
            // Reseta as estatísticas atuais
            memset(&current_player_stats, 0, sizeof(PlayerStats));
            strcpy(current_player_stats.name, "Jogador");
            
            // Reseta o estado do jogo
            game.player_name[0] = '\0';
            game.is_new_player = true;
            
            // Volta ao menu principal
            game.current_state = MENU_MAIN;
        }
    }

    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_ESCAPE) {
        game.current_state = MENU_MAIN;
    }
}