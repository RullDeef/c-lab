#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "film.h"
#include "film_array.h"
#include "reader.h"

#define MAX_YEAR_STR_LEN 128

int imp__read_year(FILE *file, uint16_t *year)
{
    char tmp[MAX_YEAR_STR_LEN];
    int status = EXIT_FAILURE;
    int count;

    if (fgets(tmp, MAX_YEAR_STR_LEN, file) != NULL &&
            sscanf(tmp, "%hu %n", year, &count) == 1 &&
            (size_t)count == strlen(tmp))
    {
        status = EXIT_SUCCESS;

        // mark eof
        int c;
        if ((c = fgetc(file)) != EOF)
            ungetc(c, file);
    }

    return status;
}

struct film fmrd_read_film(FILE *file)
{
    char *title = NULL;
    char *producer = NULL;
    uint16_t year;
    size_t n = 0;
    ssize_t len;

    if ((len = getline(&title, &n, file)) != -1)
    {
        while (len > 0 && isspace((int)title[len - 1]))
            title[len - 1] = '\0';

        if (len > 0 && (len = getline(&producer, &n, file)) != -1)
        {
            while (len > 0 && isspace((int)producer[len - 1]))
                producer[len - 1] = '\0';

            if (len == 0 || imp__read_year(file, &year) != EXIT_SUCCESS)
            {
                free(title);
                free(producer);
                title = NULL;
                producer = NULL;
            }
        }
        else
        {
            free(title);
            title = NULL;
        }
    }

    return fm_construct(title, producer, year);
}

struct film_array fmrd_read_film_array(FILE *file, film_cmp_t fm_cmp)
{
    struct film_array fma = fma_create();

    if (fma_is_valid(&fma))
    {
        struct film fm;

        while (!feof(file))
        {
            fm = fmrd_read_film(file);
            if (fm_is_valid(&fm))
                fma_insert_sorted(&fma, &fm, fm_cmp);
            else
            {
                fma_destroy(&fma);
                break;
            }
        }
    }

    return fma;
}
