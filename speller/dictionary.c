// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

//Choose number of buckets in hash table
const unsigned int N = (4050); //ascii Z = 90 * LENGTH -> 4050

int word_counter = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int checkWord = hash(word);

    node *checkIndex = table[checkWord];
    while (checkIndex != NULL)
    {
        if (strcasecmp(checkIndex->word, word) == 0)
        {
            return true;
        }
        checkIndex = checkIndex->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int index = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        index += toupper(word[i]); //For every letter in word calculate number
    }
    return index;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *dict = fopen(dictionary, "r");//open file
    if (dict == NULL)
    {
        printf("Could not open file(1).\n");
        unload();
        return false;
    }

    char dictWord[LENGTH + 1]; //save words
    while (fscanf(dict, "%s", dictWord) != EOF) //do untill you reach EOF
    {
        node *newWord = malloc(sizeof(node)); //Allocate memory
        if (newWord == NULL) //Check memory
        {
            return false;
        }

        strcpy(newWord->word, dictWord); //Copy words
        newWord->next = NULL; //Set pointer to null

        int index = hash(newWord->word); //Get an index from hashing

        if (table[index] == NULL) //Set first word
        {
            table[index] = newWord;
        }
        else
        {
            newWord->next = table[index]; //Point new element to first element
            table[index] = newWord; //Point head to new element
        }
        word_counter ++;
    }

    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *front = table[i]; //Open each linked list [0 - N]
        node *cursor = front; //Create moving pointer
        node *tmp = front; //Create temporary pointer

        while (cursor != NULL) //Do untill empty
        {
            cursor = cursor->next; //Move cursor to next
            free(tmp); //Free current adress
            tmp = cursor; //point temporary pointer back to cursor(head)
        }
    }
    return true;
}