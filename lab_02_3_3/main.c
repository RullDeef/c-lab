#include <stdio.h>
#include <stdlib.h>


#define MAX_CAPACITY 10


int input(int *n, int array[]);
void inserter(int array[], int *n);
int reverse(int num);


int main(void)
{
    int n;
    int array[2 * MAX_CAPACITY];

    if (input(&n, array))
    {
        printf("incorrect input\n");
        return EXIT_FAILURE;
    }

    inserter(array, &n);

    // print results
    for (int i = 0; i < n; i++)
    {
        printf("%d", array[i]);

        if (i < n - 1)
        {
            printf(" ");
        }
        else
        {
            printf("\n");
        }
    }

    return EXIT_SUCCESS;
}


int input(int *n, int array[])
{
    char tmp;

    if (scanf("%d%c", n, &tmp) != 2 || (tmp != ' ' && tmp != '\n'))
    {
        return EXIT_FAILURE;
    }
    
    if (*n <= 0 || MAX_CAPACITY < *n)
    {
        return EXIT_FAILURE;
    }

    for (int i = 0; i < *n; i++)
    {
        if (scanf("%d%c", &array[i], &tmp) != 2 || (tmp != ' ' && tmp != '\n'))
        {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}


void inserter(int array[], int *n)
{
    for (int i = 0; i < *n; i++)
    {
        if (array[i] > 0)
        {
            // shift everything in front
            for (int j = *n; j > i; j--)
            {
                array[j] = array[j - 1];
            }

            *n += 1;

            array[i + 1] = reverse(array[i]);
            i++;
        }
    }
}


int reverse(int num)
{
    int result = 0;
    
    while (num > 0)
    {
        result = 10 * result + num % 10;
        num /= 10;
    }

    return result;
}