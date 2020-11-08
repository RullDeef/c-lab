#include <stdlib.h>
#include "list.h"

int copy(node_t *head, node_t **new_head)
{
    int status = EXIT_SUCCESS;

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
        }
    }

    while (status == EXIT_FAILURE && *new_head != NULL)
    {
        node_t *next = (*new_head)->next;
        free(*new_head);
        *new_head = next;
    }

    return status;
}

void *pop_front(node_t **head)
{
    void *data = NULL;

    if (*head != NULL)
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

    if (*head != NULL)
    {
        node_t **back = head;

        if ((*head)->next != NULL)
        {
            node_t *bef_back = *head;
            while (bef_back->next->next != NULL)
                bef_back = (*back)->next;

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

    for (node_t *next = head ? head->next : NULL; head; new_head = head, head = next)
        head->next = new_head;

    return new_head;
}

void front_back_split(node_t *head, node_t **back)
{
    node_t **mid = &head;

    for (node_t *node = head; node != NULL && node->next != NULL; node = node->next->next)
        mid = &((*mid)->next);
    
    *back = *mid ? (*mid)->next : NULL;
    if (*mid != NULL)
        (*mid)->next = NULL;
}

node_t *sorted_merge(node_t **head_a, node_t **head_b, int (*cmp)(const void *, const void *))
{
    node_t *head = NULL;
    node_t **node = &head;

    node_t **old_head_a = head_a;
    node_t **old_head_b = head_b;

    while (*head_a != NULL && *head_b != NULL)
    {
        int res = cmp((*head_a)->data, (*head_b)->data);

        *node = res <= 0 ? *head_a : *head_b;
        node_t ***target = res <= 0 ? &head_a : &head_b;
        *target = &((**target)->next);

        node = &((*node)->next);
    }

    *node = *head_a ? *head_a : *head_b;
    *old_head_a = NULL;
    *old_head_b = NULL;

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
