#include <check.h>
#include <stdlib.h>
#include "filter_func.h"


START_TEST(invalid_args)
{
    ck_assert_int_ne(key(NULL, NULL, NULL, NULL), 0);
}
END_TEST

START_TEST(begin_eq_end)
{
    int begin[] = { 0, 1, 2, 3, 4 };

    int *filtered_begin;
    int *filtered_end;

    ck_assert_int_ne(key(begin, begin, &filtered_begin, &filtered_end), 0);
}
END_TEST

START_TEST(begin_gt_end)
{
    int begin[] = { 0, 1, 2, 3, 4 };

    int *filtered_begin;
    int *filtered_end;

    ck_assert_int_ne(key(begin + 2, begin, &filtered_begin, &filtered_end), 0);
}
END_TEST

START_TEST(empty_out)
{
    int begin[] = { 5, 5, 5, 5, 5 };
    int *end = begin + sizeof(begin) / sizeof(int);

    int *filtered_begin;
    int *filtered_end;

    ck_assert_int_ne(key(begin, end, &filtered_begin, &filtered_end), 0);
}
END_TEST

START_TEST(base_case)
{
    int begin[] = { 3, 5, 7, 2, 1, 8, 3 };
    int *end = begin + sizeof(begin) / sizeof(int);

    int expected[] = { 5, 7, 8 };
    int expected_size = sizeof(expected) / sizeof(int);

    int *filtered_begin;
    int *filtered_end;

    ck_assert_int_eq(key(begin, end, &filtered_begin, &filtered_end), 0);

    // check output array size
    ck_assert_int_eq(filtered_end - filtered_begin, expected_size);

    for (int i = 0; i < expected_size; i++)
        ck_assert_int_eq(filtered_begin[i], expected[i]);
    
    // free memory
    free(filtered_begin);
}
END_TEST

void check_filter_func(TCase *tc_core)
{
    tcase_add_test(tc_core, invalid_args);
    tcase_add_test(tc_core, begin_eq_end);
    tcase_add_test(tc_core, begin_gt_end);
    tcase_add_test(tc_core, empty_out);
    tcase_add_test(tc_core, base_case);
}
