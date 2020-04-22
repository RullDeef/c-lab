// допущение : повторяющиеся слова в строке не вызывают ошибку
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strfuncs.h"

int input_str(input_str_t str)
{
    size_t i = 0;
    int result = EXIT_SUCCESS;

    while (1)
    {
        char c = str[i] = (char)getchar();
        if (c == '\n' || c == '\0' || c == EOF)
        {
            if (i == 0)
                result = EXIT_FAILURE;
            break;
        }
        else if (++i == MAX_STR_LEN)
        {
            result = EXIT_FAILURE;
            break;
        }
    }

    if (result == EXIT_SUCCESS)
        str[i] = '\0';
    return result;
}

void output(word_array_t word_array, const size_t size)
{
    printf("Result: ");
    printf("%s", word_array[0]);

    for (size_t i = 1; i < size; i++)
        printf(" %s", word_array[i]);
}

int main(void)
{
    input_str_t str;
    word_array_t word_array;
    size_t size;

    int result = EXIT_SUCCESS;

    if ((result = input_str(str)) == EXIT_SUCCESS)
    {
        if ((result = split_str(str, word_array, &size)) == EXIT_SUCCESS)
        {
            size--;
            for (size_t i = 0; i < size; i++)
                remove_repititions(word_array[i]);
            
            reverse(word_array, size);
            output(word_array, size);
        }
    }

    return result;
}