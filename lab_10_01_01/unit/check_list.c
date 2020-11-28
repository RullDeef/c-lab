#include <stdlib.h>
#include <stdarg.h>
#include <check.h>
#include "list.h"

node_t *str_list_construct(int n, ...)
{
    node_t *head = NULL;
    node_t **next = &head;

    va_list args;
    va_start(args, n);

    for (int i = 0; i < n; i++)
    {
        *next = malloc(sizeof(node_t));
        (*next)->data = (void *)va_arg(args, char *);
        next = &((*next)->next);
        *next = NULL;
    }

    va_end(args);
    return head;
}

void list_free(node_t *head)
{
    for (node_t *next; head; free(head), head = next)
        next = head->next;
}

int strcmprator(const void *str_1, const void *str_2)
{
    return strcmp((const char *)str_1, (const char *)str_2);
}

START_TEST(list_find)
{
    node_t *list = str_list_construct(3, "hello", "world", "!");

    node_t *node = find(list, "hello", strcmprator);
    ck_assert(node != NULL);
    ck_assert(list == node);
    ck_assert_str_eq((const char*)node->data, "hello");

    node = find(list, "world", strcmprator);
    ck_assert(node != NULL);
    ck_assert(list->next == node);
    ck_assert_str_eq((const char*)node->data, "world");

    node = find(list, "!", strcmprator);
    ck_assert(node != NULL);
    ck_assert(list->next->next == node);
    ck_assert_str_eq((const char*)node->data, "!");

    node = find(list, "str", strcmprator);
    ck_assert(node == NULL);

    list_free(list);
}
END_TEST

START_TEST(list_copy)
{
    node_t *list_1 = str_list_construct(4, "a", "a2", "e", "800");
    node_t *list_2 = NULL;

    int res = copy(list_1, &list_2);
    ck_assert_int_eq(res, EXIT_SUCCESS);
    ck_assert(list_2 != NULL);
    ck_assert_str_eq((const char *)list_2->data, "a");
    ck_assert(list_2->next != NULL);
    ck_assert_str_eq((const char *)list_2->next->data, "a2");
    ck_assert(list_2->next->next != NULL);
    ck_assert_str_eq((const char *)list_2->next->next->data, "e");
    ck_assert(list_2->next->next->next != NULL);
    ck_assert_str_eq((const char *)list_2->next->next->next->data, "800");

    list_free(list_1);
    list_free(list_2);

    list_1 = NULL;
    list_2 = NULL;
    ck_assert_int_ne(copy(NULL, NULL), EXIT_SUCCESS);
    ck_assert_int_ne(copy(list_1, NULL), EXIT_SUCCESS);
    ck_assert_int_ne(copy(list_1, &list_2), EXIT_SUCCESS);
}
END_TEST

START_TEST(list_front_back_split_small)
{
    node_t *front = str_list_construct(2, "1", "2");
    node_t *back = NULL;

    front_back_split(front, &back);
    ck_assert(back != NULL);
    ck_assert_str_eq((const char *)back->data, "2");
    ck_assert(front->next == NULL);
    ck_assert(back->next == NULL);
    list_free(back);

    front_back_split(front, &back);
    ck_assert(back == NULL);
    list_free(front);

    front = NULL;
    front_back_split(front, &back);
    ck_assert(back == NULL);
}
END_TEST

START_TEST(list_front_back_split_odd)
{
    node_t *front = str_list_construct(5, "1", "2", "3", "4", "5");
    node_t *back = NULL;

    front_back_split(front, &back);
    ck_assert(back != NULL);
    ck_assert_str_eq((const char *)back->data, "4");
    ck_assert(back->next != NULL);
    ck_assert_str_eq((const char *)back->next->data, "5");
    ck_assert(back->next->next == NULL);

    list_free(front);
    list_free(back);
}
END_TEST

START_TEST(list_front_back_split_even)
{
    
    node_t *front = str_list_construct(6, "1", "2", "3", "4", "5", "6");
    node_t *back = NULL;

    front_back_split(front, &back);
    ck_assert(back != NULL);
    ck_assert_str_eq((const char *)back->data, "4");
    ck_assert(back->next != NULL);
    ck_assert_str_eq((const char *)back->next->data, "5");
    ck_assert(back->next->next != NULL);
    ck_assert_str_eq((const char *)back->next->next->data, "6");
    ck_assert(back->next->next->next == NULL);

    list_free(front);
    list_free(back);
}
END_TEST

START_TEST(list_pop_front)
{
    node_t *list = str_list_construct(3, "a", "b", "c");
    
    ck_assert_str_eq((const char *)pop_front(&list), "a");
    ck_assert_str_eq((const char *)list->data, "b");
    ck_assert_str_eq((const char *)pop_front(&list), "b");
    ck_assert_str_eq((const char *)list->data, "c");
    ck_assert_str_eq((const char *)pop_front(&list), "c");
    ck_assert(list == NULL);

    ck_assert(pop_front(&list) == NULL);
}
END_TEST

