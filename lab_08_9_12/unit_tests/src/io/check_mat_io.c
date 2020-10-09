#include <check.h>
#include "../../../src/io/mat_io.h"

START_TEST(simple_input)
{
    // prepare input file
    FILE *file = fopen("./unit_tests/data/m1x1.txt", "rt");

    matrix_t mat = mat_null();
    int status = mat_io_input_simple(file, &mat);

    fclose(file);

    ck_assert_int_eq(status, mat_success);
    ck_assert(!mat_is_null(&mat));

    mat_free(&mat);
}
END_TEST

START_TEST(coordinate_output)
{
    // prepare output file
    FILE *file = fopen("./unit_tests/data/m1x1_out.txt", "wt");

    matrix_t mat = mat_create(1, 1);
    mat_set(&mat, 0, 0, 15);

    int status = mat_io_output_coordinate(file, &mat, MAT_IO_INT_PRECISION);
    ck_assert_int_eq(status, mat_success);

    status = mat_io_output_coordinate(file, &mat, MAT_IO_DOUBLE_PRECISION);
    ck_assert_int_eq(status, mat_success);

    fclose(file);
    mat_free(&mat);
}
END_TEST

void test_case_mat_io(TCase *tc_core)
{
    tcase_add_test(tc_core, simple_input);
    tcase_add_test(tc_core, coordinate_output);
}
