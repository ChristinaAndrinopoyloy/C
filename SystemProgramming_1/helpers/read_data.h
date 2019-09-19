#ifndef _READ_DATA_H_
#define _READ_DATA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transaction_routines.h"
#include "../structures.h"
#include "../data_structures/hashtable.h"
#include "../data_structures/linked_list.h"
#include "../data_structures/tree.h"
#include "../error_handling/error_handling.h"
#include "../date_time_library/date_time_library.h"

struct temp_user
{
    char username[50];
    int *bitcoins;
};

void read_and_initialize(char *file_to_open, char kind_of_txt, hashtable **wallets, hashtable **senders, hashtable **receivers, int balance, tree *bitcoin_trees, int size_of_bitcoin_trees);
void split(char *line, char kind_of_text, hashtable **wallets, hashtable **senders, hashtable **receivers, int balance, list_node **checking_list, list_node **bitcoinID_list, list_node **transactionID_list, tree *bitcoin_trees, int size_of_bitcoin_trees);
struct temp_user read_balances(char *line, int number_of_bitcoins, list_node **checking_list, list_node **bitcoinID_list);
transaction read_transactions(char *line, list_node **transactionID_list);
void tree_initialization(tree *bitcoin_trees, list_node *bitcoinID_list, int value, hashtable **wallets);

#endif