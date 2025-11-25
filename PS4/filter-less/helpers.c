#include "helpers.h"
#include <math.h>
#include <stdlib.h>

void check_surround_pixel(int height, int width, int n, int m, int *counter, float *red, float *green, float *blue,
                          RGBTRIPLE cpimage[height][width]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int n = 0; n < height; n++)
    {
        for (int m = 0; m < width; m++)
        {
            float blue = image[n][m].rgbtBlue;
            float green = image[n][m].rgbtGreen;
            float red = image[n][m].rgbtRed;
            int grayscale = round((red + blue + green) / 3);
            image[n][m].rgbtBlue = grayscale;
            image[n][m].rgbtGreen = grayscale;
            image[n][m].rgbtRed = grayscale;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int n = 0; n < height; n++)
        for (int m = 0; m < width; m++)
        {
            float tr = 0.393 * image[n][m].rgbtRed + 0.769 * image[n][m].rgbtGreen + 0.189 * image[n][m].rgbtBlue;
            int r = round(tr);
            float tg = 0.349 * image[n][m].rgbtRed + 0.686 * image[n][m].rgbtGreen + 0.168 * image[n][m].rgbtBlue;
            int g = round(tg);
            float tb = 0.272 * image[n][m].rgbtRed + 0.534 * image[n][m].rgbtGreen + 0.131 * image[n][m].rgbtBlue;
            int b = round(tb);
            if (r > 255)
                image[n][m].rgbtRed = 255;
            else
                image[n][m].rgbtRed = r;

            if (g > 255)
                image[n][m].rgbtGreen = 255;
            else
                image[n][m].rgbtGreen = g;

            if (b > 255)
                image[n][m].rgbtBlue = 255;
            else
                image[n][m].rgbtBlue = b;
        }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int n = 0; n < height; n++)
        for (int m = 0; m < width / 2; m++)
        {
            RGBTRIPLE temp_pos = image[n][m];
            image[n][m] = image[n][width - 1 - m];
            image[n][width - 1 - m] = temp_pos;
        }
    return;
}

void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE cpimage[height][width];
    for (int n = 0; n < height; n++)
        for (int m = 0; m < width; m++)
            cpimage[n][m] = image[n][m];

    for (int n = 0; n < height; n++)
        for (int m = 0; m < width; m++)
        {
            float red = 0;
            float blue = 0;
            float green = 0;
            int counter = 0;
            check_surround_pixel(height, width, n, m, &counter, &red, &green, &blue, cpimage);
            image[n][m].rgbtRed = round(red / counter);
            image[n][m].rgbtGreen = round(green / counter);
            image[n][m].rgbtBlue = round(blue / counter);
        }
}

void check_surround_pixel(int height, int width, int n, int m, int *counter, float *red, float *green, float *blue,
                          RGBTRIPLE cpimage[height][width])
{
    for (int i = -1; i < 2; i++)
        for (int j = -1; j < 2; j++)
        {
            if (n + i < 0 || n + i >= height)
                continue;
            if (m + j < 0 || m + j >= width)
                continue;
            *red += cpimage[n + i][m + j].rgbtRed;
            *blue += cpimage[n + i][m + j].rgbtBlue;
            *green += cpimage[n + i][m + j].rgbtGreen;
            (*counter)++;
        }
}
