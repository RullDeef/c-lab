#ifndef __LIST_H_
#define __LIST_H_

typedef struct list *list_t;

typedef int (*lst_unary_fn_t)(void *);

list_t lst_create(void);
void lst_destroy(list_t list, lst_unary_fn_t func);

int lst_append(list_t list, void *data);
int lst_pop_back(list_t list, void **data);

int lst_for_each(list_t list, lst_unary_fn_t func);

#endif // __LIST_H_
