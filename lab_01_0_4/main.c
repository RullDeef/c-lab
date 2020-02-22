#include <stdio.h>

int main()
{
    int S;
    int amount;

    printf("Enter integer: ");
    scanf("%d", &S);

    amount = S < 45 ? 0 : (S - 45) / 25 + 1;

    printf("Amount = %d", amount);

    return 0;
}
