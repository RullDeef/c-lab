#include <stdio.h>


#define MAX_CAPACITY 10


typedef enum
{
    exit_success,
    exit_failure,
    invalid_array_length
} status_code_t;


status_code_t input(int *n, int array[]);
status_code_t check_neg_num(int array[], const int n);
double get_average(int array[], const int n);


int main(void)
{
    int array[MAX_CAPACITY];
    int n;
    
    double average;
    
    if (input(&n, array))
    {
        printf("incorrect input\n");
        return exit_failure;
    }
    
    // check if there is any negative number
    if (check_neg_num(array, n))
    {
        printf("no negative numbers\n");
        return exit_failure;
    }
    
    // count average among negatives
    average = get_average(array, n);
    printf("%f", average);
    
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


status_code_t check_neg_num(int array[], const int n)
{
    for (int i = 0; i < n; i++)
        if (array[i] < 0)
            return exit_success;
    
    return exit_failure;
}


double get_average(int array[], const int n)
{
    int amount = 0;
    int total = 0;
    
    for (int i = 0; i < n; i++)
    {
        if (array[i] < 0)
        {
            total += array[i];
            amount++;
        }
    }
    
    return (double)total / amount;
}
