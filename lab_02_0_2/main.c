#include <stdio.h>


#define MAX_CAPACITY 10


typedef enum
{
    exit_success,
    exit_failure,
    invalid_array_length
} status_code_t;


status_code_t input(int *n, int array[]);
double get_average(const int array[], const int n);
status_code_t filter(int array[], const int n, double average, int *m);
void print_array(const int *array, const int n);


int main(void)
{
    int n;
    int array[MAX_CAPACITY];
    
    double average;
    int m;
    
    if (input(&n, array))
    {
        printf("incorrect input\n");
        return exit_failure;
    }
    
    // find average
    average = get_average(array, n);

    // filter all numbers in array
    if (filter(array, n, average, &m))
    {
        printf("empty array\n");
        return exit_failure;
    }

    print_array(array, m);
    
    return exit_success;
}


status_code_t input(int *n, int array[])
{
    if (scanf("%d", n) != 1)
        return exit_failure;
    
    if (*n <= 0 || *n > MAX_CAPACITY)
        return invalid_array_length;
    
    for (int i = 0; i < *n; i++)
        if (scanf("%d", array + i) != 1)
            return exit_failure;
    
    return exit_success;
}


double get_average(const int array[], const int n)
{
    double result = 0.0;
    
    for (int i = 0; i < n; i++)
        result += (double)array[i];
    
    return result / n;
}


status_code_t filter(int array[], const int n, double average, int *m)
{
    *m = 0;
    
    for (int i = 0; i < n; i++)
        if (array[i] > average)
            array[(*m)++] = array[i];
    
    if (*m == 0)
        return exit_failure;
    
    return exit_success;
}


void print_array(const int *array, const int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d", array[i]);
        if (i != n - 1)
            printf(" ");
        else
            printf("\n");
    }
}