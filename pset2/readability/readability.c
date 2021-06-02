#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//Prototypes for functions
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    //Access functions to count the letters, words, and strings of text
    string text = get_string("Text: ");
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);
    
    //Store averages in float variables
    float L = (100.0 / words) * letters;
    float S = (100.0 / words) * sentences;
    
    //Input variables in formula and round to nearest integer
    float rawindex = roundf(0.0588 * L - 0.296 * S - 15.8);
    int index = (int) rawindex;
    
    //If index is in between 1 and 16 exact index is displayed
    if (index >= 1 && index <= 16)
    {
        printf("Grade %i\n", index);
    }
    //If index is less than 1 Before Grade 1 Message is displayed
    else if (index < 1)
    {
        printf("Grade: Before Grade 1\n");
    }
    //Else it is assumed that the grade is above 16
    else
    {
        printf("Grade 16+\n");
    }
}


//Count letters code 
int count_letters(string text)
{
    int n = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        //Count number of uppercase and lowercase letters
        if ((isupper(text[i])) || (islower(text[i])))
        {
            n++;
        }
    }
    return n;
}

//Count words code
int count_words(string text)
{
    int n = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        //Count number of spaces
        if (text[i] == ' ')
        {
            n++;
        }
    }
    
    return n + 1;
}

//Count sentences code
int count_sentences(string text)
{
    int n = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        int asciitext = (int)text[i];
        //Count the specific question marks, periods, and exclamation marks.
        if (asciitext == 33 || asciitext == 46 || asciitext == 63)
        {
            n++;
        }
    }
    
    //return n value 
    return n;
}
