#include <stdlib.h>
#include <string.h>
#include "item.h"

struct item it_construct(char *title, unsigned int price)
{
    struct item it = { .name = title, .price = price };
    return it;
}

struct item it_invalid(void)
{
    struct item it = { .name = NULL, .price = 0U };
    return it;
}

void it_destroy(struct item *it)
{
    if (it_is_valid(it))
    {
        free(it->name);
        *it = it_invalid();
    }
}

bool it_is_valid(const struct item *it)
{
    return it->name != NULL;
}
