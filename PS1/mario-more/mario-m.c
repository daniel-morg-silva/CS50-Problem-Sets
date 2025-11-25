#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (8 < height || height < 1);
    for (int h = 0; h < height; h++)
    {
        for (int s = 0; s < height - h - 1; s++)
        {
            printf(" ");
        }
        for (int l = 0; l < h + 1; l++)
        {
            printf("#");
        }
        printf("  ");
        for (int m = 0; m < h + 1; m++)
        {
            printf("#");
        }
        printf("\n");
    }
}
