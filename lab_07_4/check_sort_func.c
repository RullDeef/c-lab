#include <check.h>
#include "sort_func.h"

#define start_test(x) START_TEST(x)
#define end_test END_TEST

#define tcase TCase

start_test(invalid_array_ptr)
{
    ck_assert_int_ne(mysort(NULL, 4, sizeof(int), int_comparator), 0);
}
end_test

start_test(invalid_array_size)
{
    int array[] = { 10, 20, 14 };

    ck_assert_int_ne(mysort(array, -3, sizeof(int), int_comparator), 0);
}
end_test

start_test(invalid_element_size)
{
    int array[] = { 10, 20, 14 };

    ck_assert_int_ne(mysort(array, 3, 0, int_comparator), 0);
}
end_test

start_test(single_element)
{
    int array[] = { 24 };

    ck_assert_int_eq(mysort(array, 1, sizeof(int), int_comparator), 0);
    ck_assert_int_eq(array[0], 24);
}
end_test

start_test(two_elements)
{
    int array[] = { 12, 4 };

    ck_assert_int_eq(mysort(array, 2, sizeof(int), int_comparator), 0);
    ck_assert_int_eq(array[0], 4);
    ck_assert_int_eq(array[1], 12);
}
end_test

start_test(repeating_elements)
{
    int array[] = { 15, 15, 15 };

    ck_assert_int_eq(mysort(array, 3, sizeof(int), int_comparator), 0);
    ck_assert_int_eq(array[0], 15);
    ck_assert_int_eq(array[1], 15);
    ck_assert_int_eq(array[2], 15);
}
end_test

void check_sort_func(tcase *tc_core)
{
    tcase_add_test(tc_core, invalid_array_ptr);
    tcase_add_test(tc_core, invalid_array_size);
    tcase_add_test(tc_core, invalid_element_size);
    tcase_add_test(tc_core, single_element);
    tcase_add_test(tc_core, two_elements);
    tcase_add_test(tc_core, repeating_elements);
}
