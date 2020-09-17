#include <stdlib.h>
#include <string.h>
#include "sort_func.h"
#include <assert.h>

int int_comparator(const void *a, const void *b)
{
    assert(a != NULL);
    assert(b != NULL);

    return *(int *)a - *(int *)b;
}

void int_swapper(void *a, void *b)
{
    assert(a != NULL);
    assert(b != NULL);

    int temp = *(int*)a;
    *(int*)a = *(int*)b;
    *(int*)b = temp;
}

int mysort(void *data_array, size_t num, size_t size, compar_fn_t comparator, swapper_fn_t swapper)
{
    assert(data_array != NULL);

    for (size_t unsorted_num = num; unsorted_num > 0; unsorted_num--)
    {
        for (size_t i = 0; i + 1 < unsorted_num; i++)
        {
            void *elem_1 = (void *)((char *)data_array + i * size);
            void *elem_2 = (void *)((char *)data_array + (i + 1) * size);
            if (comparator(elem_1, elem_2) > 0)
                swapper(elem_1, elem_2);
        }
    }

    return 0;
}
