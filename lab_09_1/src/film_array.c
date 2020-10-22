#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "film.h"
#include "film_array.h"

struct film_array fma_create(void)
{
    struct film_array fma = { .size = 0U };
    fma.data = (struct film *)calloc(FMA_INIT_CAPACITY, sizeof(struct film));
    fma.capacity = fma.data == NULL ? 0U : FMA_INIT_CAPACITY;
    return fma;
}

struct film_array fma_invalid(void)
{
    struct film_array fma = { .size = 0U, .capacity = 0U, .data = NULL };
    return fma;
}

void fma_destroy(struct film_array *fma)
{
    for (uint32_t i = 0; i < fma->size; i++)
        fm_destroy(fma->data + i);

    free(fma->data);
    *fma = fma_invalid();
}

bool fma_is_valid(const struct film_array *fma)
{
    return fma->data != NULL;
}

int fma_resize(struct film_array *fma, uint32_t size)
{
    int status = EXIT_SUCCESS;

    if (fma->capacity < size)
    {
        uint32_t new_capacity = size * FMA_ALLOC_MULTIPLIER;
        struct film *new_data = (struct film *)realloc(fma->data, new_capacity * sizeof(struct film));
        if (new_data != NULL)
        {
            fma->capacity = new_capacity;
            fma->data = new_data;
        }
        else
            status = EXIT_FAILURE;
    }

    return status;
}

int fma_insert_sorted(struct film_array *fma, struct film *fm, film_cmp_t fm_cmp)
{
    int status = fma_resize(fma, fma->size + 1);

    if (status == EXIT_SUCCESS)
    {
        fma->size += 1;

        bool shift = false;
        for (uint32_t i = 0; i < fma->size; i++)
        {
            shift = shift || i + 1 == fma->size || fm_cmp(fma->data + i, fm) > 0;
            if (shift)
            {
                struct film tmp = *fm;
                *fm = fma->data[i];
                fma->data[i] = tmp;
            }
        }
    }

    return status;
}

int get_fm_cmp(const char *field, film_cmp_t *fm_cmp)
{
    int status = EXIT_SUCCESS;

    if (strcmp(field, "title") == 0)
        *fm_cmp = fm_cmp_title;
    else if (strcmp(field, "name") == 0)
        *fm_cmp = fm_cmp_producer;
    else if (strcmp(field, "year") == 0)
        *fm_cmp = fm_cmp_year;
    else
        status = EXIT_FAILURE;

    return status;
}

/*
struct film fma_find(struct film_array *fma, film_fd_t fm_fd, const void *key)
{
    struct film fm = fm_invalid();

    uint32_t left = 0;
    uint32_t right = fma->size - 1;

    while (left < right)
    {
        uint32_t mid = (left + right) / 2;
        int fd = fm_fd(fma->data + mid, key);
        if (fd == 0)
        {
            fm = fma->data[mid];
            break;
        }
        else if (fd < 0)
            left = mid;
        else
            right = mid - 1;
    }

    return fm;
}
*/
