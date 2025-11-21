#ifndef RANKING_H
#define RANKING_H

#include "../game/stats.h"

#define MAX_RANKING 10

typedef struct {
    PlayerStats ranking[MAX_RANKING];
    int count;
} Ranking;

void init_ranking(Ranking *rank);
void load_ranking(Ranking *rank);
void save_ranking(const Ranking *rank);
void add_to_ranking(Ranking *rank, const PlayerStats *stats);
void render_ranking(void);

#endif