#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string text = get_string("Text: "); // input
    int slenght = strlen(text);         // convert lenght of string into an integer

    for (int n = 0; n < slenght; n++)
    {
        text[n] = toupper(text[n]);
    }

    int letters = 0;
    int words = 0;
    int sentences = 0;

    for (int n = 0; n < slenght; n++)
    {
        if ('A' <= text[n] && text[n] <= 'Z')
            letters++;
        if ((text[n] == ' ' || text[n] == '.' || text[n] == '?' || text[n] == '!' || text[n] == ':' || text[n] == ',') &&
            'A' <= text[n - 1] && text[n - 1] <= 'Z')
            words++;
        if ('A' <= text[n - 1] && text[n - 1] <= 'Z' && (text[n] == '.' || text[n] == '?' || text[n] == '!'))
            sentences++;
    }

    float L = ((float) letters / words) * 100;
    float S = ((float) sentences / words) * 100;
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    if (index < 1)
        printf("Before Grade 1\n");
    else if (16 < index)
        printf("Grade 16+\n");
    else
        printf("Grade %i\n", index);
}
