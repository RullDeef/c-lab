/*

Классы эквивалентностей:

1. ввод нечисловых данных
2. ненатуральное число в качестве размера массива
3. превышение максимального размера массива
4. отрицательное число кратности
5. не удаление ни одного элемента массива
6. удаление некоторых элементов массива

*/

#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdbool.h>

#define MAX_ARRAY_SIZE 20

typedef int array_t[MAX_ARRAY_SIZE];

typedef enum
{
    EXIT_SUCCESS = 0,
    INVALID_INPUT,
    INVALID_ARRAY_SIZE,
    INVALID_ARRAY_ELEMENT,
    INVALID_NUMBER_TO_REMOVE
} status_code_t;

status_code_t input_size(size_t *size);
status_code_t input_array(array_t array, const size_t size);
status_code_t input_num_to_remove(int *num_to_remove);
void delete_array_element(array_t array, size_t *size, size_t i);
void remove_every_2nd_num(array_t array, size_t *size, const int num_to_remove);
void print_array(const array_t array, const size_t size);

int main(void)
{
    array_t array;
    size_t size;
    int num_to_remove;

    status_code_t result = EXIT_SUCCESS;

    if ((result = input_size(&size)) != EXIT_SUCCESS)
        ; // skip
    
    else if ((result = input_array(array, size)) != EXIT_SUCCESS)
        ; // skip

    else if ((result = input_num_to_remove(&num_to_remove)) != EXIT_SUCCESS)
        ; // skip

    else
    {
        remove_every_2nd_num(array, &size, num_to_remove);
        print_array(array, size);
    }

    return result;
}

status_code_t input_size(size_t *size)
{
    status_code_t result = EXIT_SUCCESS;

    if (scanf("%zu", size) != 1)
        result = INVALID_INPUT;
    
    else if (*size <= 0 || *size > MAX_ARRAY_SIZE)
        result = INVALID_ARRAY_SIZE;

    return result;
}

status_code_t input_array(array_t array, const size_t size)
{
    status_code_t result = EXIT_SUCCESS;

    for (size_t i = 0; i < size; i++)
    {
        if (scanf("%d", &array[i]) != 1)
        {
            result = INVALID_ARRAY_ELEMENT;
            break;
        }
    }

    return result;
}

status_code_t input_num_to_remove(int *num_to_remove)
{
    status_code_t result = EXIT_SUCCESS;

    if (scanf("%d", num_to_remove) != 1)
        result = INVALID_NUMBER_TO_REMOVE;
    
    else if (*num_to_remove <= 0)
        result = INVALID_NUMBER_TO_REMOVE;

    return result;
}

void delete_array_element(array_t array, size_t *size, size_t i)
{
    while (i + 1 < *size)
    {
        array[i] = array[i + 1];
        i++;
    }
    *size -= 1;
}

void remove_every_2nd_num(array_t array, size_t *size, const int num_to_remove)
{
    bool is_2nd = false;

    for (size_t i = 0; i < *size; i++)
    {
        if (array[i] == num_to_remove)
        {
            if (is_2nd)
            {
                delete_array_element(array, size, i);
                i--;
            }
            
            is_2nd = !is_2nd;
        }
    }
}

void print_array(const array_t array, const size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        if (i == 0)
            printf("%d", array[i]);
        else
            printf(" %d", array[i]);
    }
}
