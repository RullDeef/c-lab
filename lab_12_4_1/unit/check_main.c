#include <check.h>

Suite *array_suite(void);

int main(void)
{
    Suite *suite = array_suite();
    SRunner *runner = srunner_create(suite);

    srunner_run_all(runner, CK_VERBOSE);
    int failed = srunner_ntests_failed(runner);

    srunner_free(runner);
    return failed;
}
