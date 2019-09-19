#include "server_message.h"

// initialize a struct type of server_message
void initialize_server_message(server_message **message)
{
    *message = (server_message *)malloc(sizeof(server_message));
    if (*message == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    (*message)->client_port = -1;
    (*message)->id = -1;
    initialize_string(&((*message)->type));
    initialize_string(&((*message)->client_ip));
    return;
}

// assign at a struct type of server_message
void assign_server_message(server_message *message, char *message_buffer)
{
    int i = 0;
    char *token;
    char temp[30];
    char log_on[8];
    char get_clients[12];
    char log_off[8];
    char str[INET6_ADDRSTRLEN];

    memset(&temp[0], 0, sizeof(temp));
    memset(&log_on[0], 0, sizeof(log_on));
    memset(&get_clients[0], 0, sizeof(get_clients));
    memset(&log_off[0], 0, sizeof(log_off));

    strcpy(log_on, "LOG_ON "); // type of messages
    strcpy(get_clients, "GET_CLIENTS");
    strcpy(log_off, "LOG_OFF");

    if (memcmp(log_on, message_buffer, sizeof(log_on) - 1) == 0) // LOG_ON
    {
        message->id = 1; // id

        memcpy(temp, &message_buffer[0], 6); // type
        temp[6] = '\0';
        straight_assign_string(message->type, temp);
        printf("message_type = %s\n", message->type->str);
        memset(&temp[0], 0, sizeof(temp));

        memcpy(temp, &message_buffer[7], strlen(message_buffer)); // IP, port
        token = strtok(temp, ",");
        while (token != NULL)
        {
            if (i == 0)
            {
                int sended_ip = ntohl(atoi(token));
                if (inet_ntop(AF_INET, &sended_ip, str, INET6_ADDRSTRLEN) == NULL)
                {
                    perror("inet_ntop");
                    exit(EXIT_FAILURE);
                }
                straight_assign_string(message->client_ip, str);
            }
            else
            {
                message->client_port = ntohl(atoi(token));
            }
            i++;
            token = strtok(NULL, ",");
        }
        printf("message_ip = %s\n", message->client_ip->str);
        printf("message_port = %d\n", message->client_port);
        return;
    }

    if (memcmp(get_clients, message_buffer, sizeof(get_clients) - 1) == 0) // GET_CLIENTS
    {
        message->id = 2; // id

        straight_assign_string(message->type, message_buffer); // type
        printf("message_type = %s\n", message->type->str);
        return;
    }

    if (memcmp(log_off, message_buffer, sizeof(log_off) - 1) == 0) // LOG_OFF
    {
        message->id = 3; // id

        straight_assign_string(message->type, message_buffer); // type
        printf("message_type = %s\n", message->type->str);
        return;
    }

    error_handling(9); // exit
    return;
}

// destroy a struct type of server_message
void destroy_server_message(server_message *message)
{
    destroy_string(message->type);
    destroy_string(message->client_ip);
    free(message);
    return;
}