#include "user_interaction_helpers.h"

void print_earnings_payments(list_node *head, char *down_date, char *down_time, char *up_date, char *up_time)
{
    list_node *current = head;
    char *previous = "none";
    int sum = 0;
    char *current_date;
    char *current_time;
    while (current != NULL)
    {
        if (down_date == NULL && down_time == NULL && up_date == NULL & up_time == NULL)
        {
            sum += current->data.ptr_tree->ptr_tree->value.value_of_bitcoin;
            if (strcmp(current->data.ptr_tree->ptr_tree->value.transaction_of_tree_node->transactionID, previous) != 0)
            {
                printf("%s %s %s %d %s %s\n", current->data.ptr_tree->ptr_tree->value.transaction_of_tree_node->transactionID,
                       current->data.ptr_tree->ptr_tree->value.transaction_of_tree_node->senderWalletID,
                       current->data.ptr_tree->ptr_tree->value.transaction_of_tree_node->receiverWalletID,
                       current->data.ptr_tree->ptr_tree->value.transaction_of_tree_node->value,
                       current->data.ptr_tree->ptr_tree->value.transaction_of_tree_node->date_of_transaction,
                       current->data.ptr_tree->ptr_tree->value.transaction_of_tree_node->time_of_transaction);
            }
            previous = current->data.ptr_tree->ptr_tree->value.transaction_of_tree_node->transactionID;
        }
        else
        {
            current_date = current->data.ptr_tree->ptr_tree->value.transaction_of_tree_node->date_of_transaction;
            current_time = current->data.ptr_tree->ptr_tree->value.transaction_of_tree_node->time_of_transaction;
            if (time_borders(down_date, down_time, up_date, up_time, current_date, current_time))
            {
                sum += current->data.ptr_tree->ptr_tree->value.value_of_bitcoin;
                if (strcmp(current->data.ptr_tree->ptr_tree->value.transaction_of_tree_node->transactionID, previous) != 0)
                {
                    printf("%s %s %s %d %s %s\n", current->data.ptr_tree->ptr_tree->value.transaction_of_tree_node->transactionID,
                           current->data.ptr_tree->ptr_tree->value.transaction_of_tree_node->senderWalletID,
                           current->data.ptr_tree->ptr_tree->value.transaction_of_tree_node->receiverWalletID,
                           current->data.ptr_tree->ptr_tree->value.transaction_of_tree_node->value,
                           current->data.ptr_tree->ptr_tree->value.transaction_of_tree_node->date_of_transaction,
                           current->data.ptr_tree->ptr_tree->value.transaction_of_tree_node->time_of_transaction);
                }
                previous = current->data.ptr_tree->ptr_tree->value.transaction_of_tree_node->transactionID;
            }
        }
        current = current->next;
    }
    printf("Sum of transactions is %d\n", sum);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void clear_wallets(hashtable **wallets)
{
    bucket *current_bucket = NULL;
    bucket *previous_bucket = NULL;
    for (int i = 0; i < (*wallets)->size_of_hashtable; i++)
    {
        current_bucket = (*wallets)->hash_table[i]->meta_data.next_bucket;
        while (current_bucket != NULL)
        {
            for (int j = 0; j < current_bucket->meta_data.current_records; j++)
            {
                free_list(current_bucket->data[j].hash_value.mywallet.bitcoin_and_balance, BITCOIN_AND_BALANCE);
            }
            free(current_bucket->data);
            previous_bucket = current_bucket;
            current_bucket = current_bucket->meta_data.next_bucket;
            free(previous_bucket);
        }
    }
    free((*wallets));
}

void clear_senders(hashtable **senders)
{
    bucket *current_bucket = NULL;
    bucket *previous_bucket = NULL;
    for (int i = 0; i < (*senders)->size_of_hashtable; i++)
    {
        current_bucket = (*senders)->hash_table[i]->meta_data.next_bucket;
        while (current_bucket != NULL)
        {
            for (int j = 0; j < current_bucket->meta_data.current_records; j++)
            {
                free_list(current_bucket->data[j].hash_value.mysender.sending, 0);
                free_list(current_bucket->data[j].hash_value.mysender.transactions, TRANSACT);
            }
            free(current_bucket->data);
            previous_bucket = current_bucket;
            current_bucket = current_bucket->meta_data.next_bucket;
            free(previous_bucket);
        }
    }
    free((*senders));
}

void clear_receivers(hashtable **receivers)
{
    bucket *current_bucket = NULL;
    bucket *previous_bucket = NULL;
    for (int i = 0; i < (*receivers)->size_of_hashtable; i++)
    {
        current_bucket = (*receivers)->hash_table[i]->meta_data.next_bucket;
        while (current_bucket != NULL)
        {
            for (int j = 0; j < current_bucket->meta_data.current_records; j++)
            {
                free_list(current_bucket->data[j].hash_value.myreceiver.receiving, 0);
                free_list(current_bucket->data[j].hash_value.myreceiver.transactions, TRANSACT);
            }
            free(current_bucket->data);
            previous_bucket = current_bucket;
            current_bucket = current_bucket->meta_data.next_bucket;
            free(previous_bucket);
        }
    }
    free((*receivers));
}

void clear_bitcoin_trees(tree *bitcoin_trees, int size_of_bitcoin_trees)
{
    for(int i = 0; i < size_of_bitcoin_trees; i++)
    {
        free_tree(bitcoin_trees[i].root);
    }
    free(bitcoin_trees);
}
