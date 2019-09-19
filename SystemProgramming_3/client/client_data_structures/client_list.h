#ifndef _CLIENT_LIST_H_
#define _CLIENT_LIST_H_

#include <stdbool.h>
#include "./../../string_library/string_library.h"
#include "./../../error_handling/error_handling.h"

typedef struct Client_List
{
    my_string *ip;
    int port;    
    struct Client_List *next;
} client_list;

bool empty_list(client_list *head);
int length_list(client_list *head);
void insert_list(client_list **head, my_string *ip, int port);
client_list *search_list(client_list *head, my_string *ip, int port);
client_list *search_list_by_connection_list_no(client_list *head);
void delete_list(client_list **head, my_string *ip, int port);
void print_list(client_list *head);
void free_list(client_list *head);

#endif