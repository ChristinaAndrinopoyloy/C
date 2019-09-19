#include "tree.h"

tree_node *create_tree_node(tree_node_value val)
{
    tree_node *new_node = (tree_node *)malloc(sizeof(tree_node));
    new_node->left = NULL;
    new_node->right = NULL;
    memcpy(&(new_node->value.walletID), &(val.walletID), strlen(val.walletID) + 1);
    new_node->value.value_of_bitcoin = val.value_of_bitcoin;
    new_node->value.transaction_of_tree_node = val.transaction_of_tree_node;
    return new_node;
}

void insert_tree(tree_node *new_node, tree_node **root)
{
    if (*root == NULL)
    {
        *root = new_node;
    }
    else
    {
        if (new_node->value.value_of_bitcoin <= (*root)->value.value_of_bitcoin)
        {
            insert_tree(new_node, &(*root)->left);
        }
        else
        {
            insert_tree(new_node, &(*root)->right);
        }
    }
    return;
}

void break_tree_node(tree_node *new_node, tree_node *old_node, tree_node **current, transaction mytransaction)
{
    if (new_node->value.value_of_bitcoin <= (*current)->value.value_of_bitcoin)
    {
        (*current)->left = new_node;
        (*current)->right = old_node;
    }
    else
    {
        (*current)->left = old_node;
        (*current)->right = new_node;
    }
    return;
}

tree_node *search_tree_by_value(int value, tree_node *root)
{
    if (root == NULL)
    {
        return NULL;
    }
    else
    {
        if (root->value.value_of_bitcoin == value)
        {
            return root;
        }
        else
        {
            if (root->value.value_of_bitcoin < value)
            {
                search_tree_by_value(value, root->right);
            }
            else
            {
                search_tree_by_value(value, root->left);
            }
        }
    }
}

void preorder_print(tree_node *current)
{
    if (current == NULL)
    {
        return;
    }
    if (current->left != NULL && current->right != NULL)
    {
        printf("%s %s %s %d %s %s\n", current->value.transaction_of_tree_node->transactionID,
               current->value.transaction_of_tree_node->senderWalletID,
               current->value.transaction_of_tree_node->receiverWalletID,
               current->value.transaction_of_tree_node->value,
               current->value.transaction_of_tree_node->date_of_transaction,
               current->value.transaction_of_tree_node->time_of_transaction);
    }
    preorder_print(current->left);
    preorder_print(current->right);
    return;
}

int number_of_treenodes(tree_node *root)
{
    if (root == NULL)
    {
        return 0;
    }
    else
    {
        return 1 + number_of_treenodes(root->right) + number_of_treenodes(root->left);
    }
}

int number_of_leaves(tree_node *root)
{
    if (root == NULL)
    {
        return 0;
    }
    if (root->right == NULL && root->left == NULL)
    {
        return 1;
    }
    else
    {
        return number_of_leaves(root->right) + number_of_leaves(root->left);
    }
}

tree_node *righter_node(tree_node *root)
{
    if (root == NULL)
    {
        return NULL;
    }
    if (root->right == NULL)
    {
        return root;
    }
    else
    {
        return righter_node(root->right);
    }
}

void free_tree(tree_node *root)
{
    if (root != NULL)
    {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}