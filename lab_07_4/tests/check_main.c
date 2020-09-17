#include <check.h>

void check_filter_func(TCase *tc_core);
void check_sort_func(TCase *tc_core);

Suite *app_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("app suite");
    tc_core = tcase_create("core test case");

    check_filter_func(tc_core);
    check_sort_func(tc_core);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    SRunner *runner;
    Suite *s = app_suite();

    runner = srunner_create(s);

    srunner_run_all(runner, CK_NORMAL);
    int num_failed = srunner_ntests_failed(runner);

    srunner_free(runner);
    return num_failed;
}
