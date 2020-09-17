#ifndef SORT_FUNC_H
#define SORT_FUNC_H

typedef int (*compar_fn_t)(const void *, const void *);
typedef void (*swapper_fn_t)(void *, void *);

int int_comparator(const void *a, const void *b);
void int_swapper(void *a, void *b);

int mysort(void *data_array, size_t num, size_t size, compar_fn_t comparator, swapper_fn_t swapper);

#endif // SORT_FUNC_H
