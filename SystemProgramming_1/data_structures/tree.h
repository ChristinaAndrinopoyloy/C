#ifndef _TREE_H_
#define _TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../structures.h"

typedef struct Transaction transaction;

typedef struct Tree_node_value
{
    char walletID[50];
    int value_of_bitcoin;
    transaction *transaction_of_tree_node;
}tree_node_value;

typedef struct Tree_node
{
    tree_node_value value;
    struct Tree_node *left;
    struct Tree_node *right;
}tree_node;

typedef struct Tree
{
    int bitcoinID;
    tree_node *root;
}tree;

tree_node *create_tree_node(tree_node_value val);
void insert_tree(tree_node *new_node, tree_node **root);
void break_tree_node(tree_node *new_node, tree_node *old_node, tree_node **current, transaction mytransaction);
tree_node *search_tree_by_value(int value,  tree_node *root);
void preorder_print(tree_node *current);
int number_of_treenodes(tree_node *root);
int number_of_leaves(tree_node *root);
tree_node *righter_node(tree_node *root);
void free_tree(tree_node *root);

#endif