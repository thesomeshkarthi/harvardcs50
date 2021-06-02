#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10, 0};

//Prototype for computing function
int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 == score2)
    {
        printf("Tie!\n");
    }
    else
    {
        printf("Player 2 wins!\n");
    }
}

int compute_score(string word)
{
    //Declares sum and position variables
    int sum = 0;
    int position;
    
    //For loop that iterates through each character in word
    for (int i = 0; i < strlen(word); i++)
    {
        //If word is lowercase, following code is used to derive position 
        if (islower(word[i]))
        {
            position = (int)(word[i] - 'a');
        }
        //If word is uppercase, following code is used to derive position
        else if (isupper(word[i]))
        {
            position = (int)((word[i] + 32) - 'a');
        }
        //If word is not a letter, following code is used to derive position
        else
        {
            position = 26;
        }
        
        //Sum of points 
        sum = sum + POINTS[position];
    }
    return sum;
}