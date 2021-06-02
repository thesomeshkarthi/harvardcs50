#include <stdio.h>
#include <cs50.h>

int main(void)
{

    //Retrieves user's desired height value between 1 and 8
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);


    //Iterates through rows of bricks
    for (int i = 0; i < height; i++)
    {
        //Displays number of spaces
        for (int j = 0; j < (height - (i + 1)); j++)
        {
            printf(" ");
        }

        //Displays number of hashes
        for (int k = 0; k < (i + 1); k++)
        {
            printf("#");
        }

        printf("  ");

        for (int m = 0; m < (i + 1); m++)
        {
            printf("#");
        }

        //Displays space
        printf("\n");
    }
}