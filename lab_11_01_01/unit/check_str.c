#include <check.h>
#include "strlib.h"

START_TEST(test_str)
{
    char buf[100] = "";
    int write;

    write = my_snprintf(NULL, 0, "%s", "hello");
    ck_assert_int_eq(write, 5);
    ck_assert_str_eq(buf, "");

    write = my_snprintf(buf, 0, "%s", "hello");
    ck_assert_int_eq(write, 5);
    ck_assert_str_eq(buf, "");

    write = my_snprintf(buf, 3, "%s", "hello");
    ck_assert_int_eq(write, 5);
    ck_assert_str_eq(buf, "he");

    write = my_snprintf(buf, 100, "%s", "hello");
    ck_assert_int_eq(write, 5);
    ck_assert_str_eq(buf, "hello");

    write = my_snprintf(buf, 100, "%s", "");
    ck_assert_int_eq(write, 0);
    ck_assert_str_eq(buf, "");
}
END_TEST

void add_str_tests(Suite *suite)
{
    TCase *tcase = tcase_create("str");

    tcase_add_test(tcase, test_str);

    suite_add_tcase(suite, tcase);
}
