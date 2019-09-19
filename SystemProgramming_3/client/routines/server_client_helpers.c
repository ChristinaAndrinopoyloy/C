/* based on: https://www.lowtek.com/sockets/select.html (the same with server.c)*/
#include "server_client_helpers.h"

// set file nonblock
void set_file_nonblock(int *sock)
{
    int opts;

    if ((opts = fcntl(*sock, F_GETFL)) < 0) // returns the file access mode
    {
        perror("fcntl(F_GETFL)");
        exit(EXIT_FAILURE);
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(*sock, F_SETFL, opts) < 0) // set the file status flags
    {
        perror("fcntl(F_SETFL)");
        exit(EXIT_FAILURE);
    }
    return;
}

// make a "list of sockets"
void make_array_of_select(int *sock, fd_set *sockets, int *highsocket, int *connected_sockets)
{
    FD_ZERO(sockets); // initialize sockets
    FD_SET(*sock, sockets);
    for (int i = 0; i < CONNECT_LIST_SIZE; i++) // add sockets at fd_set
    {
        if (connected_sockets[i] != 0)
        {
            FD_SET(connected_sockets[i], sockets);
            if (connected_sockets[i] > *highsocket)
            {
                (*highsocket) = connected_sockets[i];
            }
        }
    }
}

// check for data from my clients
void read_sockets(int *sock, fd_set *sockets, int *connected_sockets, client_list **alive_clients, my_string *pathname, file_storage **saved_files)
{
    int i;
    if (FD_ISSET(*sock, sockets))
    {
        try_accept_connection(sock, connected_sockets, *alive_clients);
    }
    for (i = 0; i < CONNECT_LIST_SIZE; i++) // check if can pull data from sockets
    {
        if (FD_ISSET(connected_sockets[i], sockets))
        {
            read_data(i, connected_sockets, pathname, alive_clients, saved_files);
        }
    }
}

// accept
void try_accept_connection(int *sock, int *connected_sockets, client_list *alive_clients)
{
    int ret;
    int i, connection;
    struct sockaddr_in client;
    struct sockaddr *clientptr = (struct sockaddr *)&client;
    socklen_t clientlen;
    if ((connection = accept(*sock, NULL, NULL)) < 0) // accept a new connection
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    set_file_nonblock(&connection);
    for (i = 0; (i < CONNECT_LIST_SIZE) && (connection != -1); i++)
    {
        if (connected_sockets[i] == 0) // connection accepted
        {
            printf("\n\nConnection accepted:   FD=%d; Slot=%d\n", connection, i);
            connected_sockets[i] = connection;
            connection = -1;
        }
    }
    if (connection != -1) // cannot accept connection
    {
        printf("\nNo room left for new client.\n");
        close(connection);
    }
}

// read data from a client
void read_data(int i, int *connected_sockets, my_string *pathname, client_list **alive_clients, file_storage **saved_files)
{
    client_message *message;
    char buffer[MESSAGE_SIZE];
    char message_buffer[MESSAGE_SIZE];
    memset(&buffer[0], 0, sizeof(buffer));
    memset(&message_buffer[0], 0, sizeof(message_buffer));
    int ret;
    bool received_all_message = false;

    while (received_all_message == false)
    {
        if ((ret = read(connected_sockets[i], buffer, MESSAGE_SIZE)) <= 0) // connection lost
        {
            close(connected_sockets[i]);
            connected_sockets[i] = 0;
            // continue;
            break;
        }
        else // read
        {
            if (buffer[ret - 1] == '\0' || buffer[ret] == '\0') // message finished
            {
                received_all_message = true;
                strcat(message_buffer, buffer);
                printf("\nReceived: %s \n", message_buffer);
                prepare_client_message(&message, message_buffer); // prepare and initialize client_message
                assign_client_message(message, message_buffer);
                if (serve_request(&message, connected_sockets, i, pathname, alive_clients, saved_files) == false) // serve the request of message
                {
                    error_handling(17);
                }
            }
            else
            {
                buffer[ret] = '\0';
                strcat(message_buffer, buffer);
            }
        }
    }
}