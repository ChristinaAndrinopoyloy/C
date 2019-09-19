#include "file_storage.h"

//returns true if the list is empty
bool file_storage_check_empty(file_storage *head)
{
    return head == NULL;
}

// returns the length of a list
int file_storage_length(file_storage *head)
{
    int length = 0;
    file_storage *current;

    for (current = head; current->next != NULL; current = current->next)
    {
        length++;
    }
    length++;
    return length;
}

//insert a node at the end of the list
bool file_storage_assign(file_storage **head, my_string *pathname, int version)
{
    file_storage *ret;
    file_storage *current = *head;

    if ((ret = file_storage_search(head, pathname, version)) == NULL)
    {
        if (*head == NULL)
        {
            *head = (file_storage *)malloc(sizeof(file_storage));
            if ((*head) == NULL)
            {
                perror("malloc");
            }
            initialize_string(&((*head)->path));
            assign_string((*head)->path, pathname);

            (*head)->version = version;
            (*(head))->next = NULL;
            return true;
        }
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = (file_storage *)malloc(sizeof(file_storage));
        if (current == NULL)
        {
            perror("malloc");
        }
        initialize_string(&(current->next->path));
        assign_string(current->next->path, pathname);

        current->next->version = version;
        (current->next)->next = NULL;
        return true;
    }
    else
    {
        if (ret->version != version)
        {
            file_storage_remove(head, ret->path, ret->version);
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = (file_storage *)malloc(sizeof(file_storage));
            if (current == NULL)
            {
                perror("malloc");
            }
            initialize_string(&(current->next->path));
            assign_string(current->next->path, pathname);

            current->next->version = version;
            (current->next)->next = NULL;
        }
        return false;
    }
}

//search for a given tuple
file_storage *file_storage_search(file_storage **head, my_string *pathname, int version)
{
    file_storage *current = (*head);
    if ((*head) == NULL)
    {
        return NULL;
    }

    while (strcmp(pathname->str, current->path->str) != 0)
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

void file_storage_remove(file_storage **head, my_string *pathname, int version)
{
    if (!head || !(*head))
        return;

    file_storage *current = *head;
    file_storage *previous = NULL;

    while (strcmp(current->path->str, pathname->str) != 0 && current->next != NULL)
    {
        previous = current;
        current = current->next;
    }

    if ((strcmp(current->path->str, pathname->str) == 0) && (current->version != version))
    {
        if (previous)
        {
            previous->next = current->next;
        }
        else
        {
            *head = current->next;
        }
        destroy_string(current->path);
        free(current);
    }
    return;
}

void file_storage_print(file_storage *head)
{
    file_storage *current = head;
    if (current == NULL)
    {
        printf("empty list\n");
    }
    else
    {
        printf("%s %d -> ", current->path->str, current->version);
        while (current->next != NULL)
        {
            current = current->next;
            printf("%s %d -> ", current->path->str, current->version);
        }
        printf("\n");
    }
    return;
}

void file_storage_free(file_storage *head)
{
    file_storage *current;

    while (head != NULL)
    {
        current = head;
        head = head->next;
        destroy_string(current->path);
        free(current);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////