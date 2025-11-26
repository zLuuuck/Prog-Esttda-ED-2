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

void load_stats(PlayerStats *stats, const char *player_name)
{
    strcpy(stats->name, player_name);
    stats->total_games = 0;
    stats->total_blocks = 0;
    stats->total_score = 0;
    stats->best_score = 0;

    char filename[100];
    snprintf(filename, sizeof(filename), "%splay_history_%s.dat", STATS_FOLDER, player_name);

    FILE *file = fopen(filename, "rb");
    if (file)
    {
        fread(stats, sizeof(PlayerStats), 1, file);
        fclose(file);
    }
}

void save_stats(const PlayerStats *stats)
{
// Cria a pasta stats se não existir
#ifdef _WIN32
    system("mkdir stats 2>nul");
#else
    system("mkdir -p stats");
#endif

    char filename[100];
    snprintf(filename, sizeof(filename), "%splay_history_%s.dat", STATS_FOLDER, stats->name);

    FILE *file = fopen(filename, "wb");
    if (file)
    {
        fwrite(stats, sizeof(PlayerStats), 1, file);
        fclose(file);
    }
}

void update_stats(PlayerStats *stats, int score, int blocks)
{
    stats->total_games++;
    stats->total_blocks += blocks;
    stats->total_score += score;

    if (score > stats->best_score)
    {
        stats->best_score = score;
    }

    // Adiciona a partida ao histórico
    if (stats->total_games <= MAX_GAMES)
    {
        int index = stats->total_games - 1;
        stats->games[index].game_number = stats->total_games;
        stats->games[index].blocks_placed = blocks;
        stats->games[index].score = score;
    }
}

bool player_file_exists(const char *player_name)
{
    char filename[100];
    snprintf(filename, sizeof(filename), "%splay_history_%s.dat", STATS_FOLDER, player_name);
    FILE *file = fopen(filename, "rb");
    if (file)
    {
        fclose(file);
        return true;
    }
    return false;
}

void save_last_player(const char *player_name)
{
#ifdef _WIN32
    system("mkdir stats 2>nul");
#else
    system("mkdir -p stats");
#endif

    FILE *file = fopen("stats/last_player.dat", "wb");
    if (file)
    {
        fwrite(player_name, sizeof(char), MAX_NAME_LENGTH, file);
        fclose(file);
    }
}

void load_last_player(char *player_name)
{
    FILE *file = fopen("stats/last_player.dat", "rb");
    if (file)
    {
        fread(player_name, sizeof(char), MAX_NAME_LENGTH, file);
        fclose(file);
    }
    else
    {
        player_name[0] = '\0';
    }
}