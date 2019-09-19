#ifndef _CYCLIC_BUFFER_H_
#define _CYCLIC_BUFFER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "./../../string_library/string_library.h"
#include "./../../error_handling/error_handling.h"

typedef struct Cyclic_Buffer_Node
{
    char pathname[128];
    int version;
    char IP[50];
    int port;
} cyclic_buffer_node;

typedef struct Cyclic_Buffer
{
    cyclic_buffer_node **cbuf;
    int start;
    int end;
    int readen_nodes;
}cyclic_buffer;


void initialize_cyclic_buffer_node(cyclic_buffer_node **node);
void assign_cyclic_buffer_node(cyclic_buffer_node **node, char *pathname, int version, char *ip, int port);
void destroy_cyclic_buffer_node(cyclic_buffer_node *node);
void initialize_cyclic_buffer(cyclic_buffer **buffer, int size_of_buffer);
void write_at_cyclic_buffer(cyclic_buffer_node *node);
void read_from_cyclic_buffer(int size_of_buffer, cyclic_buffer_node **returned_node);
void destroy_cyclic_buffer(cyclic_buffer *buffer, int size_of_buffer);

#endif