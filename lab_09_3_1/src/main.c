#include "item.h"
#include "item_array.h"
#include "item_io.h"
#include "arg_reader.h"

#define	EXIT_FAILURE	1	/* Failing exit status.  */
#define	EXIT_SUCCESS	0	/* Successful exit status.  */

int main(int argc, const char **argv)
{
    const char *fname = NULL;
    float price = 0.0f;
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
