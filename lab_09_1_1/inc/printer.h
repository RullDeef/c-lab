#ifndef __PRINTER_H_
#define __PRINTER_H_

#include <stdio.h>
#include "film.h"
#include "film_array.h"

void fmpr_print_film(FILE *file, const struct film *fm);
void fmpr_print_film_array(FILE *file, const struct film_array *fma);

#endif
