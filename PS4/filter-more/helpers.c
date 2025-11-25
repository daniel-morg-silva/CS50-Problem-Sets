#include "helpers.h"
#include <math.h>

void help_blur(int height, int width, float *red, float *blue, float *green, int *counter, int n, int m,
               RGBTRIPLE cpimage[height][width]);
void help_edges(int height, int width, int n, int m, float *xred, float *xblue, float *xgreen, float *yred, float *yblue,
                float *ygreen, RGBTRIPLE cpimage[height][width]);
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int n = 0; n < height; n++)
        for (int m = 0; m < width; m++)
        {
            float grey = (float) (image[n][m].rgbtRed + image[n][m].rgbtBlue + image[n][m].rgbtGreen) / 3;
            int grayscale = round(grey);
            image[n][m].rgbtRed = grayscale;
            image[n][m].rgbtBlue = grayscale;
            image[n][m].rgbtGreen = grayscale;
        }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int n = 0; n < height; n++)
        for (int m = 0; m < width / 2; m++)
        {
            RGBTRIPLE temp = image[n][m];
            image[n][m] = image[n][width - 1 - m];
            image[n][width - 1 - m] = temp;
        }
    return;
}

// Blur image
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
            help_blur(height, width, &red, &blue, &green, &counter, n, m, cpimage);
            image[n][m].rgbtRed = round(red / counter);
            image[n][m].rgbtBlue = round(blue / counter);
            image[n][m].rgbtGreen = round(green / counter);
        }
    return;
}

void help_blur(int height, int width, float *red, float *blue, float *green, int *counter, int n, int m,
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE cpimage[height][width];
    for (int n = 0; n < height; n++)
        for (int m = 0; m < width; m++)
            cpimage[n][m] = image[n][m];

    for (int n = 0; n < height; n++)
        for (int m = 0; m < width; m++)
        {
            float xred = 0;
            float xblue = 0;
            float xgreen = 0;
            float yred = 0;
            float yblue = 0;
            float ygreen = 0;
            help_edges(height, width, n, m, &xred, &xblue, &xgreen, &yred, &yblue, &ygreen, cpimage);
            int red = round(sqrt(pow(xred, 2) + pow(yred, 2)));
            int blue = round(sqrt(pow(xblue, 2) + pow(yblue, 2)));
            int green = round(sqrt(pow(xgreen, 2) + pow(ygreen, 2)));

            if (red > 255)
                image[n][m].rgbtRed = 255;
            else
                image[n][m].rgbtRed = red;
            if (blue > 255)
                image[n][m].rgbtBlue = 255;
            else
                image[n][m].rgbtBlue = blue;
            if (green > 255)
                image[n][m].rgbtGreen = 255;
            else
                image[n][m].rgbtGreen = green;
        }
    return;
}

void help_edges(int height, int width, int n, int m, float *xred, float *xblue, float *xgreen, float *yred, float *yblue,
                float *ygreen, RGBTRIPLE cpimage[height][width])
{
    int xkernel[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int ykernel[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 1; i > -2; i--)
        for (int j = 1; j > -2; j--)
        {
            if (n + i < 0 || n + i >= height)
                continue;
            if (m + j < 0 || m + j >= width)
                continue;
            *xred += xkernel[i + 1][j + 1] * cpimage[n + i][m + j].rgbtRed;
            *yred += ykernel[i + 1][j + 1] * cpimage[n + i][m + j].rgbtRed;
            *xblue += xkernel[i + 1][j + 1] * cpimage[n + i][m + j].rgbtBlue;
            *yblue += ykernel[i + 1][j + 1] * cpimage[n + i][m + j].rgbtBlue;
            *xgreen += xkernel[i + 1][j + 1] * cpimage[n + i][m + j].rgbtGreen;
            *ygreen += ykernel[i + 1][j + 1] * cpimage[n + i][m + j].rgbtGreen;
        }
}
