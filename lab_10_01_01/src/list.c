#include <stdlib.h>
#include "list.h"

int copy(node_t *head, node_t **new_head)
{
    int status = EXIT_FAILURE;

    if (new_head != NULL)
    {
        status = head ? EXIT_SUCCESS : EXIT_FAILURE;
        *new_head = NULL;
        node_t **node = new_head;
        while (status == EXIT_SUCCESS && head != NULL)
        {
            *node = malloc(sizeof(node_t));
            if (*node == NULL)
                status = EXIT_FAILURE;
            else
            {
                **node = *head;
                node = &((*node)->next);
                head = head->next;
            }
        }

        while (status == EXIT_FAILURE && *new_head != NULL)
        {
            node_t *next = (*new_head)->next;
            free(*new_head);
            *new_head = next;
        }
    }

    return status;
}

void *pop_front(node_t **head)
{
    void *data = NULL;

    if (head != NULL && *head != NULL)
    {
        data = (*head)->data;

        node_t *old_head = *head;
        *head = (*head)->next;
        free(old_head);
    }

    return data;
}

void *pop_back(node_t **head)
{
    void *data = NULL;

    if (head != NULL && *head != NULL)
    {
        node_t **back = head;

        if ((*head)->next != NULL)
        {
            node_t *bef_back = *head;
            while (bef_back->next->next != NULL)
                bef_back = bef_back->next;

            back = &(bef_back->next);
        }

        data = (*back)->data;
        free(*back);
        *back = NULL;
    }

    return data;
}

void insert(node_t **head, node_t *elem, node_t *before)
{
    while (*head != NULL && *head != before)
        head = &((*head)->next);
    
    if (*head != NULL)
    {
        elem->next = *head;
        *head = elem;
    }
}

void append(node_t **head_a, node_t **head_b)
{
    while (*head_a != NULL)
        head_a = &((*head_a)->next);
    
    *head_a = *head_b;
    *head_b = NULL;
}

node_t *find(node_t *head, const void *data, int (*cmp)(const void *, const void *))
{
    node_t *result = NULL;

    while (result == NULL && head != NULL)
    {
        if (cmp(data, head->data) == 0)
            result = head;
        head = head->next;
    }

    return result;
}

void remove_duplicates(node_t **head, int (*cmp)(const void *, const void *))
{
    while (*head != NULL && (*head)->next != NULL)
    {
        if (cmp((*head)->data, (*head)->next->data) == 0)
        {
            node_t *rem = (*head)->next;
            (*head)->next = (*head)->next->next;
            free(rem);
        }
        else
            head = &((*head)->next);
    }
}

node_t *reverse(node_t *head)
{
    node_t *new_head = NULL;

    for (node_t *temp; head; new_head = head, head = temp)
    {
        temp = head->next;
        head->next = new_head;
    }

    return new_head;
}

void front_back_split(node_t *head, node_t **back)
{
    node_t **mid = &head;

    for (node_t *node = head; node != NULL && node->next != NULL; node = node->next->next)
        mid = node->next->next ? &((*mid)->next) : mid;
    
    *back = *mid ? (*mid)->next : NULL;
    if (*mid != NULL)
        (*mid)->next = NULL;
}

node_t *sorted_merge(node_t **head_a, node_t **head_b, int (*cmp)(const void *, const void *))
{
    node_t *head = NULL;
    node_t **node = &head;

    node_t *node_a = *head_a;
    node_t *node_b = *head_b;

    while (node_a != NULL && node_b != NULL)
    {
        int res = cmp(node_a->data, node_b->data);

        if (res <= 0)
        {
            *node = node_a;
            node_a = node_a->next;
        }
        else
        {
            *node = node_b;
            node_b = node_b->next;
        }

        node = &((*node)->next);
    }

    *node = node_a ? node_a : node_b;
    *head_a = NULL;
    *head_b = NULL;

    return head;
}

node_t *sort(node_t *head, int (*cmp)(const void *, const void *))
{
    if (head != NULL && head->next != NULL)
    {
        node_t *back = NULL;
        front_back_split(head, &back);

        head = sort(head, cmp);
        back = sort(back, cmp);
        head = sorted_merge(&head, &back, cmp);
    }

    return head;
}
