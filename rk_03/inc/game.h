#ifndef __GAME_H_
#define __GAME_H_

#include <stdbool.h>

struct cell
{
    bool alive;
};

struct game_field
{
    int rows;
    int cols;

    struct cell **cells;
};

struct game_field game_init(int rows, int cols);
void game_destroy(struct game_field *gf);

int game_load(struct game_field *gf, const char *filename);

bool game_update(struct game_field *gf);

void game_randomize(struct game_field *gf, int percent);

#endif
