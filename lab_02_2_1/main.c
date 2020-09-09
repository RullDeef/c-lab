#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdbool.h>


#define MAX_CAPACITY 10
typedef int array_t[MAX_CAPACITY];


typedef enum
{
    exit_success,
    invalid_input,
    invalid_array_length,
    invalid_array_struct
} status_code_t;


status_code_t input(array_t array, size_t *size);
double get_neg_average(array_t array, size_t size);
void print_array(array_t array, size_t size);
void print_err_msg(status_code_t status_code);


int main(void)
{
    array_t array;
    size_t size;
    
    double average;

    status_code_t result = exit_success;
    
    if ((result = input(array, &size)) != exit_success)
    {
        print_err_msg(result);
    }
    else
    {
        average = get_neg_average(array, size);
        printf("%lf", average);
    }

    return result;
}


status_code_t input(array_t array, size_t *size)
{
    status_code_t result = exit_success;

    if (scanf("%zu", size) != 1)
    {
        result = invalid_input;
    }
    else if (*size <= 0 || *size > MAX_CAPACITY)
    {
        result = invalid_array_length;
    }
    else
    {
        bool has_neg_num = false;

        for (int i = 0; i < *size; i++)
        {
            if (scanf("%d", &array[i]) != 1)
            {
                result = invalid_input;
                break;
            }

            has_neg_num = has_neg_num || array[i] < 0;
        }

        if (result == exit_success && !has_neg_num)
            result = invalid_array_struct;
    }
    
    return result;
}


double get_neg_average(array_t array, size_t size)
{
    int amount = 0;
    int total = 0;
    
    for (int i = 0; i < size; i++)
    {
        if (array[i] < 0)
        {
            total += array[i];
            amount++;
        }
    }
    
    return (double)total / amount;
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
    else if (status_code == invalid_array_struct)
    {
        printf("incorrect array struct.\n");
    }
}
