#include "linked_list.h"

//returns true if the list is empty
bool empty(list_node *head)
{
    return head == NULL;
}

// returns the length of a list
int length(list_node *head)
{
    int length = 0;
    list_node *current;

    for (current = head; current->next != NULL; current = current->next)
    {
        length++;
    }

    return length;
}

//search for a given transaction
list_node *search_transaction(list_node *head, struct Transaction search_transaction)
{
    list_node *current = head;
    if (head == NULL)
    {
        return NULL;
    }

    while (strcmp(current->data.transact->transactionID, search_transaction.transactionID) != 0)
    {
        if (current->next == NULL)
        {
            printf("There isn't this transaction!\n");
            return NULL;
        }
        else
        {
            current = current->next;
        }
    }
    return current;
}

//search for a given walletID
list_node *search_ID(list_node *head, owner_bitcoin owner_of_bitcoin)
{
    list_node *current = head;
    if (head == NULL)
    {
        printf("Error: The list is empty. Cannot find any transaction! \n");
        return NULL;
    }

    while (strcmp(owner_of_bitcoin.walletID, current->data.owner_of_bitcoin.walletID) != 0)
    {
        if (current->next == NULL)
        {
            return NULL;
        }
        else
        {
            current = current->next;
        }
    }
    return current;
}

//search for a given bitcoinID
list_node *search_bitcoinID(list_node *head, owner_bitcoin owner_of_bitcoin)
{
    list_node *current = head;
    if (head == NULL)
    {
        printf("Error: The list is empty. Cannot find any transaction! \n");
        return NULL;
    }

    while (owner_of_bitcoin.bitcoinID != current->data.owner_of_bitcoin.bitcoinID)
    {
        if (current->next == NULL)
        {
            return NULL;
        }
        else
        {
            current = current->next;
        }
    }
    return current;
}

list_node *search_bitcoin_ID(list_node *head, int id)
{
    list_node *current = head;
    if (head == NULL)
    {
        printf("Error: The list is empty. Cannot find any transaction! \n");
        return NULL;
    }

    while (id != current->data.ptr_tree->bitcoinID)
    {
        if (current->next == NULL)
        {
            return NULL;
        }
        else
        {
            current = current->next;
        }
    }
    return current;
}

//search for a given ID
list_node *search_transactionID(list_node *head, char *id)
{
    list_node *current = head;
    if (head == NULL)
    {
        printf("Error: The list is empty. Cannot find any transaction! \n");
        return NULL;
    }

    while (strcmp(id, current->data.transactionID) != 0)
    {
        if (current->next == NULL)
        {
            return NULL;
        }
        else
        {
            current = current->next;
        }
    }
    return current;
}

//search (by balance of bitcoin) for a bitcoin
list_node *search_bitcoin_balance(list_node *head, int balance_of_bitcoin, int *initial_value_of_bitcoin)
{
    list_node *current = head;
    if (head == NULL)
    {
        return NULL;
    }

    while (balance_of_bitcoin > current->data.bitcoin_and_balance.balance_of_bitcoin)
    {
        if (current->next == NULL)
        {
            current = head;
            while (current->data.bitcoin_and_balance.balance_of_bitcoin == 0)
            {
                current = current->next;
            }
            (*initial_value_of_bitcoin) = current->data.bitcoin_and_balance.balance_of_bitcoin;
            if (balance_of_bitcoin < current->data.bitcoin_and_balance.balance_of_bitcoin)
            {
                current->data.bitcoin_and_balance.balance_of_bitcoin -= balance_of_bitcoin;
            }
            else
            {
                current->data.bitcoin_and_balance.balance_of_bitcoin = 0;
            }
            return current;
        }
        else
        {
            current = current->next;
        }
    }
    (*initial_value_of_bitcoin) = current->data.bitcoin_and_balance.balance_of_bitcoin;
    current->data.bitcoin_and_balance.balance_of_bitcoin -= balance_of_bitcoin;
    return current;
}

