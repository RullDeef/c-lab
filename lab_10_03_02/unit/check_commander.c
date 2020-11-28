#include <check.h>
#include "commander.h"

START_TEST(parse)
{
    ck_assert_int_eq(cmd_parse(""), cmd_invalid);
    ck_assert_int_eq(cmd_parse("cmd"), cmd_invalid);
    ck_assert_int_eq(cmd_parse("out"), cmd_out);
    ck_assert_int_eq(cmd_parse("mul"), cmd_mul);
    ck_assert_int_eq(cmd_parse("div"), cmd_div);
    ck_assert_int_eq(cmd_parse("sqr"), cmd_sqr);
}
END_TEST

START_TEST(parse_number)
{
    ck_assert_uint_eq(cmd_parse_number("0"), 0U);
    ck_assert_uint_eq(cmd_parse_number("50"), 50U);
    ck_assert_uint_eq(cmd_parse_number("123"), 123U);
    ck_assert_uint_eq(cmd_parse_number("1027"), 1027U);
    ck_assert_uint_eq(cmd_parse_number("776"), 776U);
}
END_TEST

Suite *commander_suite(void)
{
    Suite *suite = suite_create("commander");
    TCase *tcase = tcase_create("core");

    tcase_add_test(tcase, parse);
    tcase_add_test(tcase, parse_number);

    suite_add_tcase(suite, tcase);
    return suite;
}
