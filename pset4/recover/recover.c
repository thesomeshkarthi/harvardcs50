#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

//Prompts user for filename
int main(int argc, char *argv[])
{
    //If user doesn't input two fields error message displayed
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    //If user types wrong file name error message displayed
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        printf("Could not open file %s\n", argv[1]);
        return 1;
    }

    //Store all neseccary information needed universally in loop
    BYTE buffer[512];
    int images = 0;
    char filename[8];
    FILE *output = NULL;

    //Will repeat until all 512 blocks are iterated through 
    while (fread(&buffer, 512, 1, file))
    {
        //Conditions for the first 4 bytes to ensure the image is a jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //If images is 0 special case does not need to close previous file
            if (images == 0)
            {
                sprintf(filename, "%03i.jpg", images);
                output = fopen(filename, "w");
                images++;
            }
            else
            {
                fclose(output);
                sprintf(filename, "%03i.jpg", images);
                output = fopen(filename, "w");
                images++;
            }
        }
        
        //If images is not 0 then the block can be copied
        if (images != 0)
        {
            fwrite(&buffer, 512, 1, output);
        }

    }
    
    //Close files
    fclose(file);
    fclose(output);
    
}