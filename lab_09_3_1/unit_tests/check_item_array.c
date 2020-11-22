#include <check.h>
#include "item_array.h"

#define	EXIT_FAILURE	1	/* Failing exit status.  */
#define	EXIT_SUCCESS	0	/* Successful exit status.  */

START_TEST(item_array_create)
{
    struct item_array ita = ita_invalid();
    ck_assert_ptr_eq(ita.data, NULL);
    ck_assert_uint_eq(ita.size, 0U);
    ck_assert_uint_eq(ita.capacity, 0U);
    
    ita = ita_create();
    ck_assert_ptr_ne(ita.data, NULL);
    ck_assert_uint_eq(ita.size, 0U);
    ck_assert_uint_ne(ita.capacity, 0U);

    ck_assert(ita_is_valid(&ita));
    ita_destroy(&ita);
    ck_assert(!ita_is_valid(&ita));

    ck_assert_ptr_eq(ita.data, NULL);
    ck_assert_uint_eq(ita.size, 0U);
    ck_assert_uint_eq(ita.capacity, 0U);
}
END_TEST

START_TEST(item_array_resize)
{
    struct item_array ita = ita_create();

    ita_resize(&ita, ITA_INIT_CAPACITY + 1U);
    ck_assert_uint_ge(ita.capacity, ITA_INIT_CAPACITY + 1U);

    int status = ita_resize(&ita, (unsigned int)-1);
    ck_assert_int_eq(status, EXIT_FAILURE);

    ita_destroy(&ita);
}
END_TEST

START_TEST(item_array_insert)
{
    struct item_array ita = ita_create();
    struct item it = it_construct(strdup("spoon"), 15U);

    ita_insert(&ita, &it);
    ck_assert_uint_eq(ita.size, 1U);

    it = it_construct(strdup("fork"), 25U);
    ita_insert(&ita, &it);
    ck_assert_uint_eq(ita.size, 2U);

    ita_destroy(&ita);
}

Suite *item_array_suite(void)
{
    Suite *suite = suite_create("item array suite");
    TCase *tcase = tcase_create("core");

    tcase_add_test(tcase, item_array_create);
    tcase_add_test(tcase, item_array_resize);
    tcase_add_test(tcase, item_array_insert);

    suite_add_tcase(suite, tcase);
    return suite;
}
