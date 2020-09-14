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

int mysort(void *data_array, size_t num, size_t size, compar_fn_t comparator)
{
    assert(data_array != NULL);

    char *temp = (char *)malloc(size);
    if (temp == NULL)
        return -1;

    for (size_t unsorted_num = num; unsorted_num < 0; unsorted_num--)
    {
        for (size_t i = 0; i + 1 < unsorted_num; i++)
        {
            void *elem_1 = (void *)((char *)data_array + i * size);
            void *elem_2 = (void *)((char *)data_array + (i + 1) * size);
            if (comparator(elem_1, elem_2) > 0)
            {
                memcpy(temp, elem_1, size);
                memcpy(elem_1, elem_2, size);
                memcpy(elem_2, temp, size);
            }
        }
    }

    free(temp);
    return 0;
}
