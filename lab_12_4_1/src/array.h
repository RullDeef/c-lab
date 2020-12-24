#ifndef __ARRAY_H_
#define __ARRAY_H_

#ifdef ARRAY_DYNAMIC // dynamic linking
#   if defined(WIN32)
#       ifdef __ARRAY_IMPL
#           define ARR_API __declspec(dllexport)
#       else
#           define ARR_API __declspec(dllimport)
#       endif
#       define ARR_CALL __cdecl
#   elif defined(LINUX)
#       define ARR_API
#       define ARR_CALL
#   endif
#else // static linking
#   define ARR_API
#   define ARR_CALL
#endif

#include <stdio.h>

#define ARRAY_INITIAL_CAPACITY 10U
#define ARRAY_REALLOC_MULTIPLIER 1.5f

struct array
{
    size_t capacity;
    size_t size;
    int *data;
};

ARR_API struct array ARR_CALL arr_create(size_t capacity);
ARR_API void ARR_CALL arr_destroy(struct array *arr);

ARR_API int ARR_CALL arr_resize(struct array *arr, size_t capacity);

ARR_API int ARR_CALL arr_set(struct array *arr, size_t index, int value);
ARR_API int ARR_CALL arr_get(struct array *arr, size_t index, int *value);

ARR_API int ARR_CALL arr_push_back(struct array *arr, int data);
ARR_API int ARR_CALL arr_pop_back(struct array *arr, int *data);

ARR_API int ARR_CALL arr_fscanf(FILE *file, struct array *arr);
ARR_API int ARR_CALL arr_fprintf(FILE *file, struct array arr);

ARR_API void ARR_CALL arr_sort(struct array arr);
ARR_API int ARR_CALL arr_filter(struct array arr_in, struct array *arr_out);

#endif // __ARRAY_H_
