#include "error_handling.h"

void error_handling(int error_no)
{
    switch (error_no)
    {
    case 1:
        printf("Error: Balances error: Find the same userID\n");
        exit(1);
        break;
    case 2:
        printf("Error: Balances error: Find the same bitcoinID\n");
        exit(1);
        break;
    case 3:
        printf("Error: Transactions error: Find the same transactionID\n");
        break;
    case 4:
        printf("Error: Transactions error: Value of transaction is bigger than balance of sender\n");
        break;
    case 5:
        printf("Error: Transactions error: There is not this user\n");
        break;
    case 6:
        printf("Error: Command error: There is not this user\n");
        break;
    case 7:
        printf("Error: Command error: There is not this bitcoinID\n");
        break;
    case 8:
        printf("Error: Command error: Invalid command\n");
        break;
    case 9:
        printf("Error: Date and Time error\n");
        break;
    default:
        break;
    }
}

void valid_checking_id(list_node **checking_list, owner_bitcoin owner_of_bitcoin)
{
    union list_value data;
    memcpy(&(data.owner_of_bitcoin.walletID), owner_of_bitcoin.walletID, strlen(owner_of_bitcoin.walletID) + 1);
    if (*checking_list == NULL)
    {
        insert(checking_list, &data);
    }
    else
    {
        if (search_ID(*checking_list, owner_of_bitcoin) == NULL)
        {
            insert(checking_list, &data);
            return;
        }
        else
        {
            error_handling(1);
        }
    }
    return;
}

void valid_checking_bitcoin(list_node **bitcoinID_list, owner_bitcoin owner_of_bitcoin)
{
    union list_value data;
    data.owner_of_bitcoin.bitcoinID = owner_of_bitcoin.bitcoinID;
    memcpy(data.owner_of_bitcoin.walletID, owner_of_bitcoin.walletID, strlen(owner_of_bitcoin.walletID));
    if (*bitcoinID_list == NULL)
    {
        insert_owner_of_bitcoin(bitcoinID_list, &data);
    }
    else
    {
        if (search_bitcoinID(*bitcoinID_list, owner_of_bitcoin) == NULL)
        {
            insert_owner_of_bitcoin(bitcoinID_list, &data);
        }
        else
        {
            error_handling(2);
        }
    }
    return;
}

bool valid_checking_transactionID(list_node **transactionID_list, char *transaction_ID)
{
    union list_value data;
    memcpy(&(data.transactionID), transaction_ID, strlen(transaction_ID) + 1);
    if (*transactionID_list == NULL)
    {
        insert(transactionID_list, &data);
        return true;
    }
    else
    {
        if (search_transactionID(*transactionID_list, transaction_ID) == NULL)
        {
            insert(transactionID_list, &data);
            return true;
        }
        else
        {
            error_handling(3);
            return false;
        }
    }
}

bool valid_checking_sum_balance(wallet mywallet, int required_balance)
{
    if (mywallet.sum_balance >= required_balance)
    {
        return true;
    }
    else
    {
        error_handling(4);
        return false;
    }
}

bool valid_checking_sender_receiver(transaction mytransaction, hashtable **wallets)
{
    if (search_hashtable(mytransaction.senderWalletID, wallets) == NULL)
    {
        error_handling(5);
        return false;
    }

    if (search_hashtable(mytransaction.receiverWalletID, wallets) == NULL)
    {
        error_handling(5);
        return false;
    }
    return true;
}

bool valid_checking_walletID(char *key, hashtable **wallets)
{
    if (search_hashtable(key, wallets) == NULL)
    {
        error_handling(6);
        return false;
    }
    return true;
}

bool valid_checking_date_and_time(char *old_date, char *old_time, char *new_date, char *new_time)
{
    if(compare_moments(old_date, old_time, new_date, new_time) == true)
    {
        return true;
    }
    else
    {
        error_handling(9);
        return false;
    }
}