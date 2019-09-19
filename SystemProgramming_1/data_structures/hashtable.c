#include "hashtable.h"

/* returns the key for a string key */
int produce_key(char *key, int size_of_hashtable)
{
    unsigned long int value = 0;
    int index = 0;

    while (value < ULONG_MAX && index < strlen(key))
    {
        value += key[index];
        index++;
    }
    return value % size_of_hashtable;
}

/* creates a hashtable of given size and initialize it */
hashtable *create_hashtable(int size_of_hashtable, int number_of_bytes)
{
    hashtable *myhashtable;
    if (size_of_hashtable > 1)
    {
        if (number_of_bytes > (sizeof(metadata) + sizeof(record)))
        {
            int number_of_records;
            number_of_bytes -= sizeof(metadata);
            number_of_records = number_of_bytes / sizeof(record);

            myhashtable = (hashtable *)malloc(sizeof(hashtable));
            if (myhashtable == NULL)
            {
                printf("Error: malloc failed at creating a new hashtable!\n");
            }
            myhashtable->hash_table = malloc(size_of_hashtable * sizeof(bucket *));
            if (myhashtable->hash_table == NULL)
            {
                printf("Error: malloc failed at creating a new hashtable!\n");
            }
            myhashtable->number_of_bytes = number_of_bytes + sizeof(metadata);
            myhashtable->number_of_records = number_of_records;
            myhashtable->size_of_hashtable = size_of_hashtable;
            bucket *initial_buckets = (bucket *)malloc(size_of_hashtable * sizeof(bucket));
            for (int i = 0; i < size_of_hashtable; i++)
            {
                initial_buckets[i].data = NULL;
                initial_buckets[i].meta_data.next_bucket = NULL;
                initial_buckets[i].meta_data.current_records = 0;
                myhashtable->hash_table[i] = &(initial_buckets[i]);
            }
        }
        else
        {
            printf("Error: the size of bucket for hashtable is very small!\n");
            return NULL;
        }
    }
    else
    {
        printf("Error: Give an integer bigger then 1 for size of hashtable!\n");
        return NULL;
    }
    return myhashtable;
}

void insert_record(hashtable **myhashtable, record myrecord, int type_of_hashtable)
{
    int index;
    record initial_record = initialize_record(type_of_hashtable);

    if (myhashtable == NULL)
    {
        printf("Error: Cannot insert records at an empty hashtable!\n");
        return;
    }
    index = produce_key(myrecord.key, (*myhashtable)->size_of_hashtable);
    if (index > (*myhashtable)->size_of_hashtable)
    {
        printf("Error: There is not enough allocated space for this index\n");
        return;
    }

    if ((*myhashtable)->hash_table[index]->meta_data.current_records == 0) // no bucket for this index
    {
        (*myhashtable)->hash_table[index]->data = (record *)malloc((*myhashtable)->number_of_records * sizeof(record)); // make a bucket
        for (int i = 0; i < (*myhashtable)->number_of_records; i++)                                                     // and initializing
        {
            (*myhashtable)->hash_table[index]->data[i] = initial_record;
        }
        (*myhashtable)->hash_table[index]->data[0] = assign_record((*myhashtable)->hash_table[index]->data[0], myrecord, type_of_hashtable);
        (*myhashtable)->hash_table[index]->meta_data.current_records += 1;
    }
    else // if there is bucket
    {
        if ((*myhashtable)->hash_table[index]->meta_data.current_records < (*myhashtable)->number_of_records) // if this bucket is not full
        {
            int i = 0;
            while (i < (*myhashtable)->number_of_records && strlen((*myhashtable)->hash_table[index]->data[i].key) != 0)
            {
                i++;
            }
            (*myhashtable)->hash_table[index]->data[i] = assign_record((*myhashtable)->hash_table[index]->data[i], myrecord, type_of_hashtable);
            (*myhashtable)->hash_table[index]->meta_data.current_records += 1;
        }
        else // if bucket is full
        {
            bucket *current_bucket = (*myhashtable)->hash_table[index];
            while (current_bucket->meta_data.next_bucket != NULL && current_bucket->meta_data.current_records == (*myhashtable)->number_of_records) // search for a bucket with empty spase for one record
            {
                current_bucket = current_bucket->meta_data.next_bucket;
            }
            if (current_bucket->meta_data.next_bucket == NULL) // all buckets are full, so make a new one at the chain
            {
                bucket *new_bucket = (bucket *)malloc(sizeof(bucket));
                new_bucket->meta_data.next_bucket = NULL;
                new_bucket->meta_data.current_records = 0;
                new_bucket->data = (record *)malloc((*myhashtable)->number_of_records * sizeof(record)); // make a bucket
                for (int i = 0; i < (*myhashtable)->number_of_records; i++)                              // and initializing
                {
                    new_bucket->data[i] = initial_record;
                }
                current_bucket->meta_data.next_bucket = new_bucket;
                current_bucket = current_bucket->meta_data.next_bucket;
            }
            int i = 0;
            while (i < (*myhashtable)->number_of_records && strlen(current_bucket->data[i].key) != 0)
            {
                i++;
            }
            current_bucket->data[i] = assign_record(current_bucket->data[i], myrecord, type_of_hashtable);
            current_bucket->meta_data.current_records += 1;
        }
    }
    return;
}

