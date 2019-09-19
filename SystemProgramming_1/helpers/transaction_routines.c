#include "transaction_routines.h"

bool update_system(transaction mytransaction,
                   hashtable **wallets,
                   hashtable **senders,
                   hashtable **receivers,
                   tree *bitcoin_trees, int size_of_bitcoin_trees)
{
    record *sender_wallet = search_hashtable(mytransaction.senderWalletID, wallets); // find the sender wallet for this transaction
    if (sender_wallet != NULL)                                                       // if user is valid
    {
        if (valid_checking_sum_balance(sender_wallet->hash_value.mywallet, mytransaction.value)) // if balance is enough
        {
            mytransaction.bitcoins_of_transaction = appropriate_bitcoin_finder(mytransaction.value, sender_wallet->hash_value.mywallet); // bitcoins for the transaction
            update(mytransaction, wallets, senders, receivers, bitcoin_trees, size_of_bitcoin_trees);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        error_handling(5);
        return false;
    }
}

list_node *appropriate_bitcoin_finder(int balance, wallet sender_wallet)
{
    list_node *appropriate_bitcoins = NULL;
    union list_value value;
    int sum_balance = 0;
    int initial_value_of_bitcoin;
    do
    {
        value = (search_bitcoin_balance(sender_wallet.bitcoin_and_balance, (balance - sum_balance), &initial_value_of_bitcoin))->data;
        insert(&(appropriate_bitcoins), &value);
        sum_balance = sum_balance + (initial_value_of_bitcoin - (value.bitcoin_and_balance.balance_of_bitcoin));
    } while (sum_balance < balance);
    return appropriate_bitcoins;
}

void update(transaction mytransaction,
            hashtable **wallets,
            hashtable **senders,
            hashtable **receivers,
            tree *bitcoin_trees, int size_of_bitcoin_trees)
{
    list_node *current = NULL;
    current = mytransaction.bitcoins_of_transaction;
    record *found_sender, *found_receiver;
    tree_node *tttt;
    tree tt;
    while (current != NULL) // for every bitcoin of transaction
    {
        for (int i = 0; i < size_of_bitcoin_trees; i++) // search appropriate bitcoin tree
        {
            if (bitcoin_trees[i].bitcoinID == current->data.bitcoin_and_balance.bitcoinID)
            {
                tt = bitcoin_trees[i];
                tttt = bitcoin_trees[i].root;
                if (bitcoin_trees[i].root->left == NULL && bitcoin_trees[i].root->right == NULL) // if bitcoin tree has only root
                {
                    update_root(mytransaction, wallets, senders, receivers, bitcoin_trees[i], current);
                }
                else
                {
                    update_NOT_root(mytransaction, wallets, senders, receivers, bitcoin_trees[i], current);
                }
            }
        }
        current = current->next;
    }

    return;
}

void update_root(transaction mytransaction,
                 hashtable **wallets,
                 hashtable **senders,
                 hashtable **receivers,
                 tree current_bitcoin_trees,
                 list_node *current)
{
    record *mysender = search_hashtable(mytransaction.senderWalletID, wallets); // find the walletID who has this bitcoin (this is the only walletID at the moment)

    tree_node *new_node = (tree_node *)malloc(sizeof(tree_node)); // initializations tree nodes
    tree_node *old_node = (tree_node *)malloc(sizeof(tree_node));
    tree_node *start = current_bitcoin_trees.root;
    initialization_tree_nodes(&old_node, &new_node, mytransaction, current_bitcoin_trees, start, current);
    break_tree_node(new_node, old_node, &start, mytransaction); // break the root of bitcoin tree

    union list_value transaction_node; // initializations
    transaction_node.transact = &mytransaction;
    union list_value *old, *neww;
    old = (union list_value *)malloc(sizeof(union list_value));
    neww = (union list_value *)malloc(sizeof(union list_value));

    search_sender_at_senders(mytransaction, senders, wallets, old_node, new_node, old, neww, transaction_node, current_bitcoin_trees, current, mysender); // search sender at senders and update
    search_receiver_at_receivers(mytransaction, receivers, new_node, neww, transaction_node, current);                                                    // search for receiver at receivers and update
    search_receiver_at_wallets(mytransaction, wallets, new_node, current_bitcoin_trees, current);                                                         // search for receiver at wallets and update

    record *temp_sender = search_hashtable(mytransaction.senderWalletID, senders);
    list_node *temp_listnode = search_transaction(temp_sender->hash_value.mysender.transactions, mytransaction);
    start->value.transaction_of_tree_node = temp_listnode->data.transact;

    return;
}

void update_NOT_root(transaction mytransaction,
                     hashtable **wallets,
                     hashtable **senders,
                     hashtable **receivers,
                     tree current_bitcoin_trees,
                     list_node *current)
{
    record *mysender = search_hashtable(mytransaction.senderWalletID, wallets);           // find sender at wallets
    record *found_sender = search_hashtable(mytransaction.senderWalletID, senders);       // search for sender at senders
    record *found_receiver = search_hashtable(mytransaction.receiverWalletID, receivers); // search for receiver at receivers

    union list_value transaction_node; // initializations
    transaction_node.transact = &mytransaction;
    union list_value *old, *neww;
    old = (union list_value *)malloc(sizeof(union list_value));
    neww = (union list_value *)malloc(sizeof(union list_value));

    tree_node *current_tree_node;

    if (found_sender != NULL) // this sender was at senders hashtable already
    {
        owner_bitcoin temp_owner_of_bitcoin;
        temp_owner_of_bitcoin.bitcoinID = current_bitcoin_trees.bitcoinID;
        tree_node *new_node = (tree_node *)malloc(sizeof(tree_node)); // initializations tree nodes
        tree_node *old_node = (tree_node *)malloc(sizeof(tree_node));
        tree_node *start = search_bitcoin_ID(found_sender->hash_value.mysender.sending, current->data.bitcoin_and_balance.bitcoinID)->data.ptr_tree->ptr_tree;
        initialization_tree_nodes(&old_node, &new_node, mytransaction, current_bitcoin_trees, start, current);
        break_tree_node(new_node, old_node, &start, mytransaction); // break the root of bitcoin tree

        update_senders_and_wallets(wallets, found_sender, mysender, mytransaction, old_node, new_node, old, transaction_node, current_bitcoin_trees, current);
        search_receiver_at_receivers(mytransaction, receivers, new_node, neww, transaction_node, current);
        search_receiver_at_wallets(mytransaction, wallets, new_node, current_bitcoin_trees, current);

        record *temp_sender = search_hashtable(mytransaction.senderWalletID, senders);
        list_node *temp_listnode = search_transaction(temp_sender->hash_value.mysender.transactions, mytransaction);
        start->value.transaction_of_tree_node = temp_listnode->data.transact;
    }
    else
    {
        tree_node *new_node = (tree_node *)malloc(sizeof(tree_node)); // initializations tree nodes
        tree_node *old_node = (tree_node *)malloc(sizeof(tree_node));
        tree_node *start;

        found_sender = search_hashtable(mytransaction.senderWalletID, receivers);
        if (found_sender != NULL) // this sender was at receivers hashtable
        {
            owner_bitcoin temp_owner_of_bitcoin;
            temp_owner_of_bitcoin.bitcoinID = current_bitcoin_trees.bitcoinID;
            start = search_bitcoin_ID(found_sender->hash_value.myreceiver.receiving, current->data.bitcoin_and_balance.bitcoinID)->data.ptr_tree->ptr_tree;
            initialization_tree_nodes(&old_node, &new_node, mytransaction, current_bitcoin_trees, start, current);
            break_tree_node(new_node, old_node, &start, mytransaction); // break the root of bitcoin tree

            record new_sender = initialize_record(SENDER_HASHTABLE);                                        // initialization
            memcpy(new_sender.key, mytransaction.senderWalletID, strlen(mytransaction.senderWalletID) + 1); // assign
            memcpy(&(new_sender.hash_value.mysender.walletID), mytransaction.senderWalletID, strlen(mytransaction.senderWalletID) + 1);
            insert_trans(&(new_sender.hash_value.mysender.transactions), &transaction_node); // insert transaction at sender record
            old->ptr_tree = (pointer_to_tree *)malloc(sizeof(pointer_to_tree));
            old->ptr_tree->ptr_tree = (tree_node *)malloc(sizeof(tree_node));
            old->ptr_tree->ptr_tree = old_node;
            old->ptr_tree->bitcoinID = current->data.bitcoin_and_balance.bitcoinID;
            insert(&(new_sender.hash_value.mysender.sending), old); // insert ptr to tree at sender record
            insert_record(senders, new_sender, SENDER_HASHTABLE);   // update senders
            search_hashtable(mytransaction.senderWalletID, wallets)->hash_value.mywallet.sum_balance -= new_node->value.value_of_bitcoin;
        }

        search_receiver_at_receivers(mytransaction, receivers, new_node, neww, transaction_node, current); // search for receiver at receivers and update
        search_receiver_at_wallets(mytransaction, wallets, new_node, current_bitcoin_trees, current);      // search for receiver at wallets and update
        record *temp_sender = search_hashtable(mytransaction.senderWalletID, senders);
        list_node *temp_listnode = search_transaction(temp_sender->hash_value.mysender.transactions, mytransaction);
        start->value.transaction_of_tree_node = temp_listnode->data.transact;
    }

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void search_sender_at_senders(transaction mytransaction,
                              hashtable **senders, hashtable **wallets,
                              tree_node *old_node, tree_node *new_node,
                              union list_value *old, union list_value *neww, union list_value transaction_node,
                              tree current_bitcoin_trees,
                              list_node *current,
                              record *mysender)
{
    record *found_sender = search_hashtable(mytransaction.senderWalletID, senders); // search for sender at senders
    if (found_sender == NULL)                                                       // this sender wasn't at sender hashtable
    {
        record new_sender = initialize_record(SENDER_HASHTABLE);                                        // initialization
        memcpy(new_sender.key, mytransaction.senderWalletID, strlen(mytransaction.senderWalletID) + 1); // assign
        memcpy(&(new_sender.hash_value.mysender.walletID), mytransaction.senderWalletID, strlen(mytransaction.senderWalletID) + 1);
        insert_trans(&(new_sender.hash_value.mysender.transactions), &transaction_node); // insert transaction at sender record
        old->ptr_tree = (pointer_to_tree *)malloc(sizeof(pointer_to_tree));
        old->ptr_tree->ptr_tree = (tree_node *)malloc(sizeof(tree_node));
        old->ptr_tree->ptr_tree = old_node;
        old->ptr_tree->bitcoinID = current->data.bitcoin_and_balance.bitcoinID;
        insert(&(new_sender.hash_value.mysender.sending), old); // insert ptr to tree at sender record
        insert_record(senders, new_sender, SENDER_HASHTABLE);   // update senders

        owner_bitcoin temp_owner_of_bitcoin; // update wallets hashtable
        temp_owner_of_bitcoin.bitcoinID = current_bitcoin_trees.bitcoinID;

        record *rr = search_hashtable(mytransaction.senderWalletID, wallets);
        rr->hash_value.mywallet.sum_balance -= new_node->value.value_of_bitcoin;
    }
    else // this sender was at senders hashtable already
    {
        update_senders_and_wallets(wallets, found_sender, mysender, mytransaction, old_node, new_node, old, transaction_node, current_bitcoin_trees, current);
    }
    return;
}

void search_receiver_at_receivers(transaction mytransaction, hashtable **receivers, tree_node *new_node, union list_value *neww, union list_value transaction_node, list_node *current)
{
    record *found_receiver = search_hashtable(mytransaction.receiverWalletID, receivers); // search for receiver at receivers
    if (found_receiver == NULL)                                                           // this receiver isn't at receivers hashtable
    {
        record new_receiver = initialize_record(RECEIVER_HASHTABLE); // initialization

        memcpy(new_receiver.key, mytransaction.receiverWalletID, strlen(mytransaction.receiverWalletID) + 1); // assign
        memcpy(new_receiver.hash_value.myreceiver.walletID, mytransaction.receiverWalletID, strlen(mytransaction.receiverWalletID) + 1);
        insert_trans(&(new_receiver.hash_value.myreceiver.transactions), &transaction_node); // insert transaction at receiver record
        neww->ptr_tree = (pointer_to_tree *)malloc(sizeof(pointer_to_tree));
        neww->ptr_tree->ptr_tree = (tree_node *)malloc(sizeof(tree_node));
        neww->ptr_tree->ptr_tree = new_node;
        neww->ptr_tree->bitcoinID = current->data.bitcoin_and_balance.bitcoinID;

        insert(&(new_receiver.hash_value.myreceiver.receiving), neww); // insert ptr to tree at receiver record
        insert_record(receivers, new_receiver, RECEIVER_HASHTABLE);    // update receivers
    }
    else // this receiver was at receivers hashtable already
    {
        insert_trans(&(found_receiver->hash_value.myreceiver.transactions), &transaction_node); // update receivers hashtable
        neww->ptr_tree = (pointer_to_tree *)malloc(sizeof(pointer_to_tree));
        neww->ptr_tree->ptr_tree = (tree_node *)malloc(sizeof(tree_node));
        neww->ptr_tree->ptr_tree = new_node;
        neww->ptr_tree->bitcoinID = current->data.bitcoin_and_balance.bitcoinID;
        insert(&(found_receiver->hash_value.myreceiver.receiving), neww);
    }
    return;
}

void search_receiver_at_wallets(transaction mytransaction, hashtable **wallets, tree_node *new_node, tree current_bitcoin_trees, list_node *current)
{
    record *found_receiver_at_wallets;
    found_receiver_at_wallets = search_hashtable(mytransaction.receiverWalletID, wallets); // search receiver at wallets hashtable
    if (found_receiver_at_wallets == NULL)                                                 // receiver wasn't at wallets hashtable
    {
        error_handling(5);
    }
    else // receivers was at wallets hashtable
    {
        owner_bitcoin temp_owner_of_bitcoin; // update
        temp_owner_of_bitcoin.bitcoinID = current_bitcoin_trees.bitcoinID;
        list_node *temp_list_node = search_bitcoinID(found_receiver_at_wallets->hash_value.mywallet.bitcoin_and_balance, temp_owner_of_bitcoin);
        if (temp_list_node != NULL)
        {
            temp_list_node->data.bitcoin_and_balance.balance_of_bitcoin -= current->data.bitcoin_and_balance.balance_of_bitcoin; // update wallets
        }
        else
        {
            temp_list_node = (list_node *)malloc(sizeof(list_node));
            temp_list_node->data.bitcoin_and_balance.balance_of_bitcoin = new_node->value.value_of_bitcoin; //current->data.bitcoin_and_balance.balance_of_bitcoin;
            temp_list_node->data.bitcoin_and_balance.bitcoinID = current->data.bitcoin_and_balance.bitcoinID;
            temp_list_node->data.bitcoin_and_balance.root_of_bitcoin_tree = new_node;
            insert(&(found_receiver_at_wallets->hash_value.mywallet.bitcoin_and_balance), &(temp_list_node->data));
            search_hashtable(mytransaction.receiverWalletID, wallets)->hash_value.mywallet.sum_balance += new_node->value.value_of_bitcoin;
        }
    }
    return;
}

void initialization_tree_nodes(tree_node **old_node, tree_node **new_node, transaction mytransaction, tree current_bitcoin_tree, tree_node *start, list_node *current)
{
    (*old_node)->left = NULL;
    (*old_node)->right = NULL;
    memcpy((*old_node)->value.walletID, mytransaction.senderWalletID, 50); // assign tree nodes
    (*old_node)->value.value_of_bitcoin = current->data.bitcoin_and_balance.balance_of_bitcoin;
    (*old_node)->value.transaction_of_tree_node = (transaction *)malloc(sizeof(transaction));
    memcpy((*old_node)->value.transaction_of_tree_node->transactionID, mytransaction.transactionID, 15);
    memcpy((*old_node)->value.transaction_of_tree_node->senderWalletID, mytransaction.senderWalletID, 50);
    memcpy((*old_node)->value.transaction_of_tree_node->receiverWalletID, mytransaction.receiverWalletID, 50);
    memcpy((*old_node)->value.transaction_of_tree_node->date_of_transaction, mytransaction.date_of_transaction, 11);
    memcpy((*old_node)->value.transaction_of_tree_node->time_of_transaction, mytransaction.time_of_transaction, 6);
    (*old_node)->value.transaction_of_tree_node->value = mytransaction.value;

    (*new_node)->left = NULL;
    (*new_node)->right = NULL;
    memcpy((*new_node)->value.walletID, mytransaction.receiverWalletID, 50);
    (*new_node)->value.value_of_bitcoin = start->value.value_of_bitcoin - current->data.bitcoin_and_balance.balance_of_bitcoin;
    (*new_node)->value.transaction_of_tree_node = (transaction *)malloc(sizeof(transaction));
    memcpy((*new_node)->value.transaction_of_tree_node->transactionID, mytransaction.transactionID, 15);
    memcpy((*new_node)->value.transaction_of_tree_node->senderWalletID, mytransaction.senderWalletID, 50);
    memcpy((*new_node)->value.transaction_of_tree_node->receiverWalletID, mytransaction.receiverWalletID, 50);
    memcpy((*new_node)->value.transaction_of_tree_node->date_of_transaction, mytransaction.date_of_transaction, 11);
    memcpy((*new_node)->value.transaction_of_tree_node->time_of_transaction, mytransaction.time_of_transaction, 6);
    (*new_node)->value.transaction_of_tree_node->value = mytransaction.value;

    return;
}

void update_senders_and_wallets(hashtable **wallets,
                                record *found_sender, record *mysender,
                                transaction mytransaction,
                                tree_node *old_node, tree_node *new_node,
                                union list_value *old, union list_value transaction_node,
                                tree current_bitcoin_trees,
                                list_node *current)
{
    insert_trans(&(found_sender->hash_value.mysender.transactions), &transaction_node); // update senders hashtable
    old->ptr_tree = (pointer_to_tree *)malloc(sizeof(pointer_to_tree));
    old->ptr_tree->ptr_tree = (tree_node *)malloc(sizeof(tree_node));
    old->ptr_tree->ptr_tree = old_node;
    old->ptr_tree->bitcoinID = current->data.bitcoin_and_balance.bitcoinID;

    insert(&(found_sender->hash_value.mysender.sending), old);

    owner_bitcoin temp_owner_of_bitcoin; // update wallets hashtable
    temp_owner_of_bitcoin.bitcoinID = current_bitcoin_trees.bitcoinID;
    search_hashtable(mytransaction.senderWalletID, wallets)->hash_value.mywallet.sum_balance -= new_node->value.value_of_bitcoin;

    return;
}