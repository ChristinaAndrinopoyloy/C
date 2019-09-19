#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "./helpers/read_data.h"
#include "./helpers/transaction_routines.h"
#include "./data_structures/hashtable.h"
#include "./data_structures/linked_list.h"
#include "./data_structures/tree.h"
#include "./user_interaction/user_interaction.h"

#define BALANCE_TXT 0
#define TRANSACTION_TXT 1

list_node *bitcoinID_list = NULL;
int size_of_bitcoin_trees;
char app_last_date[11];
char app_last_time[6];
int app_transactionID = 0;

int main(int argc, char *argv[])
{
    int opt, bitCoin_value = 10;
    int bucket_size = 500;
    char *balance_file = "bitCoinBalancesFile.txt";
    char *transactions_file = "transactionsFile.txt";
    int sender_entries = 2;
    int receiver_entries = 2;
    tree *bitcoin_trees;

    while ((opt = getopt(argc, argv, ":a:t:v:r:s:p:b:")) != -1)
    {
        switch (opt)
        {
        case 'a':
            balance_file = optarg;
            printf("bitCoinBalancesFile: %s\n", balance_file);
            break;
        case 't':
            transactions_file = optarg;
            printf("transactionsFile: %s\n", transactions_file);
            break;
        case 'v':
            bitCoin_value = atoi(optarg);
            printf("bitCoinValue: %d\n", bitCoin_value);
            break;
        case 'r':
            sender_entries = atoi(optarg);
            printf("senderHashtableNumOfEntries: %d\n", sender_entries);
            break;
        case 's':
            receiver_entries = atoi(optarg);
            printf("receiverHashtableNumOfEntries: %d\n", receiver_entries);
            break;
        case 'b':
            bucket_size = atoi(optarg);
            printf("bucketSize: %d\n", bucket_size);
            break;
        case ':':
            printf("Error: argument without value\n");
            return 1;
            break;
        case '?':
            printf("Error: unknown argument % c\n", optopt);
            return 1;
            break;
        }
    }
    while (bucket_size < (sizeof(metadata) + sizeof(record)))
    {
        bucket_size += bucket_size;
    }
    hashtable *wallets = create_hashtable(sender_entries, bucket_size);
    hashtable *senders = create_hashtable(sender_entries, bucket_size);
    hashtable *receivers = create_hashtable(receiver_entries, bucket_size);
    read_and_initialize(balance_file, BALANCE_TXT, &wallets, &senders, &receivers, bitCoin_value, NULL, 0);

    bitcoin_trees = (tree *)malloc(size_of_bitcoin_trees * sizeof(tree));
    tree_initialization(bitcoin_trees, bitcoinID_list, bitCoin_value, &wallets);

    read_and_initialize(transactions_file, TRANSACTION_TXT, &wallets, &senders, &receivers, bitCoin_value, bitcoin_trees, size_of_bitcoin_trees);

    while (true)
    {
        user_interaction(&wallets, &senders, &receivers, bitcoin_trees, size_of_bitcoin_trees);
    }

    printf("END OF MAIN\n");
    return 0;
}