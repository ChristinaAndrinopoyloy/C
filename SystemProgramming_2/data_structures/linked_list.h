#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "./../string_library/string_library.h"

typedef struct ListNode
{
    my_string *data;
    
    struct ListNode *next;
} list_node;

bool empty_list(list_node *head);
int length_list(list_node *head);
void insert_list(list_node **head, my_string *data);
list_node *search_list(list_node *head, my_string *data);
void delete_list(list_node **head, my_string *id);
void print_list(list_node *head);
void free_list(list_node *head);

#endif