#include <stdio.h>


#define MAX_CAPACITY 10


typedef enum
{
    exit_success,
    exit_failure,
    invalid_array_length
} status_code_t;

typedef int *int_ptr_t;


status_code_t input(int_ptr_t *begin, int_ptr_t *end);
int minfunc(int *begin, int *end);


int main(void)
{
    int array[MAX_CAPACITY];
    int *begin = array;
    int *end = array;

    if (input(&begin, &end))
    {
        printf("incorrect input\n");
        return exit_failure;
    }

    int sum = minfunc(begin, end);

    printf("%d\n", sum);

    return exit_success;
}


status_code_t input(int_ptr_t *begin, int_ptr_t *end)
{
    int n;

    if (scanf("%d", &n) != 1)
        return exit_failure;

    if (n < 2 || n > MAX_CAPACITY)
        return invalid_array_length;

    while (n-- > 0)
        if (scanf("%d", (*end)++) != 1)
            return exit_failure;

    return exit_success;
}


int minfunc(int *begin, int *end)
{
    int prev = *begin;
    int curr = *++begin;
    int min = prev * curr;

    while (begin != end)
    {
        prev = curr;
        curr = *begin++;

        if (prev * curr < min)
            min = prev * curr;
    }

    return min;
}