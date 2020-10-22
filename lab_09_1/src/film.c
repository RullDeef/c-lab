#include <stdlib.h>
#include <string.h>
#include "film.h"

struct film fm_construct(char *title, char *producer, uint16_t year)
{
    struct film fm = { .title = title, .producer = producer, .year = year };
    return fm;
}

struct film fm_create(const char *title, const char *producer, uint16_t year)
{
    struct film film = { .year = year };

    film.title = (char *)malloc((strlen(title) + 1) * sizeof(char));
    if (film.title != NULL)
    {
        film.producer = (char *)malloc((strlen(producer) + 1) * sizeof(char));
        if (film.producer != NULL)
        {
            strcpy(film.title, title);
            strcpy(film.producer, producer);
        }
        else
        {
            free(film.title);
            film = fm_invalid();
        }
    }
    else
        film = fm_invalid();

    return film;
}

struct film fm_invalid(void)
{
    struct film film = { .title = NULL, .producer = NULL, .year = 0U };
    return film;
}

void fm_destroy(struct film *fm)
{
    if (fm_is_valid(fm))
    {
        free(fm->title);
        free(fm->producer);
        *fm = fm_invalid();
    }
}

bool fm_is_valid(const struct film *fm)
{
    return fm->title != NULL && fm->producer != NULL;
}

int fm_cmp_title(const struct film *fm_1, const struct film *fm_2)
{
    return strcmp(fm_1->title, fm_2->title);
}

int fm_cmp_producer(const struct film *fm_1, const struct film *fm_2)
{
    return strcmp(fm_1->producer, fm_2->producer);
}

int fm_cmp_year(const struct film *fm_1, const struct film *fm_2)
{
    return fm_1->year - fm_2->year;
}
