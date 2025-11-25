#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Size: ");
    }
    while (height < 1 || height > 8);

    int lenghts = height;

    for (int n = 0; n < height; n++)
    {
        for (int m = 0; m < lenghts - 1; m++)
        {
            printf(" ");
        }
        lenghts--;
        for (int p = height; p > lenghts; p--)
        {
            printf("#");
        }
        printf("  ");

        for (int q = height; q > lenghts; q--)
        {
            printf("#");
        }
        printf("\n");
    }
}
