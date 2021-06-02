#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //For loop iterates through each pixel and averages
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int average = roundf((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{

    //For loop iterates through each pixels and applied formula
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sepiaRed = roundf(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue);
            int sepiaGreen = roundf(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue);
            int sepiaBlue = roundf(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue);
    
            if (sepiaRed > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = sepiaRed;
            }

            if (sepiaGreen > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = sepiaGreen;
            }

            if (sepiaBlue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = sepiaBlue;
            }
        }
    }

    return;
}



// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //For loop iterates through each pixel and swaps using temp as temp memory
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < round(width / 2); j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    int redaverages[height][width];
    int greenaverages[height][width];
    int blueaverages[height][width];

    //For loop iterates through each pixel and copies their RGB 
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Special case for j = 0
            if (j == 0)
            {
                if (i == 0)
                {
                    //Copies red averages to array
                    redaverages[i][j] = roundf((image[i][j].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed + image[i][j + 1].rgbtRed) /
                                               4.0);
                    greenaverages[i][j] = roundf((image[i][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen + image[i][j +
                                                  1].rgbtGreen) / 4.0);
                    blueaverages[i][j] = roundf((image[i][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue + image[i][j +
                                                 1].rgbtBlue) / 4.0);
                }
                else if (i == height - 1)
                {
                    //Copies red averages to array
                    redaverages[i][j] = roundf((image[i][j].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j + 1].rgbtRed) /
                                               4.0);
                    greenaverages[i][j] = roundf((image[i][j].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j +
                                                  1].rgbtGreen) / 4.0);
                    blueaverages[i][j] = roundf((image[i][j].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j +
                                                 1].rgbtBlue) / 4.0);
                }
                else
                {
                    //Copies red averages to array
                    redaverages[i][j] = roundf((image[i][j].rgbtRed + image[i + 1][j].rgbtRed + image[i - 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed +
                                                image[i - 1][j + 1].rgbtRed + image[i][j + 1].rgbtRed) / 6.0);
                    greenaverages[i][j] = roundf((image[i][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i - 1][j].rgbtGreen + image[i + 1][j +
                                                  1].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j + 1].rgbtGreen) / 6.0);
                    blueaverages[i][j] = roundf((image[i][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i - 1][j].rgbtBlue + image[i + 1][j +
                                                 1].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j + 1].rgbtBlue) / 6.0);
                }
            }
            //Special case if j is width - 1
            else if (j == width - 1)
            {
                if (i == 0)
                {
                    //Copies red averages to array
                    redaverages[i][j] = roundf((image[i][j].rgbtRed + image[i][j - 1].rgbtRed + image[i + 1][j].rgbtRed
                                                + image[i + 1][j - 1].rgbtRed) / 4.0);
                    greenaverages[i][j] = roundf((image[i][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen
                                                  + image[i + 1][j - 1].rgbtGreen) / 4.0);
                    blueaverages[i][j] = roundf((image[i][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue
                                                 + image[i + 1][j - 1].rgbtBlue) / 4.0);
                }
                else if (i == height - 1)
                {
                    //Copies red averages to array
                    redaverages[i][j] = roundf((image[i][j].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j - 1].rgbtRed + image[i][j - 1].rgbtRed) /
                                               4.0);
                    greenaverages[i][j] = roundf((image[i][j].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j - 1].rgbtGreen + image[i][j -
                                                  1].rgbtGreen) / 4.0);
                    blueaverages[i][j] = roundf((image[i][j].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j - 1].rgbtBlue + image[i][j -
                                                 1].rgbtBlue) / 4.0);
                }
                else
                {
                    //Copies red averages to array
                    redaverages[i][j] = roundf((image[i][j].rgbtRed + image[i + 1][j].rgbtRed + image[i - 1][j].rgbtRed + image[i + 1][j - 1].rgbtRed +
                                                image[i - 1][j - 1].rgbtRed + image[i][j - 1].rgbtRed) / 6.0);
                    greenaverages[i][j] = roundf((image[i][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i - 1][j].rgbtGreen + image[i + 1][j -
                                                  1].rgbtGreen + image[i - 1][j -
                                                          1].rgbtGreen + image[i][j - 1].rgbtGreen) / 6.0);
                    blueaverages[i][j] = roundf((image[i][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i - 1][j].rgbtBlue + image[i + 1][j -
                                                 1].rgbtBlue + image[i - 1][j -
                                                         1].rgbtBlue + image[i][j - 1].rgbtBlue) / 6.0);
                }
            }
            //Special case if i = 0
            else if (i == 0)
            {
                //Copies red averages to array
                redaverages[i][j] = roundf((image[i][j].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed + image[i + 1][j -
                                            1].rgbtRed + image[i][j + 1].rgbtRed + image[i][j - 1].rgbtRed) / 6.0);
                greenaverages[i][j] = roundf((image[i][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen + image[i + 1][j -
                                              1].rgbtGreen + image[i][j + 1].rgbtGreen + image[i][j - 1].rgbtGreen) / 6.0);
                blueaverages[i][j] = roundf((image[i][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue + image[i + 1][j -
                                             1].rgbtBlue + image[i][j + 1].rgbtBlue + image[i][j - 1].rgbtBlue) / 6.0);
            }
            //Special case if i is height - 1
            else if (i == height - 1)
            {
                //Copies red averages to array
                redaverages[i][j] = roundf((image[i][j].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i - 1][j -
                                            1].rgbtRed + image[i][j + 1].rgbtRed + image[i][j - 1].rgbtRed) / 6.0);
                greenaverages[i][j] = roundf((image[i][j].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i - 1][j -
                                              1].rgbtGreen + image[i][j + 1].rgbtGreen + image[i][j - 1].rgbtGreen) / 6.0);
                blueaverages[i][j] = roundf((image[i][j].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i - 1][j -
                                             1].rgbtBlue + image[i][j + 1].rgbtBlue + image[i][j - 1].rgbtBlue) / 6.0);

            }
            //All regular treatments of 9 are put in else
            else
            {
                //Copies red averages to array
                redaverages[i][j] = roundf((image[i][j].rgbtRed + image[i - 1][j].rgbtRed + image[i + 1][j].rgbtRed + image[i][j + 1].rgbtRed +
                                            image[i][j - 1].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i + 1][j + 1].rgbtRed + image[i - 1][j - 1].rgbtRed + image[i - 1][j +
                                                    1].rgbtRed) / 9.0);
                greenaverages[i][j] = roundf((image[i][j].rgbtGreen + image[i - 1][j].rgbtGreen + image[i + 1][j].rgbtGreen + image[i][j +
                                              1].rgbtGreen + image[i][j - 1].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen + image[i - 1][j -
                                                      1].rgbtGreen + image[i - 1][j + 1].rgbtGreen) / 9.0);
                blueaverages[i][j] = roundf((image[i][j].rgbtBlue + image[i - 1][j].rgbtBlue + image[i + 1][j].rgbtBlue + image[i][j + 1].rgbtBlue +
                                             image[i][j - 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue + image[i - 1][j - 1].rgbtBlue + image[i -
                                                     1][j + 1].rgbtBlue) / 9.0);
            }
        }
    }

    //Iterates through each pixel and assigns value
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = redaverages[i][j];
            image[i][j].rgbtGreen = greenaverages[i][j];
            image[i][j].rgbtBlue = blueaverages[i][j];
        }
    }

    return;
}



