#include <stdio.h>
#include <stdlib.h>


#define MAX_CAPACITY 10


int input(int *n, int array[]);
double get_average(int array[], const int n);
int filter(int array[], const int n, double average, int *m);


int main(void)
{
    int n;
    int array[MAX_CAPACITY];
    
    double average;
    int m;
    
    if (input(&n, array))
    {
        printf("incorrect input\n");
        return EXIT_FAILURE;
    }
    
    // find average
    average = get_average(array, n);

    // filter all numbers in array
    if (filter(array, n, average, &m))
    {
        printf("empty array\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < m; i++)
    {
        printf("%d", array[i]);
        if (i != m - 1)
        {
            printf(" ");
        }
        else
        {
            printf("\n");
        }
    }
    
    return EXIT_SUCCESS;
}


int input(int *n, int array[])
{
    if (scanf("%d", n) != 1)
    {
        return EXIT_FAILURE;
    }
    
    if (*n <= 0 || *n > MAX_CAPACITY)
    {
        return EXIT_FAILURE;
    }
    
    for (int i = 0; i < *n; i++)
    {
        if (scanf("%d", array + i) != 1)
        {
            return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;
}


double get_average(int array[], const int n)
{
    double result = 0.0;
    
    for (int i = 0; i < n; i++)
    {
        result += (double)array[i];
    }
    
    return result / n;
}


int filter(int array[], const int n, double average, int *m)
{
    *m = 0;
    
    for (int i = 0; i < n; i++)
    {
        if (array[i] > average)
        {
            array[*m] = array[i];
            *m += 1;
        }
    }
    
    if (*m == 0)
    {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
