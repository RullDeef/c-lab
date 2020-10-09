#include <check.h>
#include "../../../src/io/mat_io.h"

START_TEST(simple_input)
{
    // prepare input file
    FILE *file = fopen("m1x1.txt", "rt");

    if (file)
    {
        matrix_t mat = mat_null();
        int status = mat_io_input_simple(file, &mat);

        fclose(file);

        ck_assert_int_eq(status, 0);
        ck_assert(!mat_is_null(&mat));

        mat_free(&mat);
    }
    else
    {
        printf("bad filename!\n");
    }
    
}
END_TEST

void test_case_mat_io(TCase *tc_core)
{
    tcase_add_test(tc_core, simple_input);
}
