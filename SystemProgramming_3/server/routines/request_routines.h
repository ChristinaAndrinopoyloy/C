#ifndef _REQUEST_ROUTINES_H_
#define _REQUEST_ROUTINES_H_

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/select.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include "./../modals/server_message.h"
#include "./../server_data_structures/ip_port_list.h"
#include "./../../error_handling/error_handling.h"
#include "./../../string_library/string_library.h"

#define MESSAGE_SIZE 256
#define CONNECT_LIST_SIZE FD_SETSIZE

bool serve_request(server_message **message, ip_port_listnode **iports, int *connwcted_sockets, int me);
bool log_on_routine(ip_port_listnode **iports, my_string *ip, int port, int *connected_sockets, int me);
bool get_clients_routine(ip_port_listnode **iports, int *connected_sockets, int me);
bool log_off_routine(ip_port_listnode **iports, int *connected_sockets, int me);

#endif