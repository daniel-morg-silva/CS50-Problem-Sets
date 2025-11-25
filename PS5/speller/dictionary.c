// Implements a dictionary's functionality
#include <cs50.h>
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
} node;
// TODO: Choose number of buckets in hash table
const unsigned int N = 100;

// Hash table
node *table[N];

int dic_size = 0;
// Hashes word to a number
unsigned int hash(const char *word)
{
    long sum = 0;
    for (int n = 0; n < strlen(word); n++)
    {
        sum += tolower(word[n]);
    }
    return sum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *dic = fopen(dictionary, "r");
    if (dic == NULL)
    {
        return false;
    }
    char current_word[LENGTH + 1];
    while (fscanf(dic, "%s", current_word) != EOF)
    {
        node *p = malloc(sizeof(node));
        if (p == NULL)
        {
            fclose(dic);
            return false;
        }
        strcpy(p->word, current_word);
        int hash_code = hash(current_word);
        // stitching the pointers
        p->next = table[hash_code];
        table[hash_code] = p;
        dic_size++;
    }

    fclose(dic);
    return true;
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hash_code = hash(word);

    node *c = table[hash_code];

    while (c != NULL)
    {
        if (strcasecmp(c->word, word) == 0)
            return true;

        c = c->next;
    }
    return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dic_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int n = 0; n < N; n++)
    {
        node *cursor = table[n];

        while (cursor != NULL)
        {
            node *p = cursor;
            cursor = cursor->next;
            free(p);
        }
        table[n] = NULL;
    }
    return true;
}