record *search_hashtable(char *key, hashtable **myhashtable)
{
    int i = 0;
    int index = produce_key(key, (*myhashtable)->size_of_hashtable);
    bucket *current_bucket = (*myhashtable)->hash_table[index];
    int equal = -1;

    if (current_bucket->meta_data.current_records == 0)
    {
        return NULL;
    }

    while (current_bucket != NULL && equal != 0)
    {
        equal = strcmp(current_bucket->data[i].key, key);
        while (i < current_bucket->meta_data.current_records && equal != 0)
        {
            i++;
            equal = strcmp(current_bucket->data[i].key, key);
        }
        if (i == current_bucket->meta_data.current_records && equal != 0)
        {
            current_bucket = current_bucket->meta_data.next_bucket;
            i = 0;
        }
    }
    if (current_bucket != NULL)
    {
        return &(current_bucket->data[i]);
    }
    else
    {
        return NULL;
    }
}

record initialize_record(int type_of_hashtable)
{
    record initial_record;

    memset(&(initial_record.key)[0], 0, sizeof(initial_record.key));
    switch (type_of_hashtable)
    {
    case WALLET_HASHTABLE:
        memset(&(initial_record.hash_value.mywallet.walletID)[0], 0, sizeof(initial_record.hash_value.mywallet.walletID));
        initial_record.hash_value.mywallet.bitcoin_and_balance = NULL;
        break;
    case SENDER_HASHTABLE:
        memset(&(initial_record.hash_value.mysender.walletID)[0], 0, sizeof(initial_record.hash_value.mysender.walletID));
        initial_record.hash_value.mysender.sending = NULL;
        initial_record.hash_value.mysender.transactions = NULL;
        break;
    case RECEIVER_HASHTABLE:
        memset(&(initial_record.hash_value.myreceiver.walletID)[0], 0, sizeof(initial_record.hash_value.myreceiver.walletID));
        initial_record.hash_value.myreceiver.receiving = NULL;
        initial_record.hash_value.myreceiver.transactions = NULL;
        break;
    default:
        break;
    }
    return initial_record;
}

record assign_record(record destination, record src, int type_of_hashtable)
{
    memcpy(&(destination.key), &(src.key), strlen(src.key) + 1);
    switch (type_of_hashtable)
    {
    case WALLET_HASHTABLE:
        memcpy(&(destination.hash_value.mywallet.sum_balance), &(src.hash_value.mywallet.sum_balance), sizeof(int));
        memcpy(&(destination.hash_value.mywallet.walletID), &(src.hash_value.mywallet.walletID), strlen(src.hash_value.mywallet.walletID));
        list_node *current = src.hash_value.mywallet.bitcoin_and_balance;
        while (current != NULL)
        {
            insert(&(destination.hash_value.mywallet.bitcoin_and_balance), &(current->data));
            current = current->next;
        }
        break;
    case SENDER_HASHTABLE:
        memcpy(&(destination.hash_value.mysender.walletID), &(src.hash_value.mysender.walletID), strlen(src.hash_value.mysender.walletID));
        insert(&(destination.hash_value.mysender.sending), &(src.hash_value.mysender.sending->data));
        insert_trans(&(destination.hash_value.mysender.transactions), &(src.hash_value.mysender.transactions->data));
        break;
    case RECEIVER_HASHTABLE:

        memcpy(&(destination.hash_value.myreceiver.walletID), &(src.hash_value.myreceiver.walletID), strlen(src.hash_value.myreceiver.walletID));
        insert(&(destination.hash_value.myreceiver.receiving), &(src.hash_value.myreceiver.receiving->data));
        insert_trans(&(destination.hash_value.myreceiver.transactions), &(src.hash_value.myreceiver.transactions->data));
        break;
    default:
        break;
    }
    return destination;
}