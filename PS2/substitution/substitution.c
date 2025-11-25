#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int key_vrf(string arg, int agc);

void substituion(int l, string k, string txt, char ct[]);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substituion key (must contain 26 letters)\n");
        return 1;
    }

    string key = argv[1];

    for (int n = 0, l = strlen(key); n < l; n++)
        key[n] = toupper(key[n]);

    if (0 < key_vrf(key, argc))
        return 1;
    string text = get_string("plaintext: ");

    int text_lenght = strlen(text);
    char ciphertext[text_lenght + 1];

    substituion(text_lenght, key, text, ciphertext);

    printf("ciphertext: %s\n", ciphertext);

    return 0;
}

int key_vrf(string arg, int agc)
{
    char alph[26];

    for (int n = 0, l = strlen(arg); n < l; n++)
    {
        if (isalpha(arg[n]) == 0 || l != 26)
        {
            printf("Usage: ./substituion key (must contain 26 letters)\n");
            return 2;
        }
        for (int m = 0, p = strlen(alph); m <= p; m++)
        {
            if (arg[n] == alph[m])
            {
                printf("Usage: ./substituion key (you can't repeat letters))\n");
                return 3;
            }
        }
        alph[n] = arg[n];
    }
    return 0;
}

void substituion(int l, string k, string txt, char ct[])
{
    for (int n = 0; n < l; n++)
    {
        int pos = 0;
        if ('A' <= txt[n] && txt[n] <= 'Z')
        {
            pos = txt[n] - 'A';
            ct[n] = k[pos];
        }
        else if ('a' <= txt[n] && txt[n] <= 'z')
        {
            pos = txt[n] - 'a';
            ct[n] = k[pos] + 32;
        }
        else
            ct[n] = txt[n];
    }
    ct[l] = '\0';
}
