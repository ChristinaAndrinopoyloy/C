#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#define TRANSACT 1000
#define BITCOIN_AND_BALANCE 2000

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../structures.h"
#include "tree.h"

typedef struct Transaction transaction;
typedef struct Tree_node tree_node;

typedef struct Owner_bitcoin
{
    int bitcoinID;
    char walletID[50];
} owner_bitcoin;

typedef struct Bitcoin_balance
{
    int bitcoinID;
    int balance_of_bitcoin;
    tree_node *root_of_bitcoin_tree;
}bitcoin_balance;

typedef struct PRT_tree
{
    tree_node *ptr_tree;
    int bitcoinID;
}pointer_to_tree;

union list_value {
    struct Transaction *transact;
    owner_bitcoin owner_of_bitcoin;
    char transactionID[15];
    bitcoin_balance bitcoin_and_balance;
    pointer_to_tree *ptr_tree;
};

typedef struct ListNode
{
    union list_value data;
    struct ListNode *next;
} list_node;

bool empty(list_node *head);
int length(list_node *head);
list_node *search_transaction(list_node *head, struct Transaction search_transaction);
list_node *search_ID(list_node *head, owner_bitcoin owner_of_bitcoin);
list_node *search_bitcoinID(list_node *head, owner_bitcoin owner_of_bitcoin);
list_node *search_bitcoin_ID(list_node *head, int id);
void insert_owner_of_bitcoin(list_node **head, union list_value *data);
list_node *search_transactionID(list_node *head, char *id);
list_node *search_bitcoin_balance(list_node *head, int balance_of_bitcoin, int *initial_value_of_bitcoin);
void insert(list_node **head, union list_value *data);
void insert_trans(list_node **head, union list_value *data);
void print_list(list_node *head, int type);
void free_list(list_node *head, int type);

#endif