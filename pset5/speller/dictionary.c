// Implements a dictionary's functionality

#include <stdbool.h>
#include "dictionary.h"
#include <stdio.h>
#include <ctype.h>
#include <cs50.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>



// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26 ^ 10;

// Hash table
node *table[N];

//Dictionary Length Tracker
int dictlength = 0;


// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int index = (hash(word)) - 1;

    for (node *tmp = table[index]; tmp != NULL; tmp = tmp->next)
    {
        if (strcasecmp(tmp->word, word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
//Cited Code: https://stackoverflow.com/a/7666577, djb2 by Dan Bernstein
unsigned int hash(const char *word)
{
    // TODO
    unsigned long hash = 5381;
    int c = 0;

    while (tolower(c) == *word++)
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    
    char word[LENGTH + 1];

    while (fscanf(file, "%s", word) != EOF)
    {
        //fscanf(file, "%s", word);
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            unload();
            return false;
        }

        strcpy(n->word, word);

        int index = hash(word) - 1;
        n->next = table[index];
        table[index] = n;

        dictlength++;
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (dictlength > 0)
    {
        return dictlength;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
    }

    return true;
}
