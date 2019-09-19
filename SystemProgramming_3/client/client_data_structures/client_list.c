#include "client_list.h"

//returns true if the list is empty
bool empty_list(client_list *head)
{
    return head == NULL;
}

// returns the length of a list
int length_list(client_list *head)
{
    client_list *current = head;
    int length = 0;

    while (current != NULL)
    {
        length++;
        current = current->next;
    }
    return length;
}

//insert a node at the end of the list
void insert_list(client_list **head, my_string *ip, int port)
{
    client_list *current = *head;

    if (search_list(*head, ip, port) == NULL)
    {
        if (*head == NULL)
        {
            *head = (client_list *)malloc(sizeof(client_list));
            initialize_string(&((*head)->ip));
            if ((*head) == NULL)
            {
                perror("malloc");
            }
            assign_string((*head)->ip, ip);
            (*head)->port = port;
            (*(head))->next = NULL;
            return;
        }
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = (client_list *)malloc(sizeof(client_list));
        initialize_string(&(current->next->ip));
        if (current == NULL)
        {
            perror("malloc");
        }
        assign_string(current->next->ip, ip);
        current->next->port = port;
        (current->next)->next = NULL;
    }

    return;
}

//search for a given tuple
client_list *search_list(client_list *head, my_string *ip, int port)
{
    client_list *current = head;
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

void delete_list(client_list **head, my_string *ip, int port)
{
    if (!head || !(*head))
        return;

    client_list *current = *head;
    client_list *previous = NULL;

    while (strcmp(current->ip->str, ip->str) != 0 && current->port != port && current->next != NULL)
    {
        previous = current;
        current = current->next;
    }

    if (strcmp(current->ip->str, ip->str) == 0 && current->port == port)
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

void print_list(client_list *head)
{
    client_list *current = head;
    if (current == NULL)
    {
        printf("empty list\n");
    }
    else
    {
        printf("%s %d -> ", current->ip->str, current->port);
        while (current->next != NULL)
        {
            current = current->next;
            printf("%s %d -> ", current->ip->str, current->port);
        }
        printf("\n");
    }
    return;
}

void free_list(client_list *head)
{
    client_list *current;

    while (head != NULL)
    {
        current = head;
        head = head->next;
        free(current);
    }
}