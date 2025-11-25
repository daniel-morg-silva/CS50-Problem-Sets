#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void uppercase(string p1, string p2, int l1, int l2);

int main(void)
{
    string player1 = get_string("Player 1: "); // get input and set the word from player1
    int lenght1 = strlen(player1); // set the lenght of the word1
    string player2 = get_string("Player 2: "); // get input and set the word from player2
    int lenght2 = strlen(player2); // set the lenght of the word2

    uppercase(player1, player2, lenght1, lenght2); // execute uppercase function

    int scores[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
    int score1 = 0;
    int score2 = 0;

    for (int n = 0; n < lenght1; n++)
        if (player1[n] >= 'A' && player1[n] <= 'Z')
        {
            score1 += scores[player1[n] - 'A']; // check the letters from the word and assign to the respective score and adding it to the score itself
        }

    for (int n = 0; n < lenght2; n++)
        if (player2[n] >= 'A' && player2[2] <= 'Z')
        {
            score2 += scores[player2[n] - 'A']; // check the letters from the word and assign to the respective score and adding it to the score itself
        }

    if (score1 > score2) // if score from player1 is higher
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2) // if score from player2 is higher
    {
        printf("Player 2 wins!\n");
    }
    else // if score from player1 equal to player2
    {
        printf("Tie!\n");
    }
}

void uppercase(string p1, string p2, int l1, int l2) // function uppercase the words
{
    for (int n = 0; n < l1; n++)
    {
        p1[n] = toupper(p1[n]);
    }

    for (int n = 0; n < l2; n++)
    {
        p2[n] = toupper(p2[n]);
    }
}