START_TEST(list_pop_back)
{
    node_t *list = str_list_construct(5, "a", "2", "b", "3", "c");
    
    ck_assert_str_eq((const char *)pop_back(&list), "c");
    ck_assert(list->next->next->next->next == NULL);
    ck_assert_str_eq((const char *)pop_back(&list), "3");
    ck_assert(list->next->next->next == NULL);
    ck_assert_str_eq((const char *)pop_back(&list), "b");
    ck_assert(list->next->next == NULL);
    ck_assert_str_eq((const char *)pop_back(&list), "2");
    ck_assert(list->next == NULL);
    ck_assert_str_eq((const char *)pop_back(&list), "a");
    ck_assert(list == NULL);

    ck_assert(pop_back(&list) == NULL);
    ck_assert(pop_back(NULL) == NULL);
}
END_TEST

START_TEST(list_reverse)
{
    node_t *list = str_list_construct(3, "str_1", "str_2", "str_3");
    list = reverse(list);

    ck_assert(list != NULL);
    ck_assert_str_eq((const char *)list->data, "str_3");
    ck_assert(list->next != NULL);
    ck_assert_str_eq((const char *)list->next->data, "str_2");
    ck_assert(list->next->next != NULL);
    ck_assert_str_eq((const char *)list->next->next->data, "str_1");
    ck_assert(list->next->next->next == NULL);

    list_free(list);
}
END_TEST

START_TEST(list_sorted_merge)
{
    node_t *list_1 = str_list_construct(2, "a", "c");
    node_t *list_2 = str_list_construct(3, "b", "d", "e");

    node_t *list = sorted_merge(&list_1, &list_2, strcmprator);
    ck_assert(list != NULL);
    ck_assert_str_eq((const char *)list->data, "a");
    ck_assert(list->next != NULL);
    ck_assert_str_eq((const char *)list->next->data, "b");
    ck_assert(list->next->next != NULL);
    ck_assert_str_eq((const char *)list->next->next->data, "c");
    ck_assert(list->next->next->next != NULL);
    ck_assert_str_eq((const char *)list->next->next->next->data, "d");
    ck_assert(list->next->next->next->next != NULL);
    ck_assert_str_eq((const char *)list->next->next->next->next->data, "e");
    ck_assert(list->next->next->next->next->next == NULL);

    list_free(list);
}
END_TEST

START_TEST(list_sort_small)
{
    node_t *list = NULL;

    ck_assert(sort(list, strcmprator) == NULL);

    list = str_list_construct(1, "3");
    list = sort(list, strcmprator);
    ck_assert(list != NULL);
    ck_assert_str_eq((const char *)list->data, "3");
    ck_assert(list->next == NULL);
    list_free(list);

    list = str_list_construct(2, "3", "1");
    list = sort(list, strcmprator);
    ck_assert(list != NULL);
    ck_assert_str_eq((const char *)list->data, "1");
    ck_assert(list->next != NULL);
    ck_assert_str_eq((const char *)list->next->data, "3");
    ck_assert(list->next->next == NULL);
    list_free(list);
}
END_TEST

START_TEST(list_sort)
{
    node_t *list = str_list_construct(5, "3", "2", "5", "1", "4");

    list = sort(list, strcmprator);
    ck_assert(list != NULL);
    ck_assert_str_eq((const char *)list->data, "1");
    ck_assert(list->next != NULL);
    ck_assert_str_eq((const char *)list->next->data, "2");
    ck_assert(list->next->next != NULL);
    ck_assert_str_eq((const char *)list->next->next->data, "3");
    ck_assert(list->next->next->next != NULL);
    ck_assert_str_eq((const char *)list->next->next->next->data, "4");
    ck_assert(list->next->next->next->next != NULL);
    ck_assert_str_eq((const char *)list->next->next->next->next->data, "5");
    ck_assert(list->next->next->next->next->next == NULL);

    list_free(list);
}
END_TEST

int main(void)
{
    Suite *suite = suite_create("list suite");
    TCase *tcase = tcase_create("core");

    tcase_add_test(tcase, list_find);
    tcase_add_test(tcase, list_copy);
    tcase_add_test(tcase, list_front_back_split_small);
    tcase_add_test(tcase, list_front_back_split_odd);
    tcase_add_test(tcase, list_front_back_split_even);

    tcase_add_test(tcase, list_pop_front);
    tcase_add_test(tcase, list_pop_back);

    tcase_add_test(tcase, list_reverse);

    tcase_add_test(tcase, list_sorted_merge);

    tcase_add_test(tcase, list_sort_small);
    tcase_add_test(tcase, list_sort);

    suite_add_tcase(suite, tcase);

    SRunner *runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);
    int failed = srunner_ntests_failed(runner);

    srunner_free(runner);
    return failed;
}
