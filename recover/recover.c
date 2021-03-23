#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t BYTE;

// Number of bytes in a block
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover FILE_NAME.raw\n");
        return 1;
    }

    // Open memory card file
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Search memory card file one block at a time for JPEGs
    uint8_t buffer[BLOCK_SIZE];
    FILE *img = NULL;
    bool isHeader = true;
    int count = 0;
    while (fread(buffer, sizeof(uint8_t), BLOCK_SIZE, input))
    {
        // Found new JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Close the current JPEG file 
            if (count != 0)
            {
                fclose(img);
            }
            
            // Open new JPEG file
            char *fname = malloc(8);
            sprintf(fname, "%03i.jpg", count);
            img = fopen(fname, "w");
            free(fname);
            
            // Write to new JPEG file
            fwrite(buffer, sizeof(uint8_t), BLOCK_SIZE, img);
            
            isHeader = false;
            count++;
        }
        // Continue writing to continuation of current JPEG file
        else if (!isHeader)
        {
            fwrite(buffer, sizeof(uint8_t), BLOCK_SIZE, img);
        }
    }

    // Close files
    fclose(input);
    fclose(img);
}