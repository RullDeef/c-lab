#ifndef __ITEM_H_
#define __ITEM_H_

#include <stdbool.h>

struct item
{
    char *name;
    unsigned int price;
};

struct item it_construct(char *title, unsigned int price);
struct item it_invalid(void);

void it_destroy(struct item *it);

bool it_is_valid(const struct item *it);

#endif // __ITEM_H_
