#include <stdlib.h>
#include <stdbool.h>

#define __ARRAY_IMPL
#include "array.h"

ARR_API struct array ARR_CALL arr_create(size_t capacity)
{
    struct array arr = { .size = 0U };
    arr.data = capacity == 0U ? NULL : malloc(capacity * sizeof(int));
    arr.capacity = arr.data == NULL ? 0U : capacity;
    return arr;
}

ARR_API void ARR_CALL arr_destroy(struct array *arr)
{
    if (arr->capacity > 0U)
    {
        free(arr->data);
        arr->data = NULL;
        arr->capacity = 0U;
        arr->size = 0U;
    }
}

ARR_API int ARR_CALL arr_resize(struct array *arr, size_t capacity)
{
    int status = EXIT_SUCCESS;

    if (arr->capacity == 0U)
    {
        arr->data = malloc(capacity * sizeof(int));
        if (arr->data != NULL)
            arr->capacity = capacity;
        else
            status = EXIT_FAILURE;
    }
    else
    {
        int *new_data = realloc(arr->data, capacity * sizeof(int));
        if (new_data != NULL)
        {
            arr->data = new_data;
            arr->capacity = capacity;
        }
        else
            status = EXIT_FAILURE;
    }

    return status;
}

ARR_API int ARR_CALL arr_set(struct array *arr, size_t index, int value)
{
    int status = EXIT_SUCCESS;

    if (index < arr->capacity)
    {
        arr->data[index] = value;
        arr->size = index + 1 > arr->size ? index + 1 : arr->size;
    }
    else
        status = EXIT_FAILURE;

    return status;
}

ARR_API int ARR_CALL arr_get(struct array *arr, size_t index, int *value)
{
    int status = EXIT_SUCCESS;

    if (index < arr->size)
        *value = arr->data[index];
    else
        status = EXIT_FAILURE;

    return status;
}

ARR_API int ARR_CALL arr_push_back(struct array *arr, int data)
{
    int status = EXIT_SUCCESS;

    if (arr->size == arr->capacity)
        status = arr_resize(arr, arr->capacity * ARRAY_REALLOC_MULTIPLIER);

    if (status == EXIT_SUCCESS)
    {
        arr->data[arr->size] = data;
        arr->size++;
    }

    return status;
}

ARR_API int ARR_CALL arr_pop_back(struct array *arr, int *data)
{
    int status = arr->size == 0U ? EXIT_FAILURE : EXIT_SUCCESS;

    if (arr->size > 0U)
    {
        arr->size--;
        *data = arr->data[arr->size];
    }

    return status;
}

ARR_API int ARR_CALL arr_fscanf(FILE *file, struct array *arr)
{
    int status = EXIT_SUCCESS;

    int number;
    while (status == EXIT_SUCCESS && fscanf(file, "%d", &number) == 1)
        status = arr_push_back(arr, number);

    if (status != EXIT_SUCCESS)
        arr_destroy(arr);

    return status;
}

ARR_API int ARR_CALL arr_fprintf(FILE *file, struct array arr)
{
    int status = EXIT_SUCCESS;

    for (size_t i = 0U; status == EXIT_SUCCESS && i < arr.size; i++)
        if (fprintf(file, "%s%d", i == 0U ? "" : " ", arr.data[i]) < 0)
            status = EXIT_FAILURE;

    if (status == EXIT_SUCCESS)
        fprintf(file, "\n");

    return status;
}

ARR_API void ARR_CALL arr_sort(struct array arr)
{
    for (size_t i = 0U; i + 1U < arr.size; i++)
    {
        for (size_t j = 0U; j + 1U + i < arr.size; j++)
        {
            if (arr.data[j] > arr.data[j + 1U])
            {
                int temp = arr.data[j];
                arr.data[j] = arr.data[j + 1U];
                arr.data[j + 1U] = temp;
            }
        }
    }
}

static float __mean(struct array arr)
{
    float mean = 0.0f;

    for (size_t i = 0U; i < arr.size; i++)
        mean += arr.data[i];

    return arr.size == 0U ? 0.0f : mean / arr.size;
}

static bool __all_same(struct array arr)
{
    bool same = true;

    for (size_t i = 0U; same && i + 1U < arr.size; i++)
        same = arr.data[i] == arr.data[i + 1U];

    return same;
}

ARR_API int ARR_CALL arr_filter(struct array arr_in, struct array *arr_out)
{
    int status = EXIT_FAILURE;

    if (arr_in.size > 0U && !__all_same(arr_in))
    {
        *arr_out = arr_create(arr_in.size);
        if (arr_out->capacity != 0U)
        {
            float mean = __mean(arr_in);

            for (size_t i = 0U; i < arr_in.size; i++)
                if (arr_in.data[i] > mean)
                    arr_push_back(arr_out, arr_in.data[i]);

            status = EXIT_SUCCESS;
        }
    }

    return status;
}
