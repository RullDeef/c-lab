#include <stdio.h>
#include <stdlib.h>


// #define MAX_CAPACITY 10000


enum
{
    INVALID_ARRAY_LENGTH = 1,
    INVALID_ARRAY_ELEMENT
};


int input(int *n, int array[]);
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


int input(int *n, int array[])
{
    if (scanf("%d", n) != 1)
        return INVALID_ARRAY_LENGTH;

    if (*n <= 0 || *n > MAX_CAPACITY)
        return INVALID_ARRAY_LENGTH;

    for (int i = 0; i < *n; i++)
        if (scanf("%d", array + i) != 1)
            return INVALID_ARRAY_ELEMENT;

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