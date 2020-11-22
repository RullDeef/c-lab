#include <stdlib.h>
#include <string.h>
#include <check.h>
#include "item.h"

START_TEST(item_construct)
{
    struct item it = it_construct("teapot", 240U);
    ck_assert_str_eq(it.name, "teapot");
    ck_assert_uint_eq(it.price, 240U);
}
END_TEST

START_TEST(item_invalid)
{
    struct item it = it_invalid();
    ck_assert_ptr_eq(it.name, NULL);
    ck_assert_uint_eq(it.price, 0U);
}
END_TEST

START_TEST(item_destroy)
{
    struct item it = it_construct(strdup("milk"), 60U);
    ck_assert_str_eq(it.name, "milk");
    ck_assert_uint_eq(it.price, 60U);

    it_destroy(&it);
    ck_assert_ptr_eq(it.name, NULL);
    ck_assert_uint_eq(it.price, 0U);
    ck_assert(!it_is_valid(&it));
}
END_TEST

Suite *item_suite(void)
{
    Suite *suite = suite_create("item suite");
    TCase *tcase = tcase_create("core");

    tcase_add_test(tcase, item_construct);
    tcase_add_test(tcase, item_invalid);
    tcase_add_test(tcase, item_destroy);

    suite_add_tcase(suite, tcase);

    return suite;
}
