#ifndef STATS_H
#define STATS_H

#include "../main.h"

#define MAX_NAME_LENGTH 50
#define STATS_FOLDER "stats/"
#define MAX_GAMES 100

// Estrutura para uma partida individual
typedef struct {
    int game_number;
    int blocks_placed;
    int score;
} GameRecord;

// Estrutura principal do jogador
typedef struct {
    char name[MAX_NAME_LENGTH];
    int total_games;
    int total_blocks;
    int total_lines;
    int total_score;
    int best_score;
    GameRecord games[MAX_GAMES]; // Histórico de partidas
} PlayerStats;

// Funções para gerenciar estatísticas
void load_stats(PlayerStats *stats, const char *player_name);
void save_stats(const PlayerStats *stats);
void update_stats(PlayerStats *stats, int score, int blocks);
bool player_file_exists(const char *player_name);
void save_last_player(const char *player_name);
void load_last_player(char *player_name);

#endif