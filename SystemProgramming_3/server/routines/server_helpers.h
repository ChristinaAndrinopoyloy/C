#ifndef _SERVER_HELPERS_H_
#define _SERVER_HELPERS_H_

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
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include "./../../structures/arguments.h"
#include "./../../string_library/string_library.h"
#include "./../modals/server_message.h"
#include "./../server_data_structures/ip_port_list.h"
#include "request_routines.h"
#include "./../modals/server_message.h"

void set_file_nonblock(int *sock);
void make_array_of_select(int *sock, fd_set *sockets, int *highsocket, int *connected_sockets);
void try_accept_connection(int *sock, int *connected_sockets);
void read_sockets(int *sock, fd_set *sockets, int *connected_sockets, ip_port_listnode **iports);
void read_data(int i, int *connected_sockets, ip_port_listnode **iports);

#endif