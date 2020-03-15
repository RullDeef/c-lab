#include <stdio.h>


#define MAX_CAPACITY 10


typedef enum
{
    EXIT_SUCCESS,
    EXIT_FAILURE,
    INVALID_ARRAY_LENGTH
} status_code_t;


status_code_t input(int *n, int array[]);
void sort(int array[], const int n);
void output(int array[], const int n);


int main(void)
{
    int n;
    int array[MAX_CAPACITY];

    if (input(&n, array))
    {
        printf("incorrect input\n");
        return EXIT_FAILURE;
    }

    sort(array, n);
    output(array, n);

    return EXIT_SUCCESS;
}


status_code_t input(int *n, int array[])
{
    if (scanf("%d", n) != 1)
        return EXIT_FAILURE;

    if (*n <= 0 || *n > MAX_CAPACITY)
        return INVALID_ARRAY_LENGTH;

    for (int i = 0; i < *n; i++)
        if (scanf("%d", array + i) != 1)
            return EXIT_FAILURE;

    return EXIT_SUCCESS;
}


void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}


void sort(int array[], const int n)
{
    for (int i = 1; i < n; i++)
        for (int j = i; j > 0; j--)
            if (array[j] < array[j - 1])
                swap(&array[j - 1], &array[j]);
}


void output(int array[], const int n)
{
    for (int i = 0; i < n; i++)
    {
        if (i < n - 1)
            printf("%d ", array[i]);
        else
            printf("%d\n", array[i]);
    }
}