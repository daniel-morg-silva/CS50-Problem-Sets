#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    char numbers[20];
    int numblenght = 0;

    long dcn = get_long("Number: "); // input for user

    sprintf(numbers, "%ld", dcn); // convert the input into string

    numblenght = strlen(numbers);

    int number[numblenght]; // array representing the number but in a int to be able to do numeric operations

    for (int n = 0; n < numblenght; n++)
    {
        number[n] = numbers[n] - '0'; // convert the char array to a int array to be able to do numeric operations
    }

    int sum = 0;

    for (int n = numblenght - 2; n >= 0; n -= 2)
    {
        int doublen = number[n] * 2;
        if (doublen > 9)
        {
            sum += doublen / 10 + doublen % 10;
        }
        else
        {
            sum += doublen;
        }
    }

    for (int n = numblenght - 1; n >= 0; n -= 2)
    {
        sum += number[n];
    }

    printf("%i\n", sum);

    if (sum % 10 == 0)
    {
        if (numblenght == 16 && number[0] == 5 &&
            (number[1] == 1 || number[1] == 2 || number[1] == 3 || number[1] == 4 || number[1] == 5))
        {
            printf("MASTERCARD\n");
        }
        else if (numblenght == 15 && number[0] == 3 && (number[1] == 4 || number[1] == 7))
        {
            printf("AMEX\n");
        }
        else if ((numblenght == 13 || numblenght == 16) && number[0] == 4)
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
