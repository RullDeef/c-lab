// допущение : повторяющиеся слова в строке не вызывают ошибку
// допущение : в строке как минимум одно слово
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strfuncs.h"

int input_str(input_str_t str)
{
    scanf("%[^\n]s", str);

    if (strlen(str) > 0)
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
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

    input_str(str);
    split_str(str, word_array, &size);

    for (size_t i = 0; i < size; i++)
        remove_repititions(word_array[i]);
    
    reverse(word_array, size);
    output(word_array, size);

    return EXIT_SUCCESS;
}