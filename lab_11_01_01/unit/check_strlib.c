#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include "strlib.h"

#define check_snprintf(fmt, ...)                                 \
    {                                                            \
        size_t size = 2 + snprintf(NULL, 0, fmt, __VA_ARGS__);   \
        char *buf_1 = calloc(size, sizeof(char));                \
        char *buf_2 = calloc(size, sizeof(char));                \
        for (size_t i = 0; i < size; i++)                        \
        {                                                        \
            int res_1 = snprintf(buf_1, i, fmt, __VA_ARGS__);    \
            int res_2 = my_snprintf(buf_2, i, fmt, __VA_ARGS__); \
                                                                 \
            ck_assert_int_eq(res_1, res_2);                      \
            ck_assert_str_eq(buf_1, buf_2);                      \
        };                                                       \
        free(buf_1);                                             \
        free(buf_2);                                             \
    }

START_TEST(test_char)
{
    check_snprintf("hello%c", '!');
    check_snprintf("%c%%", 'j');
    check_snprintf("%c", '?');
    check_snprintf("word %c", '4');
    check_snprintf("%c%c", 'a', 'b');
}
END_TEST

START_TEST(test_str)
{
    check_snprintf("hello%s", "!");
    check_snprintf("%s!", "hello");
    check_snprintf("%s", "single");
    check_snprintf("word %s", "after");
    check_snprintf("%s%s", "first", "second");
}
END_TEST

START_TEST(test_dec)
{
    int nums[] = { 0, 1, -1, 123456789, -123456789, 10000, -98700 };
    for (int *num = nums; *num != nums[6]; num++)
    {
        check_snprintf("%d", *num);
        check_snprintf("num = %d", *num);
        check_snprintf("%d = num", *num);
        check_snprintf("num = %d = num", *num);
        check_snprintf("%d%d", *num, *num);
    }
}
END_TEST

START_TEST(test_hex)
{
    int nums[] = { 0, 1, 123456789, 16, 256, 10000 };
    for (int *num = nums; *num != nums[5]; num++)
    {
        check_snprintf("%x", *num);
        check_snprintf("num = %x", *num);
        check_snprintf("%x = num", *num);
        check_snprintf("num = %x = num", *num);
        check_snprintf("%x%x", *num, *num);
    }
}
END_TEST

START_TEST(test_oct)
{
    int nums[] = { 0, 1, 123456789, 16, 256, 10000 };
    for (int *num = nums; *num != nums[5]; num++)
    {
        check_snprintf("%o", *num);
        check_snprintf("num = %o", *num);
        check_snprintf("%o = num", *num);
        check_snprintf("num = %o = num", *num);
        check_snprintf("%o%o", *num, *num);
    }
}
END_TEST

START_TEST(test_long)
{
    long nums[] = { 0, 1, 123456789, 16, 256, 10000 };
    for (long *num = nums; *num != nums[5]; num++)
    {
        check_snprintf("%ld", *num);
        check_snprintf("num = %ld", *num);
        check_snprintf("%ld = num", *num);
        check_snprintf("num = %ld = num", *num);
        check_snprintf("%ld%ld", *num, *num);
    }
}
END_TEST

int main(void)
{
    Suite *suite = suite_create("my_snprintf");
    TCase *tcase = tcase_create("core");

    tcase_add_test(tcase, test_char);
    tcase_add_test(tcase, test_str);
    tcase_add_test(tcase, test_dec);
    tcase_add_test(tcase, test_hex);
    tcase_add_test(tcase, test_oct);
    tcase_add_test(tcase, test_long);
    suite_add_tcase(suite, tcase);

    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_VERBOSE);

    int failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return failed;
}
