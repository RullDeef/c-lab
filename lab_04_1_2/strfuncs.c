#include <stdio.h>
#include <stdlib.h>
#include "strfuncs.h"

#define WORD_DELIM " .,?!:;-"

int input_str(char *str)
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

size_t my_strlen(const char *str)
{
    size_t len = 0;
    while (str[len] != '\0')
        len++;

    return len;
}

size_t my_strspn(const char *str1, const char *str2)
{
    size_t len1 = my_strlen(str1);
    size_t len2 = my_strlen(str2);

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
    size_t len1 = my_strlen(str1);
    size_t len2 = my_strlen(str2);

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

void my_strncpy(char *dest, const char *src, const size_t len)
{
    for (size_t i = 0; i < len; i++)
        dest[i] = src[i];
}

int my_strcmp(const char *str1, const char *str2)
{
    while (*str1 != '\0' && *str2 != '\0' && *str1 == *str2)
    {
        str1++;
        str2++;
    }

    return *str1 - *str2;
}

int split_str(const char *str, word_array_t word_array, size_t *size)
{
    size_t str_len = my_strlen(str);
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

        my_strncpy(word_array[word_i], str + str_i, word_len);
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
    size_t len = my_strlen(src);
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

void remove_repititions(word_array_t word_array, size_t *size)
{
    for (size_t i = 0; i + 1 < *size; i++)
    {
        for (size_t j = i + 1; j < *size; j++)
        {
            if (my_strcmp(word_array[i], word_array[j]) == 0)
            {
                for (size_t k = j + 1; k < *size; k++)
                    word_assign(word_array[k - 1], word_array[k]);
                --j;
                --*size;
                continue;
            }
        }
    }
}

void sort_lex(word_array_t word_array, const size_t size)
{
    for (size_t i = 0; i + 1 < size; i++)
        for (size_t j = i + 1; j < size; j++)
            if (my_strcmp(word_array[i], word_array[j]) > 0)
                swap_word(word_array[i], word_array[j]);
}

void output(word_array_t word_array, const size_t size)
{
    printf("Result:");
    printf("%s", word_array[0]);

    for (size_t i = 1; i < size; i++)
        printf(" %s", word_array[i]);
}