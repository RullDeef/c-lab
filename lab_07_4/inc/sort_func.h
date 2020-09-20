#ifndef SORT_FUNC_H
#define SORT_FUNC_H

typedef int (*compar_fn_t)(const void *, const void *);

int int_comparator(const void *a, const void *b);

int mysort(void *data_array, int num, int size, compar_fn_t comparator);

#endif // SORT_FUNC_H
