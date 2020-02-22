#include <stdio.h>

int main()
{
    float R1, R2, R3;
    float R;

    printf("Enter three numbers: ");
    scanf("%f %f %f", &R1, &R2, &R3);

    R = R1 * R2 * R3 / (R1 * R2 + R1 * R3 + R2 * R3);

    printf("Total resistance = %.5f", R);

    return 0;
}
