#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

#include "./data_structures/linked_list.h"

#define WALLET_HASHTABLE -10
#define SENDER_HASHTABLE -20
#define RECEIVER_HASHTABLE -30

typedef struct ListNode list_node;

typedef struct Wallet
{
    char walletID[50];
    int sum_balance;
    list_node *bitcoin_and_balance;
} wallet;

typedef struct Sender
{
    char walletID[50];
    list_node *sending;
    list_node *transactions;
} sender;

typedef struct Receiver
{
    char walletID[50];
    list_node *receiving;
    list_node *transactions;
} receiver;

typedef struct Mydate
{
    int day;
    int month;
    int year;
} my_date;

typedef struct Mytime
{
    int hour;
    int minute;
} my_time;

typedef struct Transaction
{
    char transactionID[15];
    char senderWalletID[50];
    char receiverWalletID[50];
    int value;
    char date_of_transaction[11];
    char time_of_transaction[6];
    list_node *bitcoins_of_transaction;
} transaction;

////////////////////////////////////


#endif