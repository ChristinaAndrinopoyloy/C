#ifndef _CLIENT_MESSAGE_H_
#define _CLIENT_MESSAGE_H_

#include <arpa/inet.h>
#include <stdlib.h>
#include <stdbool.h>
#include "./../../string_library/string_library.h"
#include "./../../error_handling/error_handling.h"

typedef struct Client_Message
{
    my_string *type;
    int id; // 1 = USER_ON, 2 = CLIENT_LIST, 3 = USER_OFF, 4 = ERROR_IP_PORT_NOT_FOUND_IN_LIST, 5 = GET_FILE_LIST, 6 = GET_FILE, 7 = FILE_LIST
    int *client_port;
    my_string **client_ip;
    int counter;
    char *pathnames;
    int version;
} client_message;

void prepare_client_message(client_message **message,char *message_buffer);
void initialize_client_message(client_message **message, int number_of_clients);
void assign_client_message(client_message *message, char *message_buffer);
void destroy_client_message(client_message *message, int number_of_clients);

#endif