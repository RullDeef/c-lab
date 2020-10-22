#ifndef __FILM_ARRAY_H_
#define __FILM_ARRAY_H_

#include <stdbool.h>
#include "film.h"

#define FMA_INIT_CAPACITY 10U
#define FMA_ALLOC_MULTIPLIER 1.5f

struct film_array
{
    uint32_t size;
    uint32_t capacity;
    struct film *data;
};

struct film_array fma_create(void);
struct film_array fma_invalid(void);

void fma_destroy(struct film_array *fma);

bool fma_is_valid(const struct film_array *fma);

int fma_resize(struct film_array *fma, uint32_t size);

// вставляет новый фильм в массив, с сохранением порядка
int fma_insert_sorted(struct film_array *fma, struct film *fm, film_cmp_t fm_cmp);

int get_fm_cmp(const char *field, film_cmp_t *fm_cmp);

#endif
