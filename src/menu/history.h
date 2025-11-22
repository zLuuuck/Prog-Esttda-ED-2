#ifndef HISTORY_H
#define HISTORY_H

#include "../game/stats.h"

#define MAX_history 10

typedef struct {
    PlayerStats history[MAX_history];
    int count;
} History;

void init_history(History * hist);
void load_history(History * hist);
void save_history(const History * hist);
void add_to_history(History * hist, const PlayerStats *stats);
void render_history(void);

#endif