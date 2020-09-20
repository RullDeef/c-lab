#include <check.h>
#include "sort_func.h"

START_TEST(single_element)
{
    int array[] = { 24 };

    ck_assert_int_eq(mysort(array, 1, sizeof(int), int_comparator), 0);
    ck_assert_int_eq(array[0], 24);
}
END_TEST

START_TEST(two_elements)
{
    int array[] = { 12, 4 };

    ck_assert_int_eq(mysort(array, 2, sizeof(int), int_comparator), 0);
    ck_assert_int_eq(array[0], 4);
    ck_assert_int_eq(array[1], 12);
}
END_TEST

START_TEST(repeating_elements)
{
    int array[] = { 15, 15, 15 };

    ck_assert_int_eq(mysort(array, 3, sizeof(int), int_comparator), 0);
    ck_assert_int_eq(array[0], 15);
    ck_assert_int_eq(array[1], 15);
    ck_assert_int_eq(array[2], 15);
}
END_TEST

void check_sort_func(TCase *tc_core)
{
    tcase_add_test(tc_core, single_element);
    tcase_add_test(tc_core, two_elements);
    tcase_add_test(tc_core, repeating_elements);
}
