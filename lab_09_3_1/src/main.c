#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "item.h"
#include "item_array.h"
#include "item_io.h"

int read_args(int argc, char **argv, char **fname, float *price)
{
    int status = EXIT_FAILURE;

    if (argc == 3)
    {
        *fname = argv[1];
        if (sscanf(argv[2], "%f", price) == 1)
            status = EXIT_SUCCESS;
    }

    return status;
}

int main(int argc, char **argv)
{
    char *fname = NULL;
    float price = 0.0f;;
    int status;

    status = read_args(argc, argv, &fname, &price);
    if (status == EXIT_SUCCESS)
    {
        struct item_array ita = it_read_array(fname);
        if (ita_is_valid(&ita))
        {
            it_print_array_file_constraint(stdout, &ita, price);
            ita_destroy(&ita);
        }
        else
            status = EXIT_FAILURE;
    }

    return status;
}
