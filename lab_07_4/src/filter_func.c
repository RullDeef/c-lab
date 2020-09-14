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

int key(const int *begin, const int *end, int **filtered_begin, int **filtered_end)
{
    assert(begin != NULL);
    assert(end != NULL);
    assert(filtered_begin != NULL);
    assert(filtered_end != NULL);
    assert(begin <= end);

    if (begin == end)
        return -1;

    *filtered_begin = (int *)malloc((end - begin) * sizeof(int));

    if (*filtered_begin == NULL)
        return -2;

    *filtered_end = *filtered_begin;
    float mean = imp__calc_mean(begin, end);

    for (; begin != end; begin++)
        if (*begin > mean)
            *((*filtered_end)++) = *begin;

    if (*filtered_begin == *filtered_end)
        return -3;

    return 0;
}
