#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    long ccn;
    int stringl;
    int sum = 0;
    char stringls[20];

    ccn = get_long("Number: ");    // Input type long
    sprintf(stringls, "%ld", ccn); // Convert input into stringls
    stringl = strlen(stringls);    // check the lenght of stringl and assingning to stringl

    int checknumbers[stringl]; // declaring new array
    int code[stringl];
    for (int n = 0; n < stringl; n++) // as long as n is lesser than stringl
    {
        checknumbers[n] = stringls[n] - '0'; // convert the array "stringls" into an in array called "checknumbers"
    }
    for (int n = stringl - 2; n >= 0; n = n - 2)
    {
        if (checknumbers[n] * 2 > 9)
        {
            code[n] = checknumbers[n] * 2; // store the double of the odd position values into array code
        }
        else
        {
            sum = sum + checknumbers[n] * 2;
        }
    }

    if (sizeof(code[0]) > 0)
    {
        for (int n = 0; n < sizeof(code) / sizeof(code[0]); n++)
        {
            if (code[n] > 9) // for numbers higher than 9, separate them and add them to sum
            {
                sum = sum + code[n] / 10 + code[n] % 10;
            }
        }
    }

    for (int n = stringl - 1; n >= 0; n = n - 2)
    {
        sum = sum + checknumbers[n];
    }

    if (stringl == 15 && ((checknumbers[0] == 3 && checknumbers[1] == 4) || (checknumbers[0] == 3 && checknumbers[1] == 7)))
    {
        if (sum % 10 == 0)
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }

    else if (stringl == 16 && checknumbers[0] == 5 &&
             (checknumbers[1] == 1 || checknumbers[1] == 2 || checknumbers[1] == 3 || checknumbers[1] == 4 || checknumbers[1] == 5))
    {

        if (sum % 10 == 0)
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }

    else if ((stringl == 16 || stringl == 13) && checknumbers[0] == 4)
    {

        if (sum % 10 == 0)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}
