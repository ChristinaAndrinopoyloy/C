#ifndef _ERROR_HANDLING_H_
#define _ERROR_HANDLING_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../data_structures/linked_list.h"
#include "../data_structures/hashtable.h"
#include "../date_time_library/date_time_library.h"

void error_handling(int error_no);
void valid_checking_id(list_node **checking_list, owner_bitcoin owner_of_bitcoin);
void valid_checking_bitcoin(list_node **bitcoinID_list, owner_bitcoin owner_of_bitcoin);
bool valid_checking_transactionID(list_node **transactionID_list, char *transactionID);
bool valid_checking_sum_balance(wallet mywallet, int required_balance);
bool valid_checking_sender_receiver(transaction mytransaction, hashtable **wallets);
bool valid_checking_walletID(char *key, hashtable **wallets);
bool valid_checking_date_and_time(char *old_date, char *old_time, char *new_date, char *new_time);

#endif