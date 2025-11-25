#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Height of the pyramid:"); // get intput from user and set the height of the pyramid
    }
    while (height < 1);

    //----------------------------------------------//
    for (int h = 0; h < height; h++) // repeat as long as h < height
    {
        for (int s = 0; s < height - h - 1; s++)
        {
            printf(" "); // print the spaces that precede the hashes
        }
        for (int c = 0; c < h + 1; c++)
        {
            printf("#"); // print the hashes
        }
        printf("\n"); // change line
    }
}
