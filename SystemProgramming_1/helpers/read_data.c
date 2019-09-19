#include "read_data.h"

/*read a txt file*/
void read_and_initialize(char *file_to_open, char kind_of_txt,
                         hashtable **wallets,
                         hashtable **senders,
                         hashtable **receivers, int balance,
                         tree *bitcoin_trees,
                         int size_of_bitcoin_trees)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    size_t read;
    list_node *userID_list = NULL;
    extern list_node *bitcoinID_list;
    list_node *transactionID_list = NULL;

    fp = fopen(file_to_open, "r"); // open the txt
    if (fp == NULL)
    {
        printf("Error: Can't find %s \n", file_to_open);
        return;
    }

    while ((read = getline(&line, &len, fp)) != -1) // read line by line
    {
        split(line, kind_of_txt,
              wallets,
              senders,
              receivers,
              balance,
              &userID_list,
              &bitcoinID_list,
              &transactionID_list, bitcoin_trees, size_of_bitcoin_trees); //split the line
    }

    free(line);
    return;
}

/*split a txt of balances or a txt of transactions*/
void split(char *line, char kind_of_text,
           hashtable **wallets,
           hashtable **senders,
           hashtable **receivers,
           int balance,
           list_node **userID_list,
           list_node **bitcoinID_list,
           list_node **transactionID_list,
           tree *bitcoin_trees,
           int size_of_bitcoin_trees)
{
    if (kind_of_text == 0) // BALANCE_TXT
    {
        struct temp_user user;
        memset(&(user.username)[0], 0, 50);
        wallet temp_wallet;
        memset(&(temp_wallet.walletID)[0], 0, 50);
        record temp_record;
        memset(&(temp_record.key)[0], 0, 50);
        memset(&(temp_record.hash_value.mywallet.walletID)[0], 0, 50);
        int number_of_bitcoins = 0;
        char *token;
        char *temp_line = (char *)malloc(strlen(line) * sizeof(char));
        const char delimeter[2] = " ";

        memcpy(temp_line, line, strlen(line));
        for (int i = 0; i < strlen(temp_line) + 1; i++)
        {
            if (temp_line[i] == '\n')
            {
                temp_line[i] = '\0';
            }
        }
        for (int i = 0; i < strlen(line) + 1; i++)
        {
            if (line[i] == '\n')
            {
                line[i] = '\0';
            }
        }

        token = strtok(temp_line, delimeter);
        while (token != NULL)
        {
            token = strtok(NULL, delimeter);
            if (token != NULL)
            {
                number_of_bitcoins++; // counting the number of bitcoins
            }
        }
        user = read_balances(line, number_of_bitcoins, userID_list, bitcoinID_list); // user from balance txt
        memcpy(temp_wallet.walletID, user.username, strlen(user.username));          // make a wallet in order to insert this wallet at a hashtable (hashing by walletID)
        temp_wallet.sum_balance = 0;
        temp_wallet.bitcoin_and_balance = NULL;
        union list_value *temp_list_value = (union list_value *)malloc(sizeof(union list_value));
        for (int i = 0; i < number_of_bitcoins; i++)
        {
            temp_list_value->bitcoin_and_balance.balance_of_bitcoin = balance;
            temp_list_value->bitcoin_and_balance.bitcoinID = user.bitcoins[i];
            temp_list_value->bitcoin_and_balance.root_of_bitcoin_tree = NULL;
            insert(&(temp_wallet.bitcoin_and_balance), temp_list_value);
            temp_wallet.sum_balance += balance;
        }
        memcpy(temp_record.key, user.username, strlen(user.username));
        temp_record.hash_value.mywallet = temp_wallet;
        insert_record(wallets, temp_record, WALLET_HASHTABLE); // insert wallet at hashtable wallets

        free(user.bitcoins);
        free(temp_list_value);
        free(temp_line);
    }
    else // TRANSACTION_TXT
    {
        extern char app_last_date[11];
        extern char app_last_time[6];
        extern int app_transactionID;
        transaction mytransaction;
        memset(&(mytransaction.transactionID)[0], 0, 15);
        memset(&(mytransaction.senderWalletID)[0], 0, 50);
        memset(&(mytransaction.receiverWalletID)[0], 0, 50);
        mytransaction.bitcoins_of_transaction = NULL;
        mytransaction = read_transactions(line, transactionID_list);
        if (mytransaction.value != -1)
        {
            if (valid_checking_sender_receiver(mytransaction, wallets))
            {
                if (update_system(mytransaction, wallets, senders, receivers, bitcoin_trees, size_of_bitcoin_trees) == true)
                {
                    memcpy(app_last_date, mytransaction.date_of_transaction, 11);
                    memcpy(app_last_time, mytransaction.time_of_transaction, 11);
                    if (atoi(mytransaction.transactionID) > app_transactionID)
                    {
                        app_transactionID = atoi(mytransaction.transactionID);
                    }
                }
            }
        }
    }
    return;
}

