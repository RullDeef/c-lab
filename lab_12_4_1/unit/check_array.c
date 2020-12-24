#include <stdlib.h>
#include <check.h>
#include "array.h"

START_TEST(test_create_destroy)
{
    struct array arr = arr_create(0U);

    ck_assert_ptr_eq(arr.data, NULL);
    ck_assert_uint_eq(arr.size, 0U);
    ck_assert_uint_eq(arr.capacity, 0U);

    arr = arr_create(10U);

    ck_assert_ptr_ne(arr.data, NULL);
    ck_assert_uint_eq(arr.size, 0U);
    ck_assert_uint_eq(arr.capacity, 10U);

    arr_destroy(&arr);

    ck_assert_ptr_eq(arr.data, NULL);
    ck_assert_uint_eq(arr.size, 0U);
    ck_assert_uint_eq(arr.capacity, 0U);

    // stil ok
    arr_destroy(&arr);
}
END_TEST

START_TEST(test_get_set)
{
    int status;
    struct array arr = arr_create(10U);

    // set first two elements explicitly
    {
        status = arr_set(&arr, 0U, 55);

        ck_assert_int_eq(status, EXIT_SUCCESS);
        ck_assert_uint_eq(arr.size, 1U);
        ck_assert_int_eq(arr.data[0], 55);

        status = arr_set(&arr, 1U, 102);

        ck_assert_int_eq(status, EXIT_SUCCESS);
        ck_assert_uint_eq(arr.size, 2U);
        ck_assert_int_eq(arr.data[1], 102);
    }

    // set out of bounds
    {
        status = arr_set(&arr, 10U, -78);

        ck_assert_int_eq(status, EXIT_FAILURE);
    }

    // get second element
    {
        int value = 0;
        status = arr_get(&arr, 1U, &value);

        ck_assert_int_eq(status, EXIT_SUCCESS);
        ck_assert_int_eq(value, 102);
    }

    // get out of bounds
    {
        int value = -1534;
        status = arr_get(&arr, 6U, &value);

        ck_assert_int_eq(status, EXIT_FAILURE);
        ck_assert_int_eq(value, -1534);
    }

    arr_destroy(&arr);
}
END_TEST

START_TEST(test_push_pop)
{
    int status;

    struct array arr = arr_create(2U);

    // pop from empty array
    {
        int value = 100500;
        status = arr_pop_back(&arr, &value);

        ck_assert_int_eq(status, EXIT_FAILURE);
        ck_assert_int_eq(value, 100500);
    }

    // push two items
    {
        status = arr_push_back(&arr, 96);

        ck_assert_int_eq(status, EXIT_SUCCESS);
        ck_assert_uint_eq(arr.size, 1U);
        ck_assert_uint_eq(arr.capacity, 2U);
        ck_assert_int_eq(arr.data[0], 96);

        status = arr_push_back(&arr, -2010);

        ck_assert_int_eq(status, EXIT_SUCCESS);
        ck_assert_uint_eq(arr.size, 2U);
        ck_assert_uint_eq(arr.capacity, 2U);
        ck_assert_int_eq(arr.data[1], -2010);
    }

    // push cause resize
    {
        status = arr_push_back(&arr, 666);

        ck_assert_int_eq(status, EXIT_SUCCESS);
        ck_assert_uint_eq(arr.size, 3U);
        ck_assert_uint_gt(arr.capacity, 2U);
        ck_assert_int_eq(arr.data[2], 666);
    }

    // pop last one
    {
        int value = 1002;
        status = arr_pop_back(&arr, &value);

        ck_assert_int_eq(status, EXIT_SUCCESS);
        ck_assert_uint_eq(arr.size, 2U);
        ck_assert_int_eq(value, 666);
    }

    arr_destroy(&arr);
}
END_TEST

Suite *array_suite(void)
{
    Suite *suite = suite_create("array");
    TCase *tcase = tcase_create("core");

    tcase_add_test(tcase, test_create_destroy);
    tcase_add_test(tcase, test_get_set);
    tcase_add_test(tcase, test_push_pop);

    suite_add_tcase(suite, tcase);
    return suite;
}
