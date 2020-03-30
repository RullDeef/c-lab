#include <stdio.h>


#define MAX_CAPACITY 10


typedef enum
{
    exit_success,
    exit_failure,
    invalid_array_length
} status_code_t;


status_code_t input(int *n, int array[]);
void inserter(int array[], int *n);
int reverse(int num);
void output(int array[], const int n);


int main(void)
{
    int n;
    int array[2 * MAX_CAPACITY];

    if (input(&n, array))
    {
        printf("incorrect input\n");
        return exit_failure;
    }

    inserter(array, &n);
    output(array, n);

    return exit_success;
}


status_code_t input(int *n, int array[])
{
    if (scanf("%d", n) != 1)
        return exit_failure;
    
    if (*n <= 0 || *n > MAX_CAPACITY)
        return invalid_array_length;

    for (int i = 0; i < *n; i++)
        if (scanf("%d", &array[i]) != 1)
            return exit_failure;

    return exit_success;
}


void inserter(int array[], int *n)
{
    for (int i = 0; i < *n; i++)
    {
        if (array[i] > 0)
        {
            for (int j = *n; j > i; j--)
                array[j] = array[j - 1];

            *n += 1;

            array[i + 1] = reverse(array[i]);
            i++;
        }
    }
}


int reverse(int num)
{
    int result = 0;
    
    while (num > 0)
    {
        result = 10 * result + num % 10;
        num /= 10;
    }

    return result;
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