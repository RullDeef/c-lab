#ifndef __FILM_H_
#define __FILM_H_

#include <stdint.h>
#include <stdbool.h>

struct film
{
    char *title;
    char *producer;
    uint16_t year;
};

typedef int (*film_cmp_t)(const struct film *, const struct film *);

struct film fm_construct(char *title, char *producer, uint16_t year);
struct film fm_create(const char *title, const char *producer, uint16_t year);
struct film fm_invalid(void);

void fm_destroy(struct film *fm);

bool fm_is_valid(const struct film *fm);

// функции для сравнения
int fm_cmp_title(const struct film *fm_1, const struct film *fm_2);
int fm_cmp_producer(const struct film *fm_1, const struct film *fm_2);
int fm_cmp_year(const struct film *fm_1, const struct film *fm_2);

#endif
