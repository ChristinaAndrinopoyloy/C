#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#define SIZE 10000    /* Number of elements in table */
#define WORD_SIZE 100 /* Max size of word */

typedef struct CHAINELEMENTS
{
    char word[100];             /* word to be saved in list */
    struct CHAINELEMENTS *next; /* pointer to next element  */
} chainelements;

typedef struct TBLELEMENTS
{
    int total_elements;         /* total elements in chain  - Not used here so far  */
    chainelements *baseelement; /* Pointer to chained linked list of words */
} tblelements;

struct hashtable
{
    int size;          /* Number of table elements in hashtable */
    tblelements *base; /* Pointer to first table element */
};

struct hashtable *create_hastable(int size);
int ht_hash(char *key, int size_of_hashtable);
void insertWord(struct hashtable *phashtable, char *str);
bool searchWord(struct hashtable *phashtable, char *str);

#endif