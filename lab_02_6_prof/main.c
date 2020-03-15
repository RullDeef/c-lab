#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
  $ build

  $ main.exe
    with []: 20.192000 sec.
    with *:  20.162000 sec.
*/

#define MAX_CAPACITY 200000
#define ITERATIONS 2000

int process_1(int array[], const int n);
int process_2(int array[], const int n);
int process_3(int *begin, const int *const end);

int main(void)
{
    int array[MAX_CAPACITY];
    clock_t start, end;
    double elapsed;

    for (int i = 0; i < MAX_CAPACITY; i++)
        array[i] = (i * i * i + 2 * i * i + 4) % 1000;

    // first way
    start = clock();

    for (int i = 0; i < ITERATIONS; i++)
      process_1(array, MAX_CAPACITY);

    end = clock();

    elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("with []:   %lf sec.\n", elapsed);

    
    for (int i = 0; i < MAX_CAPACITY; i++)
        array[i] = (i * i * i + 2 * i * i + 4) % 1000;

    // second way
    start = clock();

    for (int i = 0; i < ITERATIONS; i++)
        process_2(array, MAX_CAPACITY);

    end = clock();

    elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("with *:    %lf sec.\n", elapsed);

    // third way
    start = clock();

    for (int i = 0; i < ITERATIONS; i++)
        process_3(array, array + MAX_CAPACITY);

    end = clock();

    elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("with ptr:  %lf sec.\n", elapsed);

    return EXIT_SUCCESS;
}

int process_1(int array[], const int n)
{
    int prev = array[0];
    int curr = array[1];
    int min = prev * curr;

    for (int i = 2; i < n; i++)
    {
        prev = curr;
        curr = array[i];

        if (prev * curr < min)
            min = prev * curr;
    }

    return min;
}

int process_2(int array[], const int n)
{
    int prev = array[0];
    int curr = array[1];
    int min = prev * curr;

    for (int i = 2; i < n; i++)
    {
        prev = curr;
        curr = *(array + i);

        if (prev * curr < min)
            min = prev * curr;
    }

    return min;
}

int process_3(int *begin, const int *const end)
{
    int prev = *begin++;
    int curr = *begin;
    int min = prev * curr;

    while (begin++ != end)
    {
        prev = curr;
        curr = *begin;

        if (prev * curr < min)
            min = prev * curr;
    }

    return min;
}