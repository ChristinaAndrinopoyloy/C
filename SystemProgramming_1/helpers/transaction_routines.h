#ifndef _TRANSACTION_ROUTINES_H_
#define _TRANSACTION_ROUTINES_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../structures.h"
#include "../data_structures/hashtable.h"
#include "../data_structures/linked_list.h"
#include "../data_structures/tree.h"
#include "../error_handling/error_handling.h"

bool update_system(transaction mytransaction, hashtable **wallets, hashtable **senders, hashtable **receivers, tree *bitcoin_trees, int sze_of_bitcoin_tress);
list_node *appropriate_bitcoin_finder(int balance, wallet sender_wallet);
void update(transaction mytransaction, hashtable **wallets, hashtable **senders, hashtable **receivers, tree *bitcoin_trees, int size_of_bitcoin_trees);
void initialization_tree_nodes(tree_node **old_node, tree_node **new_node, transaction mytransaction, tree current_bitcoin_tree, tree_node* start, list_node *current);
void update_root(transaction mytransaction, hashtable **wallets, hashtable **senders, hashtable **receivers, tree current_bitcoin_trees, list_node *current);
void update_NOT_root(transaction mytransaction, hashtable **wallets, hashtable **senders, hashtable **receivers, tree current_bitcoin_trees, list_node *current);

void search_sender_at_senders(transaction mytransaction, hashtable **senders, hashtable **wallets, tree_node *old_node, tree_node *new_node, union list_value *old, union list_value *neww, union list_value transaction_node, tree current_bitcoin_trees, list_node *current, record *mysender);
void search_receiver_at_receivers(transaction mytransaction, hashtable **receivers, tree_node *new_node, union list_value *neww, union list_value transaction_node, list_node *current);
void search_receiver_at_wallets(transaction mytransaction, hashtable **wallets, tree_node *new_node, tree current_bitcoin_trees, list_node *current);
void update_senders_and_wallets(hashtable **wallets, record *found_sender, record *mysender, transaction mytransaction, tree_node *old_node, tree_node *new_node, union list_value *old, union list_value transaction_node, tree current_bitcoin_trees, list_node *current);

#endif