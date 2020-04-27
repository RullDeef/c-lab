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
    else
    {
        remove_repititions(word_array, &size);
        sort_lex(word_array, size);
        output(word_array, size);
    }

    return result;
}