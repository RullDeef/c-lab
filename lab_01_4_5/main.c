#include <stdio.h>
#include <stdbool.h>

bool input_int(int *n)
{
    char c;
    int argc = scanf("%d%c", n, &c);

    return argc != 1 && (argc == 0 || c != '\n');
}


int main()
{
    int n;

    printf("Enter natural n: ");

    if (input_int(&n))
    {
        printf("Incorrect input. Closing program...\n");
        return 1;
    }

    if (n <= 0)
    {
        printf("Number n must be positive. Closing program...\n");
        return 1;
    }

    printf("\nDivisors of n: l");

    for (int i = 2; i <= n; i++)
    {
        while (n % i == 0)
        {
            printf(", %d", i);
            n /= i;
        }
    }

    return 0;
}
