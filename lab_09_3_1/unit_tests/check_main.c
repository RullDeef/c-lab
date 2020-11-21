#include <check.h>

#define	EXIT_FAILURE	1	/* Failing exit status.  */
#define	EXIT_SUCCESS	0	/* Successful exit status.  */

Suite *item_suite(void);
Suite *item_array_suite(void);
Suite *arg_reader_suite(void);

int main(void)
{
    SRunner *runner = srunner_create(NULL);

    srunner_add_suite(runner, item_suite());
    srunner_add_suite(runner, item_array_suite());
    srunner_add_suite(runner, arg_reader_suite());

    srunner_run_all(runner, CK_NORMAL);
    unsigned int n_failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return n_failed == 0U ? EXIT_SUCCESS : EXIT_FAILURE;
}
