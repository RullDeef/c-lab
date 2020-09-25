#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "status_codes.h"
#include "sort_func.h"

int int_comparator(const void *a, const void *b)
{
    assert(a != NULL);
    assert(b != NULL);

    return *(int *)a - *(int *)b;
}

void imp__swapper(void *a, void *b, size_t size)
{
    assert(a != NULL);
    assert(b != NULL);
    assert(size > 0);

    char temp;
    for (size_t i = 0; i < size; i++)
    {
        temp = *((char*)a + i);
        *((char*)a + i) = *((char*)b + i);
        *((char*)b + i) = temp;
    }
}

int mysort(void *data_array, int num, int size, compar_fn_t comparator)
{
    if (data_array == NULL)
        return invalid_args;
    
    if (num <= 0)
        return invalid_elements_amount;

    if (size <= 0)
        return invalid_elements_size;

    for (size_t unsorted_num = num; unsorted_num > 0; unsorted_num--)
    {
        for (size_t i = 0; i + 1 < unsorted_num; i++)
        {
            void *elem_1 = (void *)((char *)data_array + i * size);
            void *elem_2 = (void *)((char *)data_array + (i + 1) * size);
            if (comparator(elem_1, elem_2) > 0)
                imp__swapper(elem_1, elem_2, size);
        }
    }

    return success;
}
