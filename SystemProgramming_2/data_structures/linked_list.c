#include "linked_list.h"

//returns true if the list is empty
bool empty_list(list_node *head)
{
    return head == NULL;
}

// returns the length of a list
int length_list(list_node *head)
{
    int length = 0;
    list_node *current;

    for (current = head; current->next != NULL; current = current->next)
    {
        length++;
    }
    length++;
    return length;
}

//insert a node at the end of the list
void insert_list(list_node **head, my_string *data)
{
    list_node *current = *head;

    if (*head == NULL)
    {
        *head = (list_node *)malloc(sizeof(list_node));
        initialize_string(&((*head)->data));
        if ((*head) == NULL)
        {
            perror("malloc");
        }
        assign_string((*head)->data, data);
        // memcpy(&((*(head))->id), data, sizeof(data));
        (*(head))->next = NULL;
        return;
    }
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = (list_node *)malloc(sizeof(list_node));
    initialize_string(&(current->next->data));
    if (current == NULL)
    {
        perror("malloc");
    }
    assign_string(current->next->data, data);
    // memcpy(&(current->next->id), data, sizeof(data));
    (current->next)->next = NULL;

    return;
}

//search for a given id
list_node *search_list(list_node *head, my_string *data)
{
    list_node *current = head;
    if (head == NULL)
    {
        return NULL;
    }

    while (strcmp(data->str, current->data->str) != 0)
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

void delete_list(list_node **head, my_string *id)
{
    if (!head || !(*head))
        return;

    list_node *current = *head;
    list_node *previous = NULL;

    while (strcmp(current->data->str, id->str) != 0 && current->next != NULL)
    {
        previous = current;
        current = current->next;
    }

    if (strcmp(current->data->str, id->str) == 0)
    {
        if (previous)
        {
            previous->next = current->next;
        }
        else
        {
            *head = current->next;
        }
        destroy_string(current->data);
        free(current);
    }
    return;
}

void print_list(list_node *head)
{
    list_node *current = head;
    if (current == NULL)
    {
        printf("empty list\n");
    }
    else
    {
        printf("%s -> ", current->data->str);
        // printf("%d -> ", current->id);
        while (current->next != NULL)
        {
            current = current->next;
            printf("%s -> ", current->data->str);
            // printf("%d -> ", current->id);
        }
        printf("\n");
    }
    return;
}

void free_list(list_node *head)
{
    list_node *current;

    while (head != NULL)
    {
        current = head;
        head = head->next;
        free(current);
    }
}