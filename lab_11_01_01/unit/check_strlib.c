#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include <string.h>
#include <limits.h>
#include "strlib.h"

#define check_snprintf(fmt, ...)                                                                                                             \
    {                                                                                                                                        \
        size_t size = 2 + snprintf(NULL, 0, fmt, __VA_ARGS__);                                                                               \
        char *buf_1 = calloc(size, sizeof(char));                                                                                            \
        char *buf_2 = calloc(size, sizeof(char));                                                                                            \
        for (size_t j = 1; j < size; j++)                                                                                                    \
        {                                                                                                                                    \
            int res_1 = snprintf(buf_1, j, fmt, __VA_ARGS__);                                                                                \
            int res_2 = my_snprintf(buf_2, j, fmt, __VA_ARGS__);                                                                             \
                                                                                                                                             \
            if (res_1 != res_2)                                                                                                              \
            {                                                                                                                                \
                char *buf = malloc(1000);                                                                                                    \
                snprintf(buf, 1000, "i=%lu arg=%s expect: \"%s\"(%d), got \"%s\"(%d) -> " fmt, j, fmt, buf_1, res_1, buf_2, res_2, nums[i]); \
                ck_assert_msg(0, buf);                                                                                                       \
                free(buf);                                                                                                                   \
            }                                                                                                                                \
            else                                                                                                                             \
                ck_assert_str_eq(buf_1, buf_2);                                                                                              \
        }                                                                                                                                    \
        free(buf_1);                                                                                                                         \
        free(buf_2);                                                                                                                         \
    }

#define check_spec(spec, type, ...)                           \
    {                                                         \
        type nums[] = {__VA_ARGS__};                          \
        for (int i = 0; i * sizeof(type) < sizeof(nums); i++) \
        {                                                     \
            check_snprintf(spec, nums[i]);                    \
            check_snprintf("abc" spec, nums[i]);              \
            check_snprintf(spec "def", nums[i]);              \
            check_snprintf("ghi" spec "jkl", nums[i]);        \
        }                                                     \
    }

START_TEST(test_char)
{
    check_spec("%c", int, 'a', 'b', 'c', '0', '1', '2', '\n', '\t', '$');
}
END_TEST

START_TEST(test_str)
{
    check_spec("%s", const char *, "a", "abc", "", "1", "123\t12\n3", "%s%d%f");
}
END_TEST

START_TEST(test_dec)
{
    check_spec("%d", int, 0, 1, -1, 1234, -1234, INT_MIN, INT_MAX);
    // check_spec("%hd", int, 0, 1, -1, 1234, -1234, INT_MIN, INT_MAX);
    check_spec("%ld", long, 0, 1, -1, 1234, -1234, LONG_MIN, LONG_MAX);
}
END_TEST

START_TEST(test_hex)
{
    // check_spec("%x", int, 0, 1, -1, 1234, -1234, 16, 256, INT32_MIN, INT32_MAX);
    // check_spec("%hx", int, 0, 1, -1, 1234, -1234, 16, 256, INT32_MIN, INT32_MAX);
    // check_spec("%lx", unsigned long, 0, 1, -1, 1234, -1234, 16, 256, LONG_MIN, LONG_MAX);
}
END_TEST

START_TEST(test_oct)
{
    // check_spec("%o", int, 0, 1, -1, 1234, -1234, 16, 256, INT32_MIN, INT32_MAX);
    // check_spec("%ho", int, 0, 1, -1, 1234, -1234, 16, 256, INT32_MIN, INT32_MAX);
    // check_spec("%lo", long, 0, 1, -1, 1234, -1234, 16, 256, INT64_MIN, INT64_MAX);
}
END_TEST

int main(void)
{
    Suite *suite = suite_create("my_snprintf");
    TCase *tcase = tcase_create("core");

    tcase_add_test(tcase, test_char);
    tcase_add_test(tcase, test_str);
    tcase_add_test(tcase, test_oct);
    tcase_add_test(tcase, test_dec);
    tcase_add_test(tcase, test_hex);
    suite_add_tcase(suite, tcase);

    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_VERBOSE);

    int failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return failed;
}
