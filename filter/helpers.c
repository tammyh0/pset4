#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            // Calculate new value
            int total = 0;
            total += image[i][j].rgbtBlue;
            total += image[i][j].rgbtGreen;
            total += image[i][j].rgbtRed;
            int new_value = round(total / (float) 3);
            
            // Assign new value
            image[i][j].rgbtBlue = new_value;
            image[i][j].rgbtGreen = new_value;
            image[i][j].rgbtRed = new_value;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate new values
            int sepiaRed = round(.393 * (float) image[i][j].rgbtRed + .769 * (float) image[i][j].rgbtGreen + .189 *
                                 (float) image[i][j].rgbtBlue);
            int sepiaGreen = round(.349 * (float) image[i][j].rgbtRed + .686 * (float) image[i][j].rgbtGreen + .168 *
                                   (float) image[i][j].rgbtBlue);
            int sepiaBlue = round(.272 * (float) image[i][j].rgbtRed + .534 * (float) image[i][j].rgbtGreen + .131 *
                                  (float) image[i][j].rgbtBlue);
            
            // Assign new values
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
    for (int i = 0; i < height; i++)
    {
        int stopPoint = 0;
        if (height % 2) // Height is odd
        {
            stopPoint = (int) floor(width / 2.0);
        }
        else // Height is even
        {
            stopPoint = width / 2;
        }
        
        // Swap first and last pixel of row
        for (int j = 0; j < stopPoint; j++) 
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - (1 + j)];
            image[i][width - (1 + j)] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height * width];
    int currentPixel = 0;
    
    // Go through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++) 
        {
            int totalRed = 0;
            int totalGreen = 0;
            int totalBlue = 0;
            int count = 0;
            
            // Go through all neighboring pixels
            for (int k = -1; k < 2; k++) 
            {
                for (int l = -1; l < 2; l++) 
                {
                    int x = k + i; 
                    int y = l + j; 
                    
                    // Check if neighboring pixel exists
                    if (x >= 0 && x < height && y >= 0 && y < width)
                    {
                        totalRed += image[x][y].rgbtRed;
                        totalGreen += image[x][y].rgbtGreen;
                        totalBlue += image[x][y].rgbtBlue;
                        count++;
                    }
                }
            }
            
            // Calculate average of each color and save in temp
            temp[currentPixel].rgbtRed = round(totalRed / (float) count);
            temp[currentPixel].rgbtGreen = round(totalGreen / (float) count);
            temp[currentPixel].rgbtBlue = round(totalBlue / (float) count);
            currentPixel++;
        }
    }
    
    // Replace with new values
    int index = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp[index].rgbtRed;
            image[i][j].rgbtGreen = temp[index].rgbtGreen;
            image[i][j].rgbtBlue = temp[index].rgbtBlue;
            index++;
        }
    }
    return;
}
