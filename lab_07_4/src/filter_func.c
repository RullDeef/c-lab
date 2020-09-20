#include <stdlib.h>
#include "filter_func.h"
#include <assert.h>

float imp__calc_mean(const int *begin, const int *end)
{
    assert(begin != NULL);
    assert(end != NULL);
    assert(begin <= end);

    int sum = 0;
    int count = end - begin;

    while (begin != end)
        sum += *(begin++);

    return count == 0 ? 0.0f : (float)sum / count;
}

int key(int *begin, int *end, int **filtered_begin, int **filtered_end)
{
    // assert(begin != NULL);
    // assert(end != NULL);
    // assert(filtered_begin != NULL);
    // assert(filtered_end != NULL);
    // assert(begin <= end);

    if (begin == NULL || end == NULL || filtered_begin == NULL || filtered_end == NULL)
        return -1;

    if (begin >= end)
        return -2;

    // *filtered_begin = (int *)malloc((end - begin) * sizeof(int));
    *filtered_begin = begin;

    *filtered_end = *filtered_begin;
    float mean = imp__calc_mean(begin, end);

    for (; begin != end; begin++)
        if (*begin > mean)
            *((*filtered_end)++) = *begin;

    if (*filtered_begin == *filtered_end)
        return -3;

    return 0;
}