/*read a line of balance and return the name and the bitcoins*/
struct temp_user read_balances(char *line, int number_of_bitcoins,
                               list_node **userID_list,
                               list_node **bitcoinID_list)
{
    struct temp_user user;
    memset(&(user.username)[0], 0, 50);
    char *token;
    const char delimeter[2] = " ";
    user.bitcoins = (int *)malloc(number_of_bitcoins * sizeof(int));
    memset(&(user).bitcoins[0], 0, number_of_bitcoins);
    int index = 0;
    extern int size_of_bitcoin_trees;
    size_of_bitcoin_trees += number_of_bitcoins;
    owner_bitcoin owner, owner_and_bitcoin;

    token = strtok(line, delimeter);
    memcpy(owner.walletID, token, strlen(token) + 1);
    valid_checking_id(userID_list, owner);
    memcpy(user.username, token, strlen(token) + 1);
    while (token != NULL)
    {
        token = strtok(NULL, delimeter);
        if (token != NULL)
        {
            memcpy(owner_and_bitcoin.walletID, user.username, strlen(user.username) + 1);
            owner_and_bitcoin.bitcoinID = atoi(token);
            valid_checking_bitcoin(bitcoinID_list, owner_and_bitcoin);
            user.bitcoins[index] = atoi(token);
            index++;
        }
    }
    return user;
}

/*read a line of transactions and return the ID, the name of sender and receiver, the value of transaction and the date-time*/
transaction read_transactions(char *line, list_node **transactionID_list)
{
    transaction mytransaction;
    char *token;
    const char delimeter[2] = " ";
    int i = 1;
    char transactionID[15];
    char senderID[50];
    char receiverID[50];
    char char_value[11];
    int value_backup;
    char char_date[11];
    char char_time[6];

    mytransaction.value = -1;
    token = strtok(line, delimeter);

    memcpy(transactionID, token, strlen(token) + 1);
    i++;
    while (token != NULL)
    {
        token = strtok(NULL, delimeter);
        if (token != NULL)
        {
            switch (i)
            {
            case 2:
                memcpy(senderID, token, strlen(token) + 1);
                break;
            case 3:
                memcpy(receiverID, token, strlen(token) + 1);
                break;
            case 4:
                memcpy(char_value, token, strlen(token) + 1);
                value_backup = atoi(char_value);
                break;
            case 5:
                memcpy(char_date, token, strlen(token) + 1);
                break;
            case 6:
                memcpy(char_time, token, strlen(token) + 1);
                break;
            default:
                break;
            }
            i++;
        }
    }
    if (i == 6)
    {
        extern int app_transactionID;
        app_transactionID += 1;
        sprintf(mytransaction.transactionID, "%d", app_transactionID);
        memcpy(mytransaction.senderWalletID, transactionID, strlen(transactionID) + 1);
        memcpy(mytransaction.receiverWalletID, senderID, strlen(senderID) + 1);
        mytransaction.value = atoi(receiverID);
        memcpy(mytransaction.date_of_transaction, char_value, strlen(char_value) + 1);
        memcpy(mytransaction.time_of_transaction, char_date, strlen(char_date) + 1);
    }
    else
    {
        if (valid_checking_transactionID(transactionID_list, transactionID))
        {
            memcpy(&(mytransaction.transactionID)[0], transactionID, strlen(transactionID) + 1);
            memcpy(mytransaction.senderWalletID, senderID, strlen(senderID) + 1);
            memcpy(mytransaction.receiverWalletID, receiverID, strlen(receiverID) + 1);
            mytransaction.value = value_backup;
            memcpy(mytransaction.date_of_transaction, char_date, strlen(char_date) + 1);
            memcpy(mytransaction.time_of_transaction, char_time, strlen(char_date) + 1);
        }
    }

    return mytransaction;
}

void tree_initialization(tree *bitcoin_trees, list_node *bitcoinID_list, int value, hashtable **wallets)
{
    int index = 0;
    list_node *current = bitcoinID_list;
    tree_node_value temp_node;

    while (current != NULL)
    {
        bitcoin_trees[index].bitcoinID = current->data.owner_of_bitcoin.bitcoinID;
        memcpy(temp_node.walletID, current->data.owner_of_bitcoin.walletID, sizeof(current->data.owner_of_bitcoin.walletID));
        temp_node.value_of_bitcoin = value;
        temp_node.transaction_of_tree_node = NULL;
        bitcoin_trees[index].root = create_tree_node(temp_node);

        // return at wallet hashtable and update the list bitcoin_and balance with the root that made
        record *temp_record;
        temp_record = search_hashtable(current->data.owner_of_bitcoin.walletID, wallets);
        list_node *temp_listnode = search_bitcoinID(temp_record->hash_value.mywallet.bitcoin_and_balance, current->data.owner_of_bitcoin);
        temp_listnode->data.bitcoin_and_balance.root_of_bitcoin_tree = bitcoin_trees[index].root;
        current = current->next;
        index++;
    }

    return;
}