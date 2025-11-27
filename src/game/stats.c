#include "stats.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

void load_stats(PlayerStats *stats) {
    // Inicializa com valores padrão
    strcpy(stats->name, "Jogador");
    stats->total_games = 0;
    stats->total_blocks = 0;
    stats->total_score = 0;
    stats->best_score = 0;

    FILE *file = fopen(PLAYER_DATA_FILE, "rb");
    if (file) {
        fread(stats, sizeof(PlayerStats), 1, file);
        fclose(file);
    }
}

void save_stats(const PlayerStats *stats) {
    // Cria a pasta stats se não existir
    #ifdef _WIN32
    system("mkdir stats 2>nul");
    #else
    system("mkdir -p stats");
    #endif

    FILE *file = fopen(PLAYER_DATA_FILE, "wb");
    if (file) {
        fwrite(stats, sizeof(PlayerStats), 1, file);
        fclose(file);
    }
}

void update_stats(PlayerStats *stats, int score, int blocks) {
    stats->total_games++;
    stats->total_blocks += blocks;
    stats->total_score += score;
    
    if (score > stats->best_score) {
        stats->best_score = score;
    }

    // Adiciona a partida ao histórico
    if (stats->total_games <= MAX_GAMES) {
        int index = stats->total_games - 1;
        stats->games[index].game_number = stats->total_games;
        stats->games[index].blocks_placed = blocks;
        stats->games[index].score = score;
    }
}

bool player_data_exists(void) {
    FILE *file = fopen(PLAYER_DATA_FILE, "rb");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

void reset_player_data(void) {
    remove(PLAYER_DATA_FILE);
}