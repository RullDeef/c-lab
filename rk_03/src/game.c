#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "game.h"

static size_t calc_alloc_size(int rows, int cols)
{
    return rows * sizeof(struct cell *) + rows * cols * sizeof(struct cell);
}

struct game_field game_init(int rows, int cols)
{
    struct game_field gf = { .rows = rows, .cols = cols };
    gf.cells = calloc(1, calc_alloc_size(rows, cols));

    if (gf.cells != NULL)
        for (int row = 0; row < rows; row++)
            gf.cells[row] = (struct cell*)(gf.cells + rows) + cols * row;

    return gf;
}

void game_destroy(struct game_field *gf)
{
    free(gf->cells);
    gf->cells = NULL;
}

static int count_neighbors(struct game_field *gf, int row, int col)
{
    int neighbors = 0;

    for (int dy = -1; dy < 2; dy++)
    {
        for (int dx = -1; dx < 2; dx++)
        {
            if (dx == 0 && dy == 0)
                continue;

            int x = (col + dx + gf->cols) % gf->cols;
            int y = (row + dy + gf->rows) % gf->rows;

            if (gf->cells[y][x].alive)
                neighbors++;
        }
    }

    return neighbors;
}

static bool check_file_dims(FILE *file, int *rows, int *cols)
{
    bool valid = true;
    char *line = NULL;
    size_t size = 0U;

    *cols = -1;
    *rows = 0;

    while (!feof(file) && getline(&line, &size, file) != EOF)
    {
        if (*cols == -1)
            *cols = strlen(line);
        else if (*cols != strlen(line))
        {
            valid = false;
            break;
        }
        (*rows)++;
    }

    free(line);
    return valid;
}

int game_load(struct game_field *gf, const char *filename)
{

    FILE *file = fopen(filename, "rt");
    int status = EXIT_FAILURE;

    if (file)
    {
        int rows, cols;
        if (check_file_dims(file, &rows, &cols))
        {
            rewind(file);

            *gf = game_init(rows, cols);

            char *line = NULL;
            size_t size = 0U;
            for (int row = 0; row < rows; row++)
            {
                getline(&line, &size, file);
                for (int col = 0; col < cols; col++)
                    gf->cells[row][col].alive = line[col] == '1';
            }

            free(line);
            status = EXIT_SUCCESS;
        }

        fclose(file);
    }

    return status;
}

bool game_update(struct game_field *gf)
{
    bool end = true;

    struct cell **next_generation = calloc(1, calc_alloc_size(gf->rows, gf->cols));
    if (next_generation != NULL)
        for (int row = 0; row < gf->rows; row++)
            next_generation[row] = (struct cell *)(next_generation + gf->rows) + gf->cols * row;

    for (int col = 0; col < gf->cols; col++)
    {
        for (int row = 0; row < gf->rows; row++)
        {
            struct cell *cell = gf->cells[row] + col;
            int neighbors = count_neighbors(gf, row, col);

            if ((!cell->alive && neighbors == 3) ||
                (cell->alive && (neighbors == 2 || neighbors == 3)))
            {
                next_generation[row][col].alive = true;
                end = false;
            }
        }
    }

    free(gf->cells);
    gf->cells = next_generation;
    return end;
}

void game_randomize(struct game_field *gf, int percent)
{
    for (int col = 0; col < gf->cols; col++)
        for (int row = 0; row < gf->rows; row++)
            gf->cells[row][col].alive = rand() % 100 < percent;
}
