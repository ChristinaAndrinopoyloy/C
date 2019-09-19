#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../structures.h"

typedef union Value {
    wallet mywallet;
    sender mysender;
    receiver myreceiver;
}value;

typedef struct Record
{
    char key[50];
    value hash_value;
} record;

typedef struct Metadata
{
    struct Bucket *next_bucket;
    int current_records;
} metadata;

typedef struct Bucket
{
    metadata meta_data;
    record *data;
} bucket;

typedef struct Hashtable
{
    bucket **hash_table;
    int number_of_bytes;
    int number_of_records;
    int size_of_hashtable;
} hashtable;

int produce_key(char *key, int size_of_hashtable);
hashtable *create_hashtable(int size_of_hashtable, int number_of_bytes);
void insert_record(hashtable **myhashtable, record myrecord, int type_of_hashtable);
record *search_hashtable(char *key, hashtable **myhashtable);
record initialize_record(int type_of_hashtable);
record assign_record(record destination, record src, int type_of_hashtable);

#endif