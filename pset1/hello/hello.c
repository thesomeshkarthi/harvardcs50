#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //Asks user for name and stores string in variable 'name'
    string name = get_string("What is your name?\n");
    
    //prints out name in conjunction with hello phrase.
    printf("hello, %s\n", name);
}