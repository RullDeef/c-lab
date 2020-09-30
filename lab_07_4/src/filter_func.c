#include <stdlib.h>
#include <assert.h>
#include "status_codes.h"
#include "filter_func.h"

/**
 * @brief Подсчитывает среднее значение в числовом массиве
 * 
 * @param begin - указатель на начало массива
 * @param end - указатель на конец массиав (на элемент за последним)
 * 
 * @return среднее арифметическое значение чисел или 0.0f, если массив пуст
 */
float imp__calc_mean(const int *begin, const int *end);

/**
 * @brief Подсчитывает количество чисел в массиве, которые
 * больше среднего арифметического всех чисел в массиве
 * 
 * @param begin - указатель на начало массива
 * @param end - указатель на конец массиав (на элемент за последним)
 * @param mean - среднее арифметическое всех чисел массива
 * 
 * @return proper_amount - кол-во чисел
 */
int imp__count_proper_amount(const int *begin, const int *end, float mean);

/**
 * @brief Копирует числа в выходной массив, если они больше
 * среднего арифметического всех чисел в массиве
 * 
 * @param begin - указатель на начало массива
 * @param end - указатель на конец массиав (на элемент за последним)
 * @param filtered_begin - указатель на начало массива
 * @param filtered_end - указатель на конец массиав (на элемент за последним)
 * @param mean - среднее арифметическое всех чисел в массиве
 */
void imp__copy_proper_values(const int *begin, const int *end, int **filtered_begin, int **filtered_end, float mean);

int key(const int *begin, const int *end, int **filtered_begin, int **filtered_end)
{
    if (begin == NULL || end == NULL || filtered_begin == NULL || filtered_end == NULL)
        return invalid_args;

    if (begin >= end)
        return invalid_ptrs;
    
    float mean = imp__calc_mean(begin, end);
    int proper_amount = imp__count_proper_amount(begin, end, mean);

    if (proper_amount == 0)
        return invalid_elements_amount;

    *filtered_begin = (int*)malloc(proper_amount * sizeof(int));
    if (*filtered_begin == NULL)
        return bad_alloc;
    
    imp__copy_proper_values(begin, end, filtered_begin, filtered_end, mean);
    return success;
}

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
    assert(begin != NULL);
    assert(end != NULL);
    assert(filtered_begin != NULL);
    assert(filtered_end != NULL);
    assert(*filtered_begin != NULL);
    assert(begin <= end);

    *filtered_end = *filtered_begin;

    for (; begin != end; begin++)
        if (*begin > mean)
            *((*filtered_end)++) = *begin;
}
