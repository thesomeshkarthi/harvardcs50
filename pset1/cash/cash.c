#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    // Recieve user's change
    float rawchange;
    do
    {
        rawchange = get_float("Change owed: ");
    }
    while (rawchange < 0);
    
    
    //Convert dollar change to cents
    int change = (int)((rawchange * 100) + 0.5);
    
    
    
    //Set original # of coins to 0
    int coins = 0;
    
    
    //Greed algorithim checks 25 cents first
    while (change >= 25)
    {
        coins ++;
        change = change - 25;
    }
    
    //Greed algorithim checks 10 cents second
    while (change >= 10)
    {
        coins ++;
        change = change - 10;
    }
    
    
    //Greed algorithim checks 5 cents third
    while (change >= 5)
    {
        coins ++;
        change = change - 5 ;
    }
    
    
    //Greed algorithim checks 1 cents last
    while (change >= 1)
    {
        coins ++;
        change = change - 1;
    }

    //Output number of coins used 
    printf("%i\n", coins);
}