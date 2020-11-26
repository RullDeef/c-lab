#include <check.h>

void add_decimal_tests(Suite *suite);
void add_str_tests(Suite *suite);

int main(void)
{
    Suite *suite = suite_create("my_snprintf");

    add_decimal_tests(suite);
    add_str_tests(suite);

    SRunner *runner = srunner_create(suite);

    srunner_run_all(runner, CK_VERBOSE);

    int failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return failed;
}
