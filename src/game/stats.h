#ifndef STATS_H
#define STATS_H

#include "../main.h"

#define MAX_NAME_LENGTH 50
#define STATS_FOLDER "stats/"

typedef struct {
    char name[MAX_NAME_LENGTH];
    int games_played;
    int total_lines;
    int total_score;
    int best_score;
} PlayerStats;

// Funções para gerenciar estatísticas
void load_stats(PlayerStats *stats, const char *player_name);
void save_stats(const PlayerStats *stats);
void update_stats(PlayerStats *stats, int score, int lines);
bool player_file_exists(const char *player_name);

#endif