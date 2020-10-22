#ifndef __READER_H_
#define __READER_H_

#include <stdio.h>
#include "film.h"
#include "film_array.h"

struct film fmrd_read_film(FILE *file);
struct film_array fmrd_read_film_array(FILE *file, film_cmp_t fm_cmp);

#endif
