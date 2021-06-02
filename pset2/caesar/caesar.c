#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, string argv[])
{
    //If argument is more than 2 letters, the error must be displayed.
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    else
    {
        //Check to see if argument is only numbers
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
        {
            if (argv[1][i] > 57 || argv[1][i] < 48)
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }
        
        //Convert key into integer using atoi function
        int key = atoi(argv[1]);
        
        
        
        //Prompt user for string and set up cipher text 
        string plaintext = get_string("plaintext: ");
        printf("ciphertext: ");
        
        //For loop iterates through plain text letters
        for (int i = 0; i < strlen(plaintext); i++)
        {
            char c = plaintext[i] + key;
            //Checks if letter is lowercase
            if (islower(plaintext[i]))
            {
                c = ((plaintext[i] - 'a' + key) % 26) + 'a';
                printf("%c", c);
            }
            //Checks if letter is uppercase
            else if (isupper(plaintext[i]))
            {
                c = ((plaintext[i] - 'A' + key) % 26) + 'A';
                printf("%c", c);
            }
            //Assumes letter is not letter and does not make change.
            else
            {
                printf("%c", plaintext[i]);
            }
        }
        //Prints space at the end. 
        printf("\n");
    }
}