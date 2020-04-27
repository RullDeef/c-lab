#include <stdio.h>
#include <stdlib.h>
#include "strfuncs.h"

int main(void)
{
    char str[MAX_STR_LEN];
    word_array_t word_array;
    size_t size;

    int result = EXIT_SUCCESS;

    if (input_str(str) != EXIT_SUCCESS)
        result = EXIT_FAILURE;
    else if (split_str(str, word_array, &size) != EXIT_SUCCESS)
        result = EXIT_FAILURE;
    else if (remove_same_as_last(word_array, &size) != EXIT_SUCCESS)
        result = EXIT_FAILURE;
    else
    {
        for (size_t i = 0; i < size; i++)
            remove_repititions(word_array[i]);
        
        reverse(word_array, size);
        output(word_array, size);
    }

    return result;
}