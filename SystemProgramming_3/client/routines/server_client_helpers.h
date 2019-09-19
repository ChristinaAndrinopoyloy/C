#ifndef _SERVER_CLIENT_HELPERS_H_
#define _SERVER_CLIENT_HELPERS_H_

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include "./../../string_library/string_library.h"
#include "./../modals/client_message.h"
#include "./server_client_request_routines.h"
#include "./../client_data_structures/client_list.h"
#include "./../modals/file_storage.h"

#define CONNECT_LIST_SIZE FD_SETSIZE
#define MESSAGE_SIZE 256

void set_file_nonblock(int *sock);
void make_array_of_select(int *sock, fd_set *sockets, int *highsocket, int *connected_sockets);
void try_accept_connection(int *sock, int *connected_sockets, client_list *alive_clients);
void read_sockets(int *sock, fd_set *sockets, int *connected_sockets, client_list **alive_clients, my_string *pathname, file_storage **saved_files);
void read_data(int i, int *connected_sockets, my_string *pathname, client_list **alive_clients, file_storage **saved_files);

#endif