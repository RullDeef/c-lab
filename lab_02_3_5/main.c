#include <stdio.h>
#include <stdlib.h>

#define MAX_CAPACITY 10
#define MEMORY_ERROR -2

int input(int **begin, int **end);
int cumsum(int *begin, int *end);

int main(void)
{
    int *begin, *end;

    if (input(&begin, &end))
    {
        printf("incorrect input\n");
        return EXIT_FAILURE;
    }

    int sum = cumsum(begin, end);

    printf("%d\n", sum);

    free(begin);

    return EXIT_SUCCESS;
}

int input(int **begin, int **end)
{
    int n;

    if (scanf("%d", &n) != 1)
    {
        return EXIT_FAILURE;
    }

    if (n < 0 || n > MAX_CAPACITY)
    {
        return EXIT_FAILURE;
    }

    *begin = calloc(n, sizeof(int));

    if (*begin == NULL)
    {
        return MEMORY_ERROR;
    }

    *end = *begin;

    while (n-- > 0)
    {
        if (scanf("%d", *end) != 1)
        {
            free(*begin);
            return EXIT_FAILURE;
        }

        *end += 1;
    }

    return EXIT_SUCCESS;
}

int cumsum(int *begin, int *end)
{
    int current = *begin;
    int sum = current;

    if (current > 0)
    {
        for (begin++; begin != end; begin++)
        {
            current *= *begin;
            sum += current;

            if (*begin < 0)
            {
                break;
            }
        }
    }

    return sum;
}