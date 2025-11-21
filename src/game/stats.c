#include "stats.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

void load_stats(PlayerStats *stats, const char *player_name) {
    strcpy(stats->name, player_name);
    stats->games_played = 0;
    stats->total_lines = 0;
    stats->total_score = 0;
    stats->best_score = 0;

    char filename[100];
    snprintf(filename, sizeof(filename), "%s%s.txt", STATS_FOLDER, player_name);

    FILE *file = fopen(filename, "r");
    if (file) {
        fscanf(file, "Jogos: %d\n", &stats->games_played);
        fscanf(file, "Linhas: %d\n", &stats->total_lines);
        fscanf(file, "Pontuacao Total: %d\n", &stats->total_score);
        fscanf(file, "Melhor Pontuacao: %d\n", &stats->best_score);
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

    char filename[100];
    snprintf(filename, sizeof(filename), "%s%s.txt", STATS_FOLDER, stats->name);

    FILE *file = fopen(filename, "w");
    if (file) {
        fprintf(file, "Jogos: %d\n", stats->games_played);
        fprintf(file, "Linhas: %d\n", stats->total_lines);
        fprintf(file, "Pontuacao Total: %d\n", stats->total_score);
        fprintf(file, "Melhor Pontuacao: %d\n", stats->best_score);
        fclose(file);
    }
}

void update_stats(PlayerStats *stats, int score, int lines) {
    stats->games_played++;
    stats->total_lines += lines;
    stats->total_score += score;
    if (score > stats->best_score) {
        stats->best_score = score;
    }
}

bool player_file_exists(const char *player_name) {
    char filename[100];
    snprintf(filename, sizeof(filename), "%s%s.txt", STATS_FOLDER, player_name);
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}