#ifndef _CLIENT_HELPERS_H_
#define _CLIENT_HELPERS_H_

#define _GNU_SOURCE_
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/select.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/if_link.h>
#include "file.h"
#include "./../modals/client_message.h"
#include "./../modals/file_storage.h"
#include "./../client_data_structures/client_list.h"
#include "./../client_data_structures/cyclic_buffer.h"
#include "./../../error_handling/error_handling.h"
#include "./../../string_library/string_library.h"

#define MESSAGE_BUFFER 256

void get_address(my_string **ip);
void read_from_server(char *message_buffer, int socket, client_message **message);
void send_log_on(my_string *ip, int port, int socket);
void send_get_clients(int socket, client_list **alive_clients);
void send_log_off(int socket);
cyclic_buffer_node *send_get_file_list(char *ip, int port, int *size, file_storage **saved_files);
void send_get_file(char *ip, int port, int *size, char *pathname, int version, file_storage **saved_files, char *myip, int myport);

#endif