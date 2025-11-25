#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    BYTE *buffer = malloc(512);
    FILE *card = fopen("card.raw", "r");
    if (card == NULL)
        return 1;

    int counter = 0;
    char *filename = malloc(8 * sizeof(char));
    FILE *image;

    while (fread(buffer, 1, 512, card) == 512)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0 && counter == 0)
        {
            sprintf(filename, "%03i.jpg", counter);
            image = fopen(filename, "w");
            if (image == NULL)
                return 1;
            fwrite(buffer, 1, 512, image);
            counter++;
        }

        else if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0 && counter > 0)
        {
            fclose(image);
            sprintf(filename, "%03i.jpg", counter);
            image = fopen(filename, "w");
            if (image == NULL)
                return 1;
            fwrite(buffer, 1, 512, image);
            counter++;
        }
        else if (counter > 0)
        {
            fwrite(buffer, 1, 512, image);
        }
    }

    fclose(card);
    if (image != NULL)
        fclose(image);
    free(buffer);
    free(filename);
}
