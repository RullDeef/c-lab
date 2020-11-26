#include <check.h>
#include "strlib.h"

START_TEST(test_decimal)
{
    char buf[100] = "";
    int write;

    write = my_snprintf(NULL, 0, "%d", 100);
    ck_assert_int_eq(write, 3);
    ck_assert_str_eq(buf, "");

    write = my_snprintf(buf, 0, "%d", 100);
    ck_assert_int_eq(write, 3);
    ck_assert_str_eq(buf, "");

    write = my_snprintf(buf, 3, "%d", 100);
    ck_assert_int_eq(write, 3);
    ck_assert_str_eq(buf, "10");

    write = my_snprintf(buf, 100, "%d", 100);
    ck_assert_int_eq(write, 3);
    ck_assert_str_eq(buf, "100");

    write = my_snprintf(buf, 2, "%d", -18);
    ck_assert_int_eq(write, 3);
    ck_assert_str_eq(buf, "-");
}
END_TEST

void add_decimal_tests(Suite *suite)
{
    TCase *tcase = tcase_create("decimal");

    tcase_add_test(tcase, test_decimal);

    suite_add_tcase(suite, tcase);
}
