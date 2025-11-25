#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_key_validity(int l, string i[]);
string caesar_shift(int x, string y);

int main(int argc, string argv[])
{
    if (check_key_validity(argc, argv) != 0)
        return 1;

    int key = atoi(argv[1]) % 26;

    string plaintext = get_string("plaintext: ");

    printf("ciphertext: %s\n", plaintext = caesar_shift(key, plaintext));
}

int check_key_validity(int l, string i[])
{

    if (2 < l || l == 1)
    {
        printf("Usage: ./caesar key (caesar must only have one key)\n");
        return 1;
    }

    for (int n = 0; n < strlen(i[1]); n++)
    {
        if (isdigit(i[1][n]) == 0)
        {
            printf("Usage: ./caesar key (key must only have numeric values)\n");
            return 2;
        }
    }
    return 0;
}

string caesar_shift(int x, string y)
{
    for (int n = 0, c = strlen(y); n < c; n++)
    {
        if (isalpha(y[n]) != 0)
        {
            if ((y[n] + x <= 'z' && 'a' <= y[n] + x) ||
                (y[n] + x <= 'Z' && 'A' <= y[n] + x))
                y[n] += x;
            else
                for (int m = 0; m < x; m++)
                {
                    if (('a' <= y[n] + 1 && y[n] + 1 <= 'z') ||
                        ('A' <= y[n] + 1 && y[n] + 1 <= 'Z'))
                        y[n]++;
                    else
                        y[n] -= 25;
                }
        }
    }
    return y;
}
