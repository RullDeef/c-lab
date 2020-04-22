#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strfuncs.h"

size_t my_strspn(const char *str1, const char *str2)
{
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    size_t i, j;
    int flag;

    for (i = 0; i < len1; i++)
    {
        flag = 0;
        for (j = 0; j < len2; j++)
        {
            if (str1[i] == str2[j])
            {
                flag = 1;
                break;
            }
        }

        if (flag == 0)
            break;
    }

    return i;
}

size_t my_strcspn(const char *str1, const char *str2)
{
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    size_t i, j;
    int flag;

    for (i = 0; i < len1; i++)
    {
        flag = 1;
        for (j = 0; j < len2; j++)
        {
            if (str1[i] == str2[j])
            {
                flag = 0;
                break;
            }
        }

        if (flag == 0)
            break;
    }

    return i;
}

int split_str(const input_str_t str, word_array_t word_array, size_t *size)
{
    size_t str_len = strlen(str);
    size_t word_len;
    size_t str_i = 0;
    size_t word_i = 0;

    int result = EXIT_SUCCESS;

    while (str_i < str_len)
    {
        str_i += my_strspn(str + str_i, WORD_DELIM);
        if (str_i == str_len)
            break;

        word_len = my_strcspn(str + str_i, WORD_DELIM);

        if (word_len >= MAX_WORD_LEN)
        {
            result = EXIT_FAILURE;
            break;
        }

        strncpy(word_array[word_i], str + str_i, word_len);
        word_array[word_i][word_len] = '\0';
        str_i += word_len;
        word_i++;
    }

    *size = word_i;
    if (*size == 0)
        result = EXIT_FAILURE;
    
    return result;
}

void word_assign(word_t dest, const word_t src)
{
    size_t len = strlen(src);
    for (size_t i = 0; i < len; i++)
        dest[i] = src[i];
    dest[len] = '\0';
}

void swap_word(word_t word1, word_t word2)
{
    word_t temp;
    word_assign(temp, word1);
    word_assign(word1, word2);
    word_assign(word2, temp);
}
