#include <stdio.h>

int main()
{
    int s;
    int amount;

    printf("Enter integer: ");
    scanf("%d", &s);

    amount = s < 45 ? 0 : (s - 45) / 25 + 1;

    printf("Amount = %d", amount);

    return 0;
}
