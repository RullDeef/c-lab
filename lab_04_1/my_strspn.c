#include "my_strspn.h"

size_t my_strlen(const char *str)
{
    size_t len = 0;
    while (str[len] != '\0') len++;
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