#include <check.h>
#include "arg_reader.h"

#define	EXIT_FAILURE	1	/* Failing exit status.  */
#define	EXIT_SUCCESS	0	/* Successful exit status.  */

START_TEST(arg_reader_wrong_argc)
{
    const char *fname;
    float price;

    const char *argv_1[] = { "app.exe", "data.txt" };
    const char *argv_2[] = { "app.exe", "data.txt", "name", "4.13" };

    int status = read_args(2, argv_1, &fname, &price);
    ck_assert_int_eq(status, EXIT_FAILURE);

    status = read_args(4, argv_2, &fname, &price);
    ck_assert_int_eq(status, EXIT_FAILURE);
}
END_TEST

START_TEST(arg_reader_wrong_price)
{
    const char *fname;
    float price;

    const char *argv[] = { "app.exe", "data.txt", "three" };

    int status = read_args(3, argv, &fname, &price);
    ck_assert_int_eq(status, EXIT_FAILURE);
}
END_TEST

START_TEST(arg_reader_ok)
{
    const char *fname;
    float price;

    const char *argv[] = { "app.exe", "data.txt", "3.0e+2" };

    int status = read_args(3, argv, &fname, &price);
    ck_assert_int_eq(status, EXIT_SUCCESS);
    ck_assert_str_eq(fname, "data.txt");
    ck_assert_float_eq(price, 300.0f);
}
END_TEST

Suite *arg_reader_suite(void)
{
    Suite *suite = suite_create("arg reader suite");
    TCase *tcase = tcase_create("core");

    tcase_add_test(tcase, arg_reader_wrong_argc);
    tcase_add_test(tcase, arg_reader_wrong_price);
    tcase_add_test(tcase, arg_reader_ok);

    suite_add_tcase(suite, tcase);
    return suite;
}
