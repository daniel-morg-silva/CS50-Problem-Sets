#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int change;
    int remainer;
    int n = 0;
    // Setting the change
    do
    {
        change = get_int("Change owed: ");
    }
    while (change < 1);
    // Recognition
    // Change greater than 25
    if (change >= 25)
    {
        n = n + change / 25;    // n = 2
        remainer = change % 25; // remainer = 20
        change = remainer;      // change = 20
    }
    // Change lesser than 25
    if (10 <= change && change < 25)
    {
        n = n + change / 10; // n = 4
        remainer = change % 10;
        change = remainer;
    }
    // Change lesser than 10
    if (5 <= change && change < 10)
    {
        n = n + change / 5;
        remainer = change % 5;
        change = remainer;
    }
    // Change lesser than 5
    if (change < 5)
    {
        n = n + change;
    }
    printf("%i\n", n);
}
