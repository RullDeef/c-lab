#include <stdio.h>
#include <stdlib.h>

#define MAX_CAPACITY 10
#define MEMORY_ERROR -2

int input(int **begin, int **end);
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

int input(int **begin, int **end)
{
    int n;

    if (scanf("%d", &n) != 1)
    {
        return EXIT_FAILURE;
    }

    if (n < 2 || n > MAX_CAPACITY)
    {
        return EXIT_FAILURE;
    }

    while (n-- > 0)
    {
        if (scanf("%d", *end) != 1)
        {
            return EXIT_FAILURE;
        }

        *end += 1;
    }

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