#include <stdio.h>
#include <stdint.h>
#include "printer.h"

void fmpr_print_film(FILE *file, const struct film *fm)
{
    fprintf(file, "%s\n%s\n%hu\n", fm->title, fm->producer, fm->year);
}

void fmpr_print_film_array(FILE *file, const struct film_array *fma)
{
    for (uint32_t i = 0; i < fma->size; i++)
        fmpr_print_film(file, fma->data + i);
}
