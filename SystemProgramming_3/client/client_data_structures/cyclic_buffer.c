#include "cyclic_buffer.h"

void initialize_cyclic_buffer_node(cyclic_buffer_node **node)
{
    (*node) = (cyclic_buffer_node *)malloc(sizeof(cyclic_buffer_node));
    if ((*node) == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    (*node)->port = -1;
    (*node)->version = -1;

    memset(&(*node)->pathname[0], 0, sizeof((*node)->pathname));
    memset(&(*node)->IP[0], 0, sizeof((*node)->IP));
    return;
}

void assign_cyclic_buffer_node(cyclic_buffer_node **node, char *pathname, int version, char *ip, int port)
{
    (*node)->version = version;
    (*node)->port = port;
    if (pathname != NULL)
    {
        strncpy((*node)->pathname, pathname, strlen(pathname));
    }
    if (ip != NULL)
    {
        strncpy((*node)->IP, ip, strlen(ip));
    }
    return;
}

void destroy_cyclic_buffer_node(cyclic_buffer_node *node)
{
    free(node);
    return;
}

void initialize_cyclic_buffer(cyclic_buffer **buffer, int size_of_buffer)
{
    (*buffer) = (cyclic_buffer *)malloc(sizeof(cyclic_buffer));
    if ((*buffer) == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    (*buffer)->cbuf = (cyclic_buffer_node **)malloc(size_of_buffer * sizeof(cyclic_buffer_node *));
    if ((*buffer)->cbuf == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size_of_buffer; i++)
    {
        initialize_cyclic_buffer_node(&(*buffer)->cbuf[i]);
    }

    (*buffer)->readen_nodes = 0;
    (*buffer)->start = 0;
    (*buffer)->end = -1;
    return;
}

void destroy_cyclic_buffer(cyclic_buffer *buffer, int size_of_buffer)
{
    for (int i = 0; i < size_of_buffer; i++)
    {
        free(&(buffer->cbuf[i]));
    }
    free(buffer);
    return;
}