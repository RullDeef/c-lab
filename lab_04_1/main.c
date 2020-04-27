#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <string.h>
#include "my_strspn.h"

#define TESTS_AMOUNT 7
#define TEST_ARGS_AMOUNT 3
typedef char *test_data_t[TEST_ARGS_AMOUNT];

int test_pass_fail(const test_data_t test_data);

int main(void)
{
    test_data_t tests_data[TESTS_AMOUNT] = {
        { "", "", "" },
        { "", "abc", "" },
        { "aef", "", "aef" },
        { "abc", "ca", "bc" },
        { "vbb", "qwe", "vbb" },
        { "xzzs", "xz", "s" },
        { "tttrt", "rrt", "" }
        };

    int error_amount = 0;

    for (size_t i = 0; i < TESTS_AMOUNT; i++)
    {
        int failed = test_pass_fail(tests_data[i]);
        printf("running test #%zu: %s\n", i + 1, failed ? "failed" : "passed");
        error_amount += failed;
    }

    return error_amount;
}

int test_pass_fail(const test_data_t test_data)
{
    const char *str1 = test_data[0];
    const char *str2 = test_data[1];
    const char *res = test_data[2];
    
    size_t offset = my_strspn(str1, str2);
    const char *out = str1 + offset;

    return strcmp(res, out);
}