#ifndef __LIST_H_
#define __LIST_H_

typedef struct node
{
    void *data;
    struct node *next;
} node_t;

int copy(node_t *head, node_t **new_head);

void *pop_front(node_t **head);
void *pop_back(node_t **head);

void insert(node_t **head, node_t *elem, node_t *before);
void append(node_t **head_a, node_t **head_b);
node_t *find(node_t *head, const void *data, int (*cmp)(const void *, const void *));

void remove_duplicates(node_t **head, int (*cmp)(const void *, const void *));

node_t *reverse(node_t *head);

void front_back_split(node_t *head, node_t **back);
node_t *sorted_merge(node_t **head_a, node_t **head_b, int (*cmp)(const void *, const void *));

node_t *sort(node_t *head, int (*cmp)(const void *, const void *));

#endif
