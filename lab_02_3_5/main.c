#include <stdio.h>


#define MAX_CAPACITY 10


typedef enum
{
    EXIT_SUCCESS,
    EXIT_FAILURE,
    INVALID_ARRAY_LENGTH
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
        return EXIT_FAILURE;
    }

    int sum = minfunc(begin, end);

    printf("%d\n", sum);

    return EXIT_SUCCESS;
}


status_code_t input(int_ptr_t *begin, int_ptr_t *end)
{
    int n;

    if (scanf("%d", &n) != 1)
        return EXIT_FAILURE;

    if (n < 2 || n > MAX_CAPACITY)
        return INVALID_ARRAY_LENGTH;

    while (n-- > 0)
        if (scanf("%d", (*end)++) != 1)
            return EXIT_FAILURE;

    return EXIT_SUCCESS;
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