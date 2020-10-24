#ifndef __ITEM_ARRAY_H_
#define __ITEM_ARRAY_H_

#include <stdbool.h>
#include "item.h"

#define ITA_INIT_CAPACITY 10U
#define ITA_ALLOC_MULTIPLIER 1.5f

struct item_array
{
    unsigned int size;
    unsigned int capacity;
    struct item *data;
};

struct item_array ita_create(void);
struct item_array ita_invalid(void);

void ita_destroy(struct item_array *ita);

bool ita_is_valid(const struct item_array *ita);

int ita_resize(struct item_array *ita, unsigned int size);
int ita_insert(struct item_array *ita, struct item *it);

#endif // __ITEM_ARRAY_H_
