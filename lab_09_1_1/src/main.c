#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "film.h"
#include "film_array.h"
#include "reader.h"
#include "printer.h"

int read_args(int argc, char **argv, char **fname, char **field, char **key)
{
    int status = EXIT_FAILURE;

    if (argc == 3 || argc == 4)
    {
        *fname = argv[1];
        *field = argv[2];
        *key = argc == 4 ? argv[3] : NULL;
        status = EXIT_SUCCESS;
    }

    return status;
}

int main(int argc, char **argv)
{
    char *fname = NULL;
    char *field = NULL;
    char *key = NULL;
    int status;
    
    film_cmp_t fm_cmp;

    status = read_args(argc, argv, &fname, &field, &key) || get_fm_cmp(field, &fm_cmp);
    if (status == EXIT_SUCCESS)
    {
        FILE *file = fopen(fname, "rt");
        if (file != NULL)
        {
            struct film_array fma = fmrd_read_film_array(file, fm_cmp);
            if (fma_is_valid(&fma))
            {
                if (key == NULL)
                    fmpr_print_film_array(stdout, &fma);
                else
                {
                    // find here
                }
                fma_destroy(&fma);
            }
            else
                status = EXIT_FAILURE;
            fclose(file);
        }
    }

    return status;
}
