#include "ip_port_list.h"

//returns true if the list is empty
bool empty_list(ip_port_listnode *head)
{
    return head == NULL;
}

// returns the length of a list
int length_list(ip_port_listnode *head)
{
    int length = 0;
    ip_port_listnode *current;

    for (current = head; current->next != NULL; current = current->next)
    {
        length++;
    }
    length++;
    return length;
}

//insert a node at the end of the list
void insert_list(ip_port_listnode **head, my_string *ip, int port, int connection_list_no)
{
    ip_port_listnode *current = *head;

    if (search_list(*head, ip, port) == NULL)
    {
        if (*head == NULL)
        {
            *head = (ip_port_listnode *)malloc(sizeof(ip_port_listnode));
            initialize_string(&((*head)->ip));
            if ((*head) == NULL)
            {
                perror("malloc");
            }
            assign_string((*head)->ip, ip);
            (*head)->port = port;
            (*head)->connection_list_no = connection_list_no;
            (*(head))->next = NULL;
            return;
        }
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = (ip_port_listnode *)malloc(sizeof(ip_port_listnode));
        initialize_string(&(current->next->ip));
        if (current == NULL)
        {
            perror("malloc");
        }
        assign_string(current->next->ip, ip);
        current->next->port = port;
        current->next->connection_list_no = connection_list_no;
        (current->next)->next = NULL;
    }
    else
    {
        printf("<ip,port> already exists\n");
    }

    return;
}

//search for a given id
ip_port_listnode *search_list(ip_port_listnode *head, my_string *ip, int port)
{
    ip_port_listnode *current = head;
    if (head == NULL)
    {
        return NULL;
    }

    while ((port != current->port) || (strcmp(ip->str, current->ip->str) != 0))
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

ip_port_listnode *search_list_by_connection_list_no(ip_port_listnode *head, int connection_list_no)
{
    ip_port_listnode *current = head;
    if (head == NULL)
    {
        return NULL;
    }

    // while (strcmp(data->str, current->data->str) != 0)
    while (connection_list_no != current->connection_list_no)
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

void delete_list(ip_port_listnode **head, int connection_list_no)
{
    if (!head || !(*head))
        return;

    ip_port_listnode *current = *head;
    ip_port_listnode *previous = NULL;

    // while (strcmp(current->ip->str, ip->str) != 0 && current->port != port && current->next != NULL)
    while (current->connection_list_no != connection_list_no)
    {
        previous = current;
        current = current->next;
    }

    // if (strcmp(current->ip->str, ip->str) == 0 && current->port == port)
    if (current->connection_list_no == connection_list_no)
    {
        if (previous)
        {
            previous->next = current->next;
        }
        else
        {
            *head = current->next;
        }
        destroy_string(current->ip);
        free(current);
    }
    return;
}

void print_list(ip_port_listnode *head)
{
    ip_port_listnode *current = head;
    if (current == NULL)
    {
        printf("empty list\n");
    }
    else
    {
        printf("%s %d (%d) -> ", current->ip->str, current->port, current->connection_list_no);
        while (current->next != NULL)
        {
            current = current->next;
            printf("%s %d (%d) -> ", current->ip->str, current->port, current->connection_list_no);
        }
        printf("\n");
    }
    return;
}

void free_list(ip_port_listnode *head)
{
    ip_port_listnode *current;

    while (head != NULL)
    {
        current = head;
        head = head->next;
        free(current);
    }
}