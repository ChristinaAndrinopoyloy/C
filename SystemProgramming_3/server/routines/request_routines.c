#include "request_routines.h"

bool serve_request(server_message **message, ip_port_listnode **iports, int *connected_sockets, int me)
{
    switch ((*message)->id)
    {
    case 1: // LOG_ON
        if (log_on_routine(iports, (*message)->client_ip, (*message)->client_port, connected_sockets, me) == false)
        {
            error_handling(10);
            return false;
        }
        break;
    case 2: // GET_CLIENTS
        sleep(2);
        if (get_clients_routine(iports, connected_sockets, me) == false)
        {
            error_handling(11);
            return false;
        }
        break;
    case 3: // LOG_OFF
        sleep(2);
        if (log_off_routine(iports, connected_sockets, me) == false)
        {
            error_handling(12);
            return false;
        }
        break;
    default:
        error_handling(13);
        return false;
        break;
    }
    return true;
}

// received LOG_ON and send USER_ON at others clients
bool log_on_routine(ip_port_listnode **iports, my_string *ip, int port, int *connected_sockets, int me)
{
    int sock;
    struct hostent *rem;
    struct sockaddr_in my_server;
    int length = 0;
    char buffer[MESSAGE_SIZE];
    memset(&buffer[0], 0, sizeof(buffer)); // clean buffer

    struct sockaddr_in temp;
    int ret;
    if ((ret = inet_pton(AF_INET, ip->str, &(temp.sin_addr.s_addr))) <= 0) // IP such a unsigned long
    {
        if (ret == 0)
        {
            error_handling(14);
        }
        else
        {
            perror("inet_pton");
            exit(EXIT_FAILURE);
        }
    }

    sprintf(buffer, "USER_ON %lu,%lu", htonl((unsigned int)(temp.sin_addr.s_addr)), htonl(port)); // USER_ON IP,port
    buffer[strlen(buffer)] = '\0';

    ip_port_listnode *current = *iports;
    while (current != NULL)
    {
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) // create socket
        {
            perror("socket");
            exit(EXIT_FAILURE);
        }
        /* Find server address */
        if ((rem = gethostbyname(current->ip->str)) == NULL)
        {
            herror("gethostbyname");
            exit(1);
        }
        my_server.sin_family = AF_INET; // Internet domain
        memcpy(&my_server.sin_addr, rem->h_addr, rem->h_length);
        my_server.sin_port = htons(current->port);

        if (connect(sock, (struct sockaddr *)&my_server, sizeof(my_server)) < 0) // innitiate connection
        {
            perror("connect");
            exit(EXIT_FAILURE);
        }

        if (write(sock, buffer, MESSAGE_SIZE) < 0) // write
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
        printf("                        send at %s %d: |%s|\n", current->ip->str, current->port, buffer);
        current = current->next;
    }

    insert_list(iports, ip, port, connected_sockets[me]); // insert the new client at a list
    // print_list(*iports);
    return true;
}

