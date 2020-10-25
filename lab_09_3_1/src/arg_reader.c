#include <stdio.h>
#include "arg_reader.h"

#define	EXIT_FAILURE	1	/* Failing exit status.  */
#define	EXIT_SUCCESS	0	/* Successful exit status.  */

int read_args(int argc, const char **argv, const char **fname, float *price)
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
