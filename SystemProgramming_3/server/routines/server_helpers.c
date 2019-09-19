/* based on: https://www.lowtek.com/sockets/select.html (the same with server.c)*/
#include "server_helpers.h"

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
void read_sockets(int *sock, fd_set *sockets, int *connected_sockets, ip_port_listnode **iports)
{
    int i;

    if (FD_ISSET(*sock, sockets))
    {
        try_accept_connection(sock, connected_sockets);
    }
    for (i = 0; i < CONNECT_LIST_SIZE; i++) // check if can pull data from sockets
    {
        if (FD_ISSET(connected_sockets[i], sockets))
        {
            read_data(i, connected_sockets, iports);
        }
    }
}

// accept
void try_accept_connection(int *sock, int *connected_sockets)
{
    int i, connection;

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
void read_data(int i, int *connected_sockets, ip_port_listnode **iports)
{
    server_message *message;
    char buffer[MESSAGE_SIZE];
    char message_buffer[MESSAGE_SIZE];
    initialize_server_message(&message);
    memset(&buffer[0], 0, sizeof(buffer));
    memset(&message_buffer[0], 0, sizeof(message_buffer));
    int ret;
    bool received_all_message = false;

    while (received_all_message == false)
    {
        if ((ret = read(connected_sockets[i], buffer, MESSAGE_SIZE)) <= 0)
        {
            continue;
        }
        else // read
        {
            if (buffer[ret - 1] == '\0') // message finished
            {
                received_all_message = true;
                strcat(message_buffer, buffer);
                printf("\nReceived: %s \n", message_buffer);
                assign_server_message(message, message_buffer);                     // find the kind of message
                if (serve_request(&message, iports, connected_sockets, i) == false) // serve the request of message
                {
                    error_handling(13);
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