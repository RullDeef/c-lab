#include <stdlib.h>
#include <assert.h>
#include "status_codes.h"
#include "filter_func.h"

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

int imp__count_proper_amount(const int *begin, const int *end, float mean)
{
    assert(begin != NULL);
    assert(end != NULL);
    assert(begin < end);

    int proper_amount = 0;
    for (; begin != end; begin++)
        if (*begin > mean)
            proper_amount++;
    
    return proper_amount;
}

void imp__copy_proper_values(const int *begin, const int *end, int **filtered_begin, int **filtered_end, float mean)
{
    *filtered_end = *filtered_begin;

    for (; begin != end; begin++)
        if (*begin > mean)
            *((*filtered_end)++) = *begin;
}

int key(const int *begin, const int *end, int **filtered_begin, int **filtered_end)
{
    if (begin == NULL || end == NULL || filtered_begin == NULL || filtered_end == NULL)
        return INVALID_ARGS;

    if (begin >= end)
        return INVALID_PTRS;
    
    float mean = imp__calc_mean(begin, end);
    int proper_amount = imp__count_proper_amount(begin, end, mean);

    if (proper_amount == 0)
        return INVALID_ELEMENTS_AMOUNT;

    *filtered_begin = (int*)malloc(proper_amount * sizeof(int));
    if (*filtered_begin == NULL)
        return BAD_ALLOC;
    
    imp__copy_proper_values(begin, end, filtered_begin, filtered_end, mean);
    return SUCCESS;
}
