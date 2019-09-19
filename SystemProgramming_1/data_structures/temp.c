#include "hashtable.h"

int main(void)
{
   char oneword[WORD_SIZE];
   char c;
   char *searchword = "abash";
   bool ispresent;
   hashtable *phashtable = createHashTable(SIZE);

   insertWord(phashtable, "xristina");
   insertWord(phashtable, "alex");
   insertWord(phashtable, "maria");
   insertWord(phashtable, "kostas");
   insertWord(phashtable, "bibi");
   insertWord(phashtable, "zenia");
   insertWord(phashtable, "mwraaaaaaaki");
   insertWord(phashtable, "papapapa");

   return 0;
}

/*
    Funcntion    - createHashTable(int size)
    Input params - Table size 
    Return value - pointer to created hashtable
    Task         - hashtable is created based on given
                   size and all the hash table elements are 
                   initialized to NULL
*/

struct hashtable *create_hashtable(int size_of_hashtable)
{
   int i;
   struct hashtable *myhashtable = NULL;
   if (size_of_hashtable > 1) // checking for a valid value of size of hashtable (<1 in order to make a hashtable)
   {
      myhashtable = (struct hashtable *)malloc(sizeof(struct hashtable)); // allocate space for hashtable
      if (myhashtable == NULL)
      {
         printf("Error: malloc failed at creating a new hashtable!\n");
      }
      myhashtable->size = size_of_hashtable;
      myhashtable->base = (tblelements *)malloc(sizeof(tblelements) * myhashtable->size); // allocate space for hashtable
      if (myhashtable->base == NULL)
      {
         printf("Error: malloc failed at creating a new hashtable!\n");
      }

      for (i = 0; i < size_of_hashtable; i++) // initialize hashtable at NULL
      {
         myhashtable->base[i].baseelement = NULL;
      }
   }
   else
   {
      printf("Error: Give a positive integer for size of hashtable!\n");
   }
   return myhashtable;
}



bool searchWord(hashtable *phashtable, char *str)
{
   /* error condition check */
   if (phashtable == NULL || str == NULL)
   {
      fprintf(stderr, " -- invalid pointer to string or hashtable -- \n");
      return false;
   }

   int hashindex = getKey(str);

   /* error condition check */
   if (hashindex > SIZE)
   {
      fprintf(stderr, " -- invalid hashindex -- \n");
      return false;
   }

   /* pointer to first element of the chained linked list */
   chainelements *pchainelements = phashtable->base[hashindex].baseelement;

   while (pchainelements != NULL)
   {
      /* compares the given word with present in node */
      if (!strcmp(pchainelements->word, str))
      {
         return true;
      }
      pchainelements = pchainelements->next;
   }

   return false;
}

void insertWord(hashtable *phashtable, char *str)
{
   if (phashtable == NULL || str == NULL)
   {
      return;
   }

   bool ispresent;
   int hashindex = getKey(str);

   if (hashindex > SIZE)
   {
      fprintf(stderr, " -- invalid hashindex -- \n");
      return;
   }

   chainelements *pchainelements = phashtable->base[hashindex].baseelement;

   /* if no elements are present - insert word */
   if (!pchainelements)
   {
      pchainelements = (chainelements *)malloc(sizeof(chainelements));
      if (pchainelements == NULL)
      {
         fprintf(stderr, " -- memory allocation failed for the word -- \n");
      }
      phashtable->base[hashindex].baseelement = pchainelements;
      strcpy(pchainelements->word, str);
      pchainelements->next = NULL;
   }
   else
   {
      /* Check if the word is already present if not insert*/
      ispresent = searchWord(phashtable, str);

      if (ispresent == 0)
      {
         printf("insert word   is not present - so inserting %s \n", str);

         /* Can this part be optimized ?? while searching we already reached
               till the end of loop now again before inserting we have to traverse 
               Another option is to do compare here only but that would result in
               same code present at two point - What is better stratergy ?
             */
         while (pchainelements->next != NULL)
         {
            pchainelements = pchainelements->next;
         }

         pchainelements->next = (chainelements *)malloc(sizeof(chainelements));
         if (pchainelements->next == NULL)
         {
            fprintf(stderr, " -- memory allocation failed for the word -- \n");
         }

         pchainelements = pchainelements->next;
         strcpy(pchainelements->word, str);
         pchainelements->next = NULL;
      }
   }
}