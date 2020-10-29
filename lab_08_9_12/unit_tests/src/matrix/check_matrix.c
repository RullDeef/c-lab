#include <check.h>
#include <stdlib.h>
#include <errno.h>
#include "../../../src/matrix/matrix.h"

static matrix_t imp__craft_mat(size_t rows, size_t cols, const char *str)
{
    matrix_t result = mat_create(rows, cols);
    char *str_end = NULL;

    for (size_t row = 0; row < rows; row++)
    {
        for (size_t col = 0; col < cols; col++)
        {
            matrix_elem_t value = strtol(str, &str_end, 10);
            mat_set(&result, row, col, value);
            str = str_end + 1;
        }
    }

    return result;
}

START_TEST(create_invalid_mat)
{
    matrix_t mat = mat_null();
    matrix_elem_t value = 0.0;

    mat_get(&mat, 0, 0, &value);
    ck_assert_double_eq(value, 0.0);
    
    mat = mat_create(0, 10);
    ck_assert(mat_is_null(&mat));
    
    mat = mat_create(20, 0);
    ck_assert(mat_is_null(&mat));
    
    mat = mat_create(0, 0);
    ck_assert(mat_is_null(&mat));
}
END_TEST

START_TEST(test_copy)
{
    matrix_t src = imp__craft_mat(5, 3,
        "  1  0 -2 "
        "  4 -7  6 "
        "-12  0  0 "
        "  7  0  6 "
        " 13 -6  4 "
    );

    matrix_t dst = mat_copy(&src);
    ck_assert(!mat_is_null(&dst));
    ck_assert(src.data != dst.data);

    mat_free(&src);
    mat_free(&dst);
}
END_TEST

START_TEST(test_resize)
{
    matrix_t mat = imp__craft_mat(1, 1, "4");
    matrix_elem_t value = 0.0;

    int status = mat_resize(&mat, 5, 5);
    ck_assert_int_eq(status, mat_success);
    ck_assert(!mat_is_null(&mat));
    ck_assert_int_eq(mat.rows, 5);
    ck_assert_int_eq(mat.cols, 5);

    mat_get(&mat, 0, 0, &value);
    ck_assert_double_eq(value, 4.0);

    mat_free(&mat);
}
END_TEST

START_TEST(test_add)
{
    matrix_t mat_1 = imp__craft_mat(2, 3,
        " 1 0 -3 "
        " 4 -2 5 "
    );

    matrix_t mat_2 = mat_null();
    matrix_t res = mat_null();

    int status = mat_add(&mat_1, &mat_2, &res);
    ck_assert_int_ne(status, mat_success);

    mat_2 = imp__craft_mat(2, 2,
        " 5 -1 "
        " 0  3 "
    );

    status = mat_add(&mat_1, &mat_2, &res);
    ck_assert_int_ne(status, mat_success);

    mat_free(&mat_2);
    mat_2 = imp__craft_mat(2, 3,
        " 5 -1 0 "
        " 0 3 -2 "
    );

    status = mat_add(&mat_1, &mat_2, &res);
    ck_assert_int_eq(status, mat_success);

    mat_free(&mat_1);
    mat_free(&mat_2);
    mat_free(&res);
}
END_TEST

START_TEST(test_mult)
{
    matrix_t mat_1 = imp__craft_mat(2, 3,
        " 1 0 -3 "
        " 4 -2 5 "
    );

    matrix_t mat_2 = mat_null();
    matrix_t res = mat_null();

    int status = mat_mult(&mat_1, &mat_2, &res);
    ck_assert_int_ne(status, mat_success);

    mat_2 = imp__craft_mat(2, 2,
        " 5 -1 "
        " 0  3 "
    );

    status = mat_mult(&mat_1, &mat_2, &res);
    ck_assert_int_ne(status, mat_success);

    mat_free(&mat_2);
    mat_2 = imp__craft_mat(3, 2,
        "  5 -1 "
        "  0  3 "
        " -2  0 "
    );

    status = mat_mult(&mat_1, &mat_2, &res);
    ck_assert_int_eq(status, mat_success);

    mat_free(&mat_1);
    mat_free(&mat_2);
    mat_free(&res);
}
END_TEST

START_TEST(solve_sle)
{
    matrix_t mat = imp__craft_mat(3, 4,
        "  1  0 0  -2 "
        "  4 -7 0 -22 "
        "  0  0 1   4 "
    );

    matrix_t res = mat_null();
    matrix_elem_t value = 0.0;

    int status = mat_solve_sle(&mat, &res);

    ck_assert_int_eq(status, mat_success);
    ck_assert(!mat_is_null(&res));
    ck_assert_int_eq(res.rows, 3);
    ck_assert_int_eq(res.cols, 1);

    mat_get(&res, 0, 0, &value);
    ck_assert_double_eq(value, -2.0);

    mat_get(&res, 1, 0, &value);
    ck_assert_double_eq(value, 2.0);

    mat_get(&res, 2, 0, &value);
    ck_assert_double_eq(value, 4.0);

    mat_free(&mat);
    mat_free(&res);
}
END_TEST

START_TEST(solve_sle_bad_dims)
{
    matrix_t mat = imp__craft_mat(3, 3,
        "  1  0  -2 "
        "  4 -7 -22 "
        "  0  1   3 "
    );

    matrix_t res = mat_null();

    int status = mat_solve_sle(&mat, &res);

    ck_assert_int_ne(status, mat_success);
    ck_assert(mat_is_null(&res));

    mat_free(&mat);
    mat_free(&res);
}
END_TEST

START_TEST(solve_sle_bad_mat)
{
    matrix_t mat = imp__craft_mat(3, 4,
        "  1  0  -2  0 "
        "  4 -3 -17 -6 "
        "  0  1   3  2 "
    );

    matrix_t res = mat_null();

    int status = mat_solve_sle(&mat, &res);

    ck_assert_int_ne(status, mat_success);
    ck_assert(mat_is_null(&res));

    mat_free(&mat);
    mat_free(&res);
}
END_TEST

START_TEST(solve_sle_zero_mat)
{
    matrix_t mat = imp__craft_mat(3, 4,
        "  0 0 0 0 "
        "  0 0 0 0 "
        "  0 0 0 0 "
    );

    matrix_t res = mat_null();

    int status = mat_solve_sle(&mat, &res);

    ck_assert_int_ne(status, mat_success);
    ck_assert(mat_is_null(&res));

    mat_free(&mat);
    mat_free(&res);
}
END_TEST

START_TEST(solve_sle_impossible_mat)
{
    matrix_t mat = imp__craft_mat(3, 4,
        "  0 5 0 0 "
        "  0 0 3 0 "
        "  0 0 0 1 "
    );

    matrix_t res = mat_null();

    int status = mat_solve_sle(&mat, &res);

    ck_assert_int_ne(status, mat_success);
    ck_assert(mat_is_null(&res));

    mat_free(&mat);
    mat_free(&res);
}
END_TEST

void test_case_matrix(TCase *tc_core)
{
    tcase_add_test(tc_core, create_invalid_mat);
    tcase_add_test(tc_core, test_copy);
    tcase_add_test(tc_core, test_resize);
    tcase_add_test(tc_core, test_add);
    tcase_add_test(tc_core, test_mult);
    tcase_add_test(tc_core, solve_sle);
    tcase_add_test(tc_core, solve_sle_bad_dims);
    tcase_add_test(tc_core, solve_sle_bad_mat);
    tcase_add_test(tc_core, solve_sle_zero_mat);
    tcase_add_test(tc_core, solve_sle_impossible_mat);
}
