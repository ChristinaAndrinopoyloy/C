#ifndef _REQUEST_ROUTINES_H_
#define _REQUEST_ROUTINES_H_

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include "file.h"
#include "./../modals/client_message.h"
#include "./../client_data_structures/client_list.h"
#include "./../../error_handling/error_handling.h"
#include "./../../string_library/string_library.h"
#include "./../modals/file_storage.h"

#define MESSAGE_SIZE 256
#define CONNECT_LIST_SIZE FD_SETSIZE

bool serve_request(client_message **message, int *connected_sockets, int me, my_string *pathname, client_list **alive_clients, file_storage **saved_files);
bool user_on_routine(client_list **alive_clients, client_message *message);
bool get_file_list_routine(int *connected_sockets, int me, my_string *pathname, file_storage **saved_files);
bool get_file_routine(int *connected_sockets, int me, client_message *message, char *my_dir, file_storage **saved_files);
bool log_off_routine(client_message *message, client_list **alive_clients);

#endif