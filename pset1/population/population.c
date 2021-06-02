#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Prompt for start size until user enters integer above 9
    int startsize;
    do
    {
        startsize = get_int("Starting Population Size: ");  
    }
    while (startsize < 9);
    
    //Prompt for end size until user enters integer greater than starting size
    int endsize;
    do
    {
        endsize = get_int("Ending Population Size: ");  
    }
    while (endsize < startsize);
    


    //Calculate number of years until we reach threshold
    int yearendsize = 0;
    int years = 0;
    if (startsize != endsize)
    {
        while (yearendsize < endsize)
        {
            yearendsize = (startsize + (startsize / 3)) - (startsize / 4);
            startsize = yearendsize;
            years++;
        } 
    }
   
    //Print number of years
    printf("Years: %i\n", years);
}