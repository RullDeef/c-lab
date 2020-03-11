#include <stdio.h>
#include <stdlib.h>


#define MAX_CAPACITY 10


int input(int *n, int array[]);
int has_neg_num(int array[], const int n);
double get_average(int array[], const int n);


int main(void)
{
    int array[MAX_CAPACITY];
    int n;
    
    double average;
    
    if (input(&n, array))
    {
        printf("incorrect input\n");
        return EXIT_FAILURE;
    }
    
    // check if there is any negative number
    if (!has_neg_num(array, n))
    {
        printf("no negative numbers\n");
        return EXIT_FAILURE;
    }
    
    // count average among negatives
    average = get_average(array, n);
    printf("%f", average);
    
    return EXIT_SUCCESS;
}


int input(int *n, int array[])
{
    if (scanf("%d", n) != 1)
        return EXIT_FAILURE;

    if (*n <= 0 || *n > MAX_CAPACITY)
        return EXIT_FAILURE;
    
    for (int i = 0; i < *n; i++)
        if (scanf("%d", array + sizeof(int) * i) != 1)
            return EXIT_FAILURE;
    
    return EXIT_SUCCESS;
}


int has_neg_num(int array[], const int n)
{
    int result = 0;
    
    for (int i = 0; i < n; i++)
    {
        if (array[i] < 0)
        {
            result = 1;
            break;
        }
    }
    
    return result;
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
