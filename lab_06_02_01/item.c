#include <string.h>
#include "item.h"

void imp__swap_items(item_t *item_1, item_t *item_2);
void imp__pop_item(item_t *items, short int *items_count, short int index);
bool imp__item_is_valid(const item_t *item);

void imp__swap_items(item_t *item_1, item_t *item_2)
{
    item_t temp = *item_1;
    *item_1 = *item_2;
    *item_2 = temp;
}

void imp__pop_item(item_t *items, short int *items_count, short int index)
{
    for (short int i = index; i + 1 < *items_count; i++)
        items[i] = items[i + 1];
    (*items_count)--;
}

bool imp__item_is_valid(const item_t *item)
{
    return item->mass > 0.0f && item->volume > 0.0f;
}

bool has_invalid_items(const item_t *items, short int items_count)
{
    bool has_invalid_item = false;

    for (short int i = 0; i < items_count; i++)
    {
        if (!imp__item_is_valid(items + i))
        {
            has_invalid_item = true;
            break;
        }
    }

    return has_invalid_item;
}

void calculate_density(item_t *items, short int items_count)
{
    for (short int i = 0; i < items_count; i++)
        items[i].density = items[i].mass / items[i].volume;
}

void sort_items_array(item_t *items, short int items_count)
{
    for (short int i = 1; i < items_count; i++)
        for (short int j = i - 1; j >= 0 && (items[j].density > items[j + 1].density); j--)
            imp__swap_items(items + j, items + j + 1);
}

void filter_array(item_t *items, short int *items_count, const char *prefix)
{
    for (short int i = 0; i < *items_count; i++)
        if (strstr(items[i].name, prefix) != items[i].name)
            imp__pop_item(items, items_count, i--);
}
