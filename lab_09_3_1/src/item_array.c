#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "item_array.h"

struct item_array ita_create(void)
{
    struct item_array ita = ita_invalid();

    ita.data = (struct item *)calloc(ITA_INIT_CAPACITY, sizeof(struct item));
    ita.capacity = ita.data == NULL ? 0U : ITA_INIT_CAPACITY;

    return ita;
}

struct item_array ita_invalid(void)
{
    struct item_array ita = { .size = 0U, .capacity = 0U, .data = NULL };
    return ita;
}

void ita_destroy(struct item_array *ita)
{
    if (ita_is_valid(ita))
    {
        for (unsigned int i = 0; i < ita->size; i++)
            it_destroy(ita->data + i);
        free(ita->data);
        *ita = ita_invalid();
    }
}

bool ita_is_valid(const struct item_array *ita)
{
    return ita->data != NULL;
}

int ita_resize(struct item_array *ita, unsigned int size)
{
    int status = EXIT_SUCCESS;

    if (ita->capacity < size)
    {
        unsigned int new_capacity = size * ITA_ALLOC_MULTIPLIER;
        struct item *new_data = (struct item *)realloc(ita->data, new_capacity * sizeof(struct item));
        if (new_data != NULL)
        {
            ita->capacity = new_capacity;
            ita->data = new_data;
        }
        else
            status = EXIT_FAILURE;
    }

    return status;
}

int ita_insert(struct item_array *ita, struct item *it)
{
    int status = ita_resize(ita, ita->size + 1);

    if (status == EXIT_SUCCESS)
    {
        ita->data[ita->size] = *it;
        ita->size += 1;
    }

    return status;
}
