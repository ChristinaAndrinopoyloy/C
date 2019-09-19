#ifndef _IP_PORT_LIST_
#define _IP_PORT_LIST_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "./../../string_library/string_library.h"

typedef struct IP_Port_ListNode
{
    int connection_list_no;
    int port;
    my_string *ip;
    struct IP_Port_ListNode *next;
} ip_port_listnode;

bool empty_list(ip_port_listnode *head);
int length_list(ip_port_listnode *head);
void insert_list(ip_port_listnode **head, my_string *ip, int port, int connection_list_no);
ip_port_listnode *search_list(ip_port_listnode *head, my_string *ip, int port);
ip_port_listnode *search_list_by_connection_list_no(ip_port_listnode *head, int connection_list_no);
void delete_list(ip_port_listnode **head, int connection_list_no);
void print_list(ip_port_listnode *head);
void free_list(ip_port_listnode *head);

#endif