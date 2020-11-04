#include <check.h>
#include "num_dcmp.h"

START_TEST(decompose)
{
    struct num_dcmp nd;
    
    nd = nd_decompose(0U);
    ck_assert(nd.head == NULL);

    nd = nd_decompose(1U);
    ck_assert(nd.head == NULL);

    nd = nd_decompose(2U);
    ck_assert(nd.head != NULL);
    ck_assert(nd.head->power == 1);
    ck_assert(nd.head->next == NULL);
    nd_destroy(&nd);

    nd = nd_decompose(2U * 25U * 1331U);
    ck_assert(nd.head != NULL);
    ck_assert(nd.head->power == 1); // 2
    ck_assert(nd.head->next != NULL);
    ck_assert(nd.head->next->power == 0);
    ck_assert(nd.head->next->next != NULL);
    ck_assert(nd.head->next->next->power == 2); // 5 * 5 = 25
    ck_assert(nd.head->next->next->next != NULL);
    ck_assert(nd.head->next->next->next->power == 0);
    ck_assert(nd.head->next->next->next->next != NULL);
    ck_assert(nd.head->next->next->next->next->power == 3); // 11 ^ 3 = 1331
    ck_assert(nd.head->next->next->next->next->next == NULL);
    nd_destroy(&nd);
}
END_TEST

START_TEST(multiply)
{
    struct num_dcmp nd_1 = nd_decompose(18U);
    struct num_dcmp nd_2 = nd_decompose(25U);

    struct num_dcmp res = nd_multiply(&nd_1, &nd_2);

    ck_assert(res.head != NULL);
    ck_assert(res.head->power == 1);
    ck_assert(res.head->next != NULL);
    ck_assert(res.head->next->power == 2);
    ck_assert(res.head->next->next != NULL);
    ck_assert(res.head->next->next->power == 2);
    ck_assert(res.head->next->next->next == NULL);

    nd_destroy(&nd_1);
    nd_destroy(&nd_2);
    nd_destroy(&res);
}
END_TEST

START_TEST(divide)
{
    struct num_dcmp nd_1 = nd_decompose(135000U);
    struct num_dcmp nd_2 = nd_decompose(300U);

    struct num_dcmp res = nd_divide(&nd_1, &nd_2);

    ck_assert(res.head != NULL);
    ck_assert(res.head->power == 1);
    ck_assert(res.head->next != NULL);
    ck_assert(res.head->next->power == 2);
    ck_assert(res.head->next->next != NULL);
    ck_assert(res.head->next->next->power == 2);
    ck_assert(res.head->next->next->next == NULL);

    nd_destroy(&nd_1);
    nd_destroy(&nd_2);
    nd_destroy(&res);
}
END_TEST

START_TEST(square)
{
    struct num_dcmp nd = nd_decompose(450U);
    struct num_dcmp res = nd_square(&nd);

    ck_assert(res.head != NULL);
    ck_assert(res.head->power == 2);
    ck_assert(res.head->next != NULL);
    ck_assert(res.head->next->power == 4);
    ck_assert(res.head->next->next != NULL);
    ck_assert(res.head->next->next->power == 4);
    ck_assert(res.head->next->next->next == NULL);

    nd_destroy(&nd);
    nd_destroy(&res);
}

Suite *num_dcmp_suite(void)
{
    Suite *suite = suite_create("num decompose");
    TCase *tcase = tcase_create("core");

    tcase_add_test(tcase, decompose);
    tcase_add_test(tcase, multiply);
    tcase_add_test(tcase, divide);
    tcase_add_test(tcase, square);

    suite_add_tcase(suite, tcase);
    return suite;
}
