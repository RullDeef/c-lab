#include <stdio.h>


#define MAX_CAPACITY 10


typedef enum
{
    exit_success,
    invalid_input,
    invalid_array_length
} status_code_t;


status_code_t input(int **end);
int min_neighbour_prod(int *begin, int *end);
void print_err_msg(status_code_t status_code);


int main(void)
{
    int array[MAX_CAPACITY];
    int *begin = array;
    int *end = array;

    status_code_t result = exit_success;

    if ((result = input(&end)) != exit_success)
    {
        print_err_msg(result);
    }
    else
    {
        int prod = min_neighbour_prod(begin, end);
        printf("%d\n", prod);
    }

    return result;
}


status_code_t input(int **end)
{
    int size;

    status_code_t result = exit_success;

    if (scanf("%d", &size) != 1)
    {
        result = invalid_input;
    }
    else if (size < 2 || size > MAX_CAPACITY)
    {
        result = invalid_array_length;
    }
    else
    {
        while (size-- > 0)
        {
            if (scanf("%d", (*end)++) != 1)
            {
                result = invalid_input;
                break;
            }
        }
    }

    return result;
}


int min_neighbour_prod(int *begin, int *end)
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


void print_err_msg(status_code_t status_code)
{
    if (status_code == invalid_input)
    {
        printf("incorrect input.\n");
    }
    else if (status_code == invalid_array_length)
    {
        printf("incorrect array length.\n");
    }
}