// receive GET_CLIENTS and send at this client CLIENT_LIST with the other clients
bool get_clients_routine(ip_port_listnode **iports, int *connected_sockets, int me)
{
    struct sockaddr_in temp;
    int ret;
    ip_port_listnode *current = NULL;
    int number_of_clients = length_list(*iports) - 1; // the number of alive clients right now at server
    char client_list[MESSAGE_SIZE];

    memset(&client_list[0], 0, sizeof(client_list));                                     // clean client_list
    strcpy(client_list, "CLIENT_LIST ");                                                 // CLIENT_LIST
    strcat(client_list, itoa(number_of_clients, length_of_int(number_of_clients))->str); // CLIENT_LIST N
    strcat(client_list, " ");
    printf("                        send %s\n", client_list);
    for (int j = 0; client_list[j] != '\0'; j++)
    {
        if (write(connected_sockets[me], client_list + j, 1) < 0) // write char by char
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    current = *iports;
    if (current != NULL)
    {
        do
        {
            if (current->connection_list_no != connected_sockets[me]) // if this client is not me
            {
                memset(&client_list[0], 0, sizeof(client_list));                                // clean client_list
                if ((ret = inet_pton(AF_INET, current->ip->str, &(temp.sin_addr.s_addr))) <= 0) // IP such a unsigned long
                {
                    if (ret == 0)
                    {
                        error_handling(14);
                    }
                    else
                    {
                        perror("inet_pton");
                        exit(EXIT_FAILURE);
                    }
                }

                sprintf(client_list, "%lu,%lu,", htonl((unsigned int)(temp.sin_addr.s_addr)), htonl(current->port)); // IP,port
                if (write(connected_sockets[me], client_list, strlen(client_list)) < 0) // write char by char
                {
                    perror("write");
                    exit(EXIT_FAILURE);
                }
            }
            current = current->next;
        } while (current != NULL);
    }

    char finish[1];
    finish[0] = '\0';
    if (write(connected_sockets[me], finish, 1) < 0) // finish of message
    {
        perror("write");
        exit(EXIT_FAILURE);
    }

    return true;
}

// receive LOG_OFF and send USER_OFF at other clients or ERROR_IP_PORT_NOT_FOUNDD_IN_LIST (for error)
bool log_off_routine(ip_port_listnode **iports, int *connected_sockets, int me)
{
    ip_port_listnode *returned_ip_port = NULL;
    returned_ip_port = search_list_by_connection_list_no(*iports, connected_sockets[me]);

    if (returned_ip_port != NULL) // there is the ip,socket (from LOG_OFF) at connected_sockets
    {
        int sock;
        struct hostent *rem;
        int ret;
        struct sockaddr_in temp;
        struct sockaddr_in my_server;
        char user_off[MESSAGE_SIZE];
        memset(&user_off[0], 0, sizeof(user_off)); // clean user_off

        if ((ret = inet_pton(AF_INET, returned_ip_port->ip->str, &(temp.sin_addr.s_addr))) <= 0) // IP such a unsigned long
        {
            if (ret == 0)
            {
                error_handling(14);
            }
            else
            {
                perror("inet_pton");
                exit(EXIT_FAILURE);
            }
        }

        sprintf(user_off, "USER_OFF %lu,%lu", htonl((unsigned int)(temp.sin_addr.s_addr)), htonl(returned_ip_port->port)); // LOG_OFF IP,port
        ip_port_listnode *current = *iports;
        while (current != NULL)
        {
            if (current->connection_list_no != connected_sockets[me])
            {
                if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) // create socket
                {
                    perror("socket");
                    exit(EXIT_FAILURE);
                }
                /* Find server address */
                if ((rem = gethostbyname(current->ip->str)) == NULL)
                {
                    herror("gethostbyname");
                    exit(1);
                }
                my_server.sin_family = AF_INET; // Internet domain
                memcpy(&my_server.sin_addr, rem->h_addr, rem->h_length);
                my_server.sin_port = htons(current->port);

                if (connect(sock, (struct sockaddr *)&my_server, sizeof(my_server)) < 0) // innitiate connection
                {
                    perror("connect");
                    exit(EXIT_FAILURE);
                }

                if (write(sock, user_off, strlen(user_off) + 1) < 0) // write
                {
                    perror("write");
                    exit(EXIT_FAILURE);
                }
            }
            current = current->next;
        }

        delete_list(iports, connected_sockets[me]);
        connected_sockets[me] = 0;
    }
    else // cannot find the ip,socket (from LOG_OFF) at connected_sockets
    {
        char error[32];
        memset(&error[0], 0, sizeof(error)); // clean error
        strcpy(error, "ERROR_IP_PORT_NOT_FOUND_IN_LIST");
        error[32] = '\0';
        for (int j = 0; error[j] != '\0'; j++)
        {
            if (write(connected_sockets[me], error + j, 1) < 0)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
    }
    return true;
}