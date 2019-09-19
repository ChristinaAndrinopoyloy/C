#ifndef _USER_INTERACTION_H_
#define _USER_INTERACTION_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "user_interaction_helpers.h"
#include "./../data_structures/tree.h"
#include "./../data_structures/hashtable.h"
#include "./../error_handling/error_handling.h"
#include "./../date_time_library/date_time_library.h"
#include "./../helpers/transaction_routines.h"

#define BUFFER_SIZE 200

void user_interaction(hashtable **wallets, hashtable **senders, hashtable **receivers, tree *bitcoin_trees, int size_of_bitcoin_trees);
void requestTransaction(char *token, hashtable **wallets, hashtable **senders, hashtable **receivers, tree *bitcoin_trees, int size_of_bitcoin_trees, int requestTransactions);
void requestTransactions(char *command, hashtable **wallets, hashtable **senders, hashtable **receivers, tree *bitcoin_trees, int size_of_bitcoin_trees, int number_of_args);
void requestTransactions_from_file(char *command, hashtable **wallets, hashtable **senders, hashtable **receivers, tree *bitcoin_trees, int size_of_bitcoin_trees, int number_of_args);
void findEarnings(hashtable **receivers, char *token);
void findPayments(hashtable **senders, char *token);
void walletStatus(char *token, hashtable **wallets);
void bitCoinStatus(char *token, int size_of_bitcoin_trees, tree *bitcoin_trees);
void traceCoin(char *token, int size_of_bitcoin_trees, tree *bitcoin_trees);
void myexit(hashtable **wallets, hashtable **senders, hashtable **receivers, tree *bitcoin_trees, int size_of_bitcoin_trees);

#endif