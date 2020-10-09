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
    
    mat_create(0, 10);
    ck_assert(mat_is_null(&mat));
    
    mat = mat_create(20, 0);
    ck_assert(mat_is_null(&mat));
    
    mat = mat_create(0, 0);
    ck_assert(mat_is_null(&mat));
}
END_TEST

START_TEST(t_mat_copy)
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
}
END_TEST

void test_case_matrix(TCase *tc_core)
{
    tcase_add_test(tc_core, create_invalid_mat);
    tcase_add_test(tc_core, t_mat_copy);
}
