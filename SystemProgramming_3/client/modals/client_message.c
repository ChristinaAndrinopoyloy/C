#include "client_message.h"

// prepare and initialize a client_message
void prepare_client_message(client_message **message, char *message_buffer)
{
    char client_list[13];
    memset(&client_list[0], 0, sizeof(client_list));
    strcpy(client_list, "CLIENT_LIST "); // type of message

    char number_of_clients[3];
    memset(&number_of_clients[0], 0, sizeof(number_of_clients));
    char curr[1];
    int counter = 12;

    if (memcmp(client_list, message_buffer, sizeof(client_list) - 1) == 0) // check for GET_CLIENTS message
    {
        curr[0] = message_buffer[12];
        while ((counter < strlen(message_buffer)) && (*curr != ' '))
        {
            strcat(number_of_clients, curr);
            counter++;
            curr[0] = message_buffer[counter];
        }
        initialize_client_message(message, atoi(number_of_clients));
        return;
    }
    initialize_client_message(message, 1);
    return;
}

// initialize a struct type of client_message
void initialize_client_message(client_message **message, int number_of_clients)
{
    *message = (client_message *)malloc(sizeof(client_message));
    if (*message == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    initialize_string(&((*message)->type));
    (*message)->id = -1;

    (*message)->client_ip = (my_string **)malloc(number_of_clients * sizeof(my_string *));
    if ((*message)->client_ip == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    (*message)->client_port = (int *)malloc(number_of_clients * sizeof(int));
    if ((*message)->client_port == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < number_of_clients; i++)
    {
        initialize_string(&(((*message)->client_ip)[i]));
        ((*message)->client_port)[i] = -1;
    }
    (*message)->counter = number_of_clients;
    return;
}

// assign at a struct type of client_message
void assign_client_message(client_message *message, char *message_buffer)
{
    char user_on[9];
    char client_list[13];
    char user_off[10];
    char error[32];
    memset(&user_on[0], 0, sizeof(user_on));
    memset(&client_list[0], 0, sizeof(client_list));
    memset(&user_off[0], 0, sizeof(user_off));
    memset(&error[0], 0, sizeof(error));
    strcpy(user_on, "USER_ON "); // type of messages
    strcpy(client_list, "CLIENT_LIST ");
    strcpy(user_off, "USER_OFF ");
    strcpy(error, "ERROR_IP_PORT_NOT_FOUND_IN_LIST");

    char temp[40];
    char *token;
    char str[INET6_ADDRSTRLEN];
    int i = 0;

    if (memcmp(user_on, message_buffer, sizeof(user_on) - 1) == 0) // USER_ON
    {
        message->id = 1; // id

        memcpy(temp, &message_buffer[0], 7); // type
        temp[7] = '\0';
        straight_assign_string(message->type, temp);
        printf("message_type = %s\n", message->type->str);
        memset(&temp[0], 0, sizeof(temp));

        memcpy(temp, &message_buffer[8], strlen(message_buffer)); // IP, port
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
                straight_assign_string(message->client_ip[0], str);
            }
            else
            {
                message->client_port[0] = ntohl(atoi(token));
            }
            i++;
            token = strtok(NULL, ",");
        }
        printf("message_ip = %s\n", message->client_ip[0]->str);
        printf("message_port = %d\n", message->client_port[0]);
        return;
    }

    if (memcmp(client_list, message_buffer, sizeof(client_list) - 1) == 0) // CLIENT_LIST N
    {
        message->id = 2;

        memcpy(temp, &message_buffer[0], 11); // type
        temp[11] = '\0';
        straight_assign_string(message->type, temp);
        printf("message_type = %s\n", message->type->str);
        memset(&temp[0], 0, sizeof(temp));

        int digits = 0; // find the number of clients (N)
        int index = 0;
        char curr[1];
        curr[0] = message_buffer[12];
        char number_of_clients[3];
        memset(&number_of_clients[0], 0, sizeof(number_of_clients));

        while ((digits < strlen(message_buffer) - 12) && (curr[0] != ' '))
        {
            digits++;
            strcat(number_of_clients, curr);
            curr[0] = message_buffer[12 + digits];
        }

        if (message->counter > 0)
        {
            memcpy(temp, &message_buffer[12 + digits], strlen(message_buffer)); // IP, port
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
                    straight_assign_string(message->client_ip[index], str);
                    i = 1;
                }
                else
                {
                    i = 0;
                    message->client_port[index] = ntohl(atoi(token));
                    index++;
                }
                token = strtok(NULL, ",");
            }
        }

        return;
    }

    if (memcmp(user_off, message_buffer, sizeof(user_off) - 1) == 0) // USER_OFF
    {
        message->id = 3; // id

        memcpy(temp, &message_buffer[0], 8); // type
        temp[8] = '\0';
        straight_assign_string(message->type, temp);
        printf("message_type = %s\n", message->type->str);
        memset(&temp[0], 0, sizeof(temp));

        memcpy(temp, &message_buffer[9], strlen(message_buffer)); // IP, port
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
                straight_assign_string(message->client_ip[0], str);
            }
            else
            {
                message->client_port[0] = ntohl(atoi(token));
            }
            i++;
            token = strtok(NULL, ",");
        }
        printf("message_ip = %s\n", message->client_ip[0]->str);
        printf("message_port = %d\n", message->client_port[0]);
        return;
    }

    if (memcmp("ERROR_IP_PORT_NOT_FOUND_IN_LIST", message_buffer, 31) == 0) // ERROR_IP_PORT_NOT_FOUND_IN_LIST
    {
        message->id = 4; // id

        memcpy(temp, &message_buffer[0], 31); // type
        temp[32] = '\0';
        straight_assign_string(message->type, temp);
        printf("message_type = %s\n", message->type->str);
        return;
    }

    if (memcmp("GET_FILE_LIST", message_buffer, 13) == 0) // GET_FILE_LIST
    {
        message->id = 5; // id

        memcpy(temp, &message_buffer[0], 13); // type
        temp[14] = '\0';
        straight_assign_string(message->type, temp);
        printf("message_type = %s\n", message->type->str);
        return;
    }

    if (memcmp("GET_FILE ", message_buffer, 9) == 0) // GET_FILE
    {
        message->id = 6; // id

        memcpy(temp, &message_buffer[0], 9); // type
        temp[9] = '\0';
        straight_assign_string(message->type, temp);
        printf("message_type = %s\n", message->type->str);

        int path_flag = true;
        memcpy(temp, &message_buffer[9], strlen(message_buffer)); // IP, port
        token = strtok(temp, ",");
        while (token != NULL)
        {
            if (path_flag == true)
            {
                message->pathnames = (char *)malloc(strlen(token));
                strcpy(message->pathnames, token);
                path_flag = false;
            }
            else
            {
                path_flag = true;
                message->version = atoi(token);
            }
            token = strtok(NULL, ",");
        }
        return;
    }

    if (memcmp("FILE_LIST", message_buffer, 9) == 0) // FILE_LIST N
    {
        message->id = 7;

        memcpy(temp, &message_buffer[0], 9); // type
        temp[10] = '\0';
        straight_assign_string(message->type, temp);
        printf("message_type = %s\n", message->type->str);

        int digits = 0; // find the number of files (N)
        int index = 0;
        char curr[1];
        curr[0] = message_buffer[10];
        char number_of_files[3];
        memset(&number_of_files[0], 0, sizeof(number_of_files));
        while ((digits < strlen(message_buffer) - 9) && (curr[0] != ' '))
        {
            digits++;
            strcat(number_of_files, curr);
            curr[0] = message_buffer[10 + digits];
        }

        strcpy(message->pathnames, &(message_buffer[10 + digits]));
        printf("files = %s\n", message->pathnames);
    }
}

// destroy a struct type of server_message
void destroy_client_message(client_message *message, int number_of_clients)
{
    destroy_string(message->type);
    for (int i = 0; i < number_of_clients; i++)
    {
        destroy_string(message->client_ip[i]);
    }
    free(message->client_ip);
    free(message->client_port);
    free(message);
    return;
}