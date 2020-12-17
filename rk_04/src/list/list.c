#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct node
{
    struct node *next;
    void *data;
};

struct list
{
    struct node *head;
};

list_t lst_create(void)
{
    struct list *list = calloc(1, sizeof(struct list));
    return list;
}

void lst_destroy(list_t list, lst_unary_fn_t func)
{
    if (list != NULL)
    {
        struct node *node = list->head;

        while (node != NULL)
        {
            struct node *next = node->next;

            if (func != NULL)
                func(node->data);

            free(node);
            node = next;
        }

        free(list);
    }
}

int lst_append(list_t list, void *data)
{
    int status = EXIT_FAILURE;

    if (list != NULL)
    {
        struct node *back = list->head;
        if (back == NULL)
        {
            list->head = malloc(sizeof(struct node));

            if (list->head != NULL)
            {
                list->head->next = NULL;
                list->head->data = data;
                status = EXIT_SUCCESS;
            }
        }
        else
        {
            while (back->next != NULL)
                back = back->next;
            back->next = malloc(sizeof(struct node));

            if (back->next != NULL)
            {
                back->next->next = NULL;
                back->next->data = data;
                status = EXIT_SUCCESS;
            }
        }
    }

    return status;
}

int lst_pop_back(list_t list, void **data)
{
    int status = EXIT_FAILURE;

    if (list != NULL && list->head != NULL && data != NULL)
    {
        struct node **back = &(list->head);
        while ((*back)->next != NULL)
            back = &((*back)->next);
        
        *data = (*back)->data;
        free(*back);
        *back = NULL;

        status = EXIT_SUCCESS;
    }

    return status;
}

int lst_for_each(list_t list, lst_unary_fn_t func)
{
    int status = EXIT_FAILURE;

    if (list != NULL)
    {
        for (struct node *node = list->head; node != NULL; node = node->next)
            func(node->data);
        status = EXIT_SUCCESS;
    }

    return status;
}
