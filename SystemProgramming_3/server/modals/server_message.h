#ifndef _SERVER_MESSAGE_H_
#define _SERVER_MESSAGE_H_

#include <arpa/inet.h>
#include "./../../string_library/string_library.h"
#include "./../../error_handling/error_handling.h"

typedef struct Server_Message
{
    my_string *type;
    int id; // 1 = LOG_ON, 2 = GET_CLIENTS, 3 = LOG_OFF
    int client_port;
    my_string *client_ip;
} server_message;

void initialize_server_message(server_message **message);
void assign_server_message(server_message *message, char *message_buffer);
void destroy_server_message(server_message *message);

#endif