//insert a node at the end of the list
void insert(list_node **head, union list_value *data)
{
    list_node *current = *head;

    if (*head == NULL)
    {
        *head = (list_node *)malloc(sizeof(list_node));
        memcpy(&((*(head))->data), data, sizeof(data));
        (*(head))->next = NULL;
        return;
    }
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = (list_node *)malloc(sizeof(list_node));
    memcpy(&(current->next->data), data, sizeof(data));
    (current->next)->next = NULL;

    return;
}

void insert_owner_of_bitcoin(list_node **head, union list_value *data)
{
    list_node *current = *head;

    if (*head == NULL)
    {
        *head = (list_node *)malloc(sizeof(list_node));
        (*(head))->data.owner_of_bitcoin.bitcoinID = data->owner_of_bitcoin.bitcoinID;
        memcpy(&((*(head))->data).owner_of_bitcoin.walletID, data->owner_of_bitcoin.walletID, sizeof(data->owner_of_bitcoin.walletID));
        (*(head))->next = NULL;
        return;
    }
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = (list_node *)malloc(sizeof(list_node));
    current->next->data.owner_of_bitcoin.bitcoinID = data->owner_of_bitcoin.bitcoinID;
    memcpy(&(current->next->data).owner_of_bitcoin.walletID, data->owner_of_bitcoin.walletID, sizeof(data->owner_of_bitcoin.walletID));
    (current->next)->next = NULL;

    return;
}

void insert_trans(list_node **head, union list_value *data)
{
    list_node *current = *head;

    if (*head == NULL)
    {
        *head = (list_node *)malloc(sizeof(list_node));
        (*(head))->data.transact = (transaction *)malloc(sizeof(transaction));
        memcpy(&((*(head))->data.transact->transactionID), data->transact->transactionID, strlen(data->transact->transactionID) + 1);
        memcpy(&((*(head))->data.transact->senderWalletID), data->transact->senderWalletID, strlen(data->transact->senderWalletID) + 1);
        memcpy(&((*(head))->data.transact->receiverWalletID), data->transact->receiverWalletID, strlen(data->transact->receiverWalletID) + 1);
        memcpy(&((*(head))->data.transact->date_of_transaction), data->transact->date_of_transaction, strlen(data->transact->date_of_transaction) + 1);
        memcpy(&((*(head))->data.transact->time_of_transaction), data->transact->time_of_transaction, strlen(data->transact->time_of_transaction) + 1);
        (*(head))->data.transact->value = data->transact->value;
        (*(head))->next = NULL;
        return;
    }
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = (list_node *)malloc(sizeof(list_node));
    current->next->data.transact = (transaction *)malloc(sizeof(transaction));

    memcpy(current->next->data.transact->transactionID, data->transact->transactionID, strlen(data->transact->transactionID) + 1);
    memcpy(current->next->data.transact->senderWalletID, data->transact->senderWalletID, strlen(data->transact->senderWalletID) + 1);
    memcpy(current->next->data.transact->receiverWalletID, data->transact->receiverWalletID, strlen(data->transact->receiverWalletID) + 1);
    memcpy(current->next->data.transact->date_of_transaction, data->transact->date_of_transaction, strlen(data->transact->date_of_transaction) + 1);
    memcpy(current->next->data.transact->time_of_transaction, data->transact->time_of_transaction, strlen(data->transact->time_of_transaction) + 1);
    current->next->data.transact->value = data->transact->value;
    (current->next)->next = NULL;

    return;
}

void free_list(list_node *head, int type)
{
    list_node *current;

    while (head != NULL)
    {
        current = head;
        head = head->next;

        if (type == TRANSACT)
        {
            free_list(current->data.transact->bitcoins_of_transaction, 0);
            free(current->data.transact);
        }
        else if (type == BITCOIN_AND_BALANCE)
        {
            free_tree(current->data.bitcoin_and_balance.root_of_bitcoin_tree);
        }
        free(current);
    }
}