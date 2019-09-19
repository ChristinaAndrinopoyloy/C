#include "user_interaction.h"

void user_interaction(hashtable **wallets,
                      hashtable **senders,
                      hashtable **receivers,
                      tree *bitcoin_trees, int size_of_bitcoin_trees)
{
    char command[BUFFER_SIZE];
    char character;
    char *token;
    const char delimeter[2] = " ";
    int i = 0;
    int number_of_args = 0;
    bool valid_command = false;

    printf("Give a command\n");
    while ((character = getchar()) != '\n')
    {
        command[i++] = character;
    }
    command[i] = '\0';

    puts("\nYou give: ");
    puts(command);

    char *temp_command = (char *)malloc(strlen(command) * sizeof(char));
    memcpy(temp_command, command, strlen(command) + 1);
    token = strtok(command, delimeter);
    if (strcmp(token, "requestTransaction") == 0)
    {
        requestTransaction(token, wallets, senders, receivers, bitcoin_trees, size_of_bitcoin_trees, false);
        valid_command = true;
        return;
    }

    if (strcmp(token, "requestTransactions") == 0)
    {
        int number_of_args = 0;
        while (token != NULL)
        {
            token = strtok(NULL, delimeter);
            if (token != NULL)
            {
                number_of_args++;
            }
        }
        printf("%d\n", number_of_args);
        if (number_of_args == 1) // read transactions from input file
        {
            requestTransactions_from_file(temp_command, wallets, senders, receivers, bitcoin_trees, size_of_bitcoin_trees, number_of_args);
        }
        else // read transactions from stdin
        {
            printf("%s\n", temp_command);
            requestTransactions(temp_command, wallets, senders, receivers, bitcoin_trees, size_of_bitcoin_trees, number_of_args);
        }
        valid_command = true;
        return;
    }

    if (strcmp(token, "findEarnings") == 0)
    {
        findEarnings(receivers, token);
        valid_command = true;
        return;
    }

    if (strcmp(token, "findPayments") == 0)
    {
        findPayments(senders, token);
        valid_command = true;
        return;
    }

    if (strcmp(token, "walletStatus") == 0)
    {
        walletStatus(token, wallets);
        valid_command = true;
        return;
    }

    if (strcmp(token, "bitCoinStatus") == 0)
    {
        bitCoinStatus(token, size_of_bitcoin_trees, bitcoin_trees);
        valid_command = true;
        return;
    }

    if (strcmp(token, "traceCoin") == 0)
    {
        traceCoin(token, size_of_bitcoin_trees, bitcoin_trees);
        valid_command = true;
        return;
    }

    if (strcmp(token, "exit") == 0)
    {
        myexit(wallets, senders, receivers, bitcoin_trees, size_of_bitcoin_trees);
        valid_command = true;
        exit(0);
    }

    if (valid_command == false)
    {
        error_handling(8);
    }

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void requestTransaction(char *token,
                        hashtable **wallets,
                        hashtable **senders,
                        hashtable **receivers,
                        tree *bitcoin_trees, int size_of_bitcoin_trees, int requestTransactions)
{
    int argument = 1;
    transaction mytransaction;
    extern char app_last_date[11];
    extern char app_last_time[6];
    extern int app_transactionID;
    const char delimeter[2] = " ";

    if (requestTransactions == 1)
    {
        token = strtok(token, delimeter);
        token = strtok(NULL, delimeter);
        memcpy(&(mytransaction.senderWalletID)[0], token, strlen(token) + 1);
        argument = 2;
    }

    if (requestTransactions == 2)
    {
        char *backup_command = (char *)malloc((strlen(token) + 1) * sizeof(char));
        memcpy(backup_command, token, strlen(token) + 1);
        token = strtok(backup_command, delimeter);
        memcpy(&(mytransaction.senderWalletID)[0], token, strlen(token) + 1);
        argument = 2;
    }

    while (token != NULL)
    {
        token = strtok(NULL, delimeter);
        if (token != NULL)
        {
            switch (argument)
            {
            case 1:
                memcpy(&(mytransaction.senderWalletID)[0], token, strlen(token) + 1);
                break;
            case 2:
                memcpy(&(mytransaction.receiverWalletID)[0], token, strlen(token) + 1);
                break;
            case 3:
                mytransaction.value = atoi(token);
                break;
            case 4:
                memcpy(&(mytransaction.date_of_transaction)[0], token, strlen(token) + 1);
                break;
            case 5:
                memcpy(&(mytransaction.time_of_transaction)[0], token, strlen(token) + 1);
                break;
            case 6:
                token = NULL;
                break;
            default:
                break;
            }
            argument++;
        }
    }
    if (argument < 4) // not appropriate arguments
    {
        error_handling(8);
        return;
    }
    else if (argument == 4) // lets make the moment, because we dont have date and time from user
    {
        memcpy(&(mytransaction.date_of_transaction)[0], current_date_function(), 11);
        memcpy(&(mytransaction.time_of_transaction)[0], current_time_function(), 6);
    }
    else // check date and time from user
    {
        if (valid_checking_date_and_time(app_last_date, app_last_time, mytransaction.date_of_transaction, mytransaction.time_of_transaction) == false)
        {
            error_handling(8);
            return;
        }
    }
    app_transactionID += 1;
    memcpy(app_last_date, mytransaction.date_of_transaction, 11);
    memcpy(app_last_time, mytransaction.time_of_transaction, 6);
    sprintf(mytransaction.transactionID, "%d", app_transactionID);
    printf("YOU GIVE TRANSACTION\n");
    printf("transactionID %s sender %s receiver %s amount %d moment %s %s\n",
           mytransaction.transactionID, mytransaction.senderWalletID, mytransaction.receiverWalletID,
           mytransaction.value, mytransaction.date_of_transaction, mytransaction.time_of_transaction);
    if (update_system(mytransaction, wallets, senders, receivers, bitcoin_trees, size_of_bitcoin_trees) == false)
    {
        return;
    }
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void requestTransactions(char *command,
                         hashtable **wallets,
                         hashtable **senders,
                         hashtable **receivers,
                         tree *bitcoin_trees, int size_of_bitcoin_trees, int number_of_args)
{

    char delimeter[2] = ";";
    char *backup_command = (char *)malloc((strlen(command) + 1) * sizeof(char));
    memcpy(backup_command, command, strlen(command) + 1);

    char *token = strtok(backup_command, delimeter);
    token = strtok(backup_command, " ");
    while (token != NULL)
    {
        token = strtok(token + strlen(token) + 1, delimeter);
        if (token != NULL)
        {
            requestTransaction(token, wallets, senders, receivers, bitcoin_trees, size_of_bitcoin_trees, 2);
            printf("%s\n", token);
        }
    }
    return;
}

void requestTransactions_from_file(char *command,
                                   hashtable **wallets,
                                   hashtable **senders,
                                   hashtable **receivers,
                                   tree *bitcoin_trees, int size_of_bitcoin_trees, int number_of_args)
{
    char *backup_command = (char *)malloc((strlen(command) + 1) * sizeof(char));
    memcpy(backup_command, command, strlen(command) + 1);
    char *token = strtok(backup_command, " ");
    token = strtok(token + strlen(token) + 1, " ");
    char character;
    FILE *fp;
    long size_of_file;
    char *command_file;
    size_t res;

    fp = fopen(token, "r+");
    if (fp == NULL)
    {
        printf("Error: Can't find %s \n", token);
        return;
    }

    fseek(fp, 0, SEEK_END);
    size_of_file = ftell(fp);
    rewind(fp);

    command_file = (char *)malloc(size_of_file * sizeof(char));
    if (command_file == NULL)
    {
        error_handling(10);
        return;
    }

    res = fread(command_file, 1, size_of_file, fp);
    if (res != size_of_file)
    {
        printf("Error: fread error\n");
        return;
        ;
    }
    char *token2;
    token2 = strtok(command_file, ";");
    requestTransaction(token2, wallets, senders, receivers, bitcoin_trees, size_of_bitcoin_trees, 2);
    while (token2 != NULL)
    {
        token2 = strtok(token2 + strlen(token2) + 1, ";");
        if (token2 != NULL)
        {
            requestTransaction(token2, wallets, senders, receivers, bitcoin_trees, size_of_bitcoin_trees, 2);
            printf("%s\n", token);
        }
    }

    fclose(fp);
    free(command_file);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void findEarnings(hashtable **receivers, char *token)
{
    char *walletID = NULL;
    char *down_time = NULL;
    char *up_time = NULL;
    char *down_date = NULL;
    char *up_date = NULL;
    const char delimeter[2] = " ";
    int number_of_args = 1;
    while (token != NULL)
    {
        token = strtok(NULL, delimeter);
        if (token != NULL)
        {
            switch (number_of_args)
            {
            case 1:
                walletID = token;
                break;
            case 2:
                down_time = token;
                break;
            case 3:
                down_date = token;
                break;
            case 4:
                up_time = token;
                break;
            case 5:
                up_date = token;
                break;
            default:
                break;
            }
            number_of_args++;
        }
    }
    if (number_of_args == 3)
    {
        error_handling(8);
        return;
    }
    else if (number_of_args == 4)
    {
        up_time = down_date;
        down_date = current_date_function();
        up_date = current_date_function();
    }
    else if (number_of_args == 2)
    {
        down_time = NULL;
        down_date = NULL;
        up_time = NULL;
        up_date = NULL;
    }

    record *receiver = search_hashtable(walletID, receivers);
    if (receiver != NULL)
    {
        printf("%s\nTRANSACTIONS:\n", receiver->hash_value.myreceiver.walletID);
        print_earnings_payments(receiver->hash_value.myreceiver.receiving, down_date, down_time, up_date, up_time);
    }
    else
    {
        error_handling(6);
    }

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void findPayments(hashtable **senders, char *token)
{
    char *walletID = NULL;
    char *down_time = NULL;
    char *up_time = NULL;
    char *down_date = NULL;
    char *up_date = NULL;
    const char delimeter[2] = " ";
    int number_of_args = 1;
    while (token != NULL)
    {
        token = strtok(NULL, delimeter);
        if (token != NULL)
        {
            switch (number_of_args)
            {
            case 1:
                walletID = token;
                break;
            case 2:
                down_time = token;
                break;
            case 3:
                down_date = token;
                break;
            case 4:
                up_time = token;
                break;
            case 5:
                up_date = token;
                break;
            default:
                break;
            }
            number_of_args++;
        }
    }
    if (number_of_args == 3)
    {
        error_handling(8);
        return;
    }
    else if (number_of_args == 4)
    {
        up_time = down_date;
        down_date = current_date_function();
        up_date = current_date_function();
    }
    else if (number_of_args == 2)
    {
        down_time = NULL;
        down_date = NULL;
        up_time = NULL;
        up_date = NULL;
    }

    record *sender = search_hashtable(walletID, senders);
    if (sender != NULL)
    {
        printf("%s\nTRANSACTIONS:\n", sender->hash_value.mysender.walletID);
        print_earnings_payments(sender->hash_value.mysender.sending, down_date, down_time, up_date, up_time);
    }
    else
    {
        error_handling(6);
    }

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void walletStatus(char *token, hashtable **wallets)
{
    char *walletID = NULL;
    int walletStatus;
    const char delimeter[2] = " ";

    while (token != NULL)
    {
        token = strtok(NULL, delimeter);
        if (token != NULL)
        {
            walletID = token;
        }
    }
    if (valid_checking_walletID(walletID, wallets))
    {
        walletStatus = search_hashtable(walletID, wallets)->hash_value.mywallet.sum_balance;
        printf("walletStatus of %s is %d$\n", walletID, walletStatus);
    }
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void bitCoinStatus(char *token, int size_of_bitcoin_trees, tree *bitcoin_trees)
{
    int bitcoinID, number_of_trasactions;
    int i = 0;
    tree_node *unspent;
    const char delimeter[2] = " ";

    while (token != NULL)
    {
        token = strtok(NULL, delimeter);
        if (token != NULL)
        {
            bitcoinID = atoi(token);
        }
    }

    while (i < size_of_bitcoin_trees && bitcoin_trees[i].bitcoinID != bitcoinID)
    {
        i++;
    }
    if (i == size_of_bitcoin_trees)
    {
        error_handling(7);
        return;
    }

    number_of_trasactions = number_of_treenodes(bitcoin_trees[i].root) - number_of_leaves(bitcoin_trees[i].root);
    unspent = righter_node(bitcoin_trees[i].root);
    printf("Initial balance of %d is %d$.\nThe %d bitcoin took place at %d transaction(s).\nUnspend ammount is %d$.\n",
           bitcoinID,
           bitcoin_trees[i].root->value.value_of_bitcoin,
           bitcoinID,
           number_of_trasactions,
           unspent->value.value_of_bitcoin);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void traceCoin(char *token, int size_of_bitcoin_trees, tree *bitcoin_trees)
{
    const char delimeter[2] = " ";
    int bitcoinID;
    int i = 0;
    while (token != NULL)
    {
        token = strtok(NULL, delimeter);
        if (token != NULL)
        {
            bitcoinID = atoi(token);
        }
    }

    while (i < size_of_bitcoin_trees && bitcoin_trees[i].bitcoinID != bitcoinID)
    {
        i++;
    }
    if (i == size_of_bitcoin_trees)
    {
        error_handling(7);
        return;
    }
    preorder_print(bitcoin_trees[i].root);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void myexit(hashtable **wallets, hashtable **senders, hashtable **receivers, tree *bitcoin_trees, int size_of_bitcoin_trees)
{
    clear_wallets(wallets);
    clear_senders(senders);
    clear_receivers(receivers);
    clear_bitcoin_trees(bitcoin_trees, size_of_bitcoin_trees);
}
