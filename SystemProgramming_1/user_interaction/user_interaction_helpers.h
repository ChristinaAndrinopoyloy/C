#ifndef _USER_INTERACTION_HELPERS_H_
#define _USER_INTERACTION_HELPERS_H_

#include "./../data_structures/linked_list.h"
#include "./../data_structures/hashtable.h"
#include "./../date_time_library/date_time_library.h"

void print_earnings_payments(list_node *head, char *down_date, char *down_time, char *up_date, char *up_time);
void clear_wallets(hashtable **wallets);
void clear_senders(hashtable **senders);
void clear_receivers(hashtable **receivers);
void clear_bitcoin_trees(tree *bitcoin_trees, int size_of_bitcoin_trees);

#endif