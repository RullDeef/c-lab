#include <check.h>

Suite *num_dcmp_suite(void);
Suite *commander_suite(void);

int main(void)
{
    SRunner *runner = srunner_create(num_dcmp_suite());
    srunner_add_suite(runner, commander_suite());

    srunner_run_all(runner, CK_NORMAL);

    int failed = srunner_ntests_failed(runner);

    srunner_free(runner);
    return failed;
}
