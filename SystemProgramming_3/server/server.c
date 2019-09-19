/* based on: https://www.lowtek.com/sockets/select.html */
#include "./routines/server_helpers.h"

server_arguments *arg = NULL;
ip_port_listnode *iports = NULL;

void communication_handler(int signo, siginfo_t *siginfo, void *context)
{
    printf("------------goodbye --------------\n");
    destroy_server_arguments(arg);
    free_list(iports);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    sigset_t mask;
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    struct sigaction signalAction;
    signalAction.sa_flags = SA_SIGINFO;
    signalAction.sa_handler = (void (*)(int))communication_handler;
    sigaction(SIGINT, &signalAction, NULL);

    int opt;
    int sock;                                 // socket file descriptor
    int connected_sockets[CONNECT_LIST_SIZE]; // array of connected sockets
    fd_set sockets;                           // socket file descriptors for select
    int highest_socket;
    int port;
    struct sockaddr_in server; // bind struct
    int reuse_address = 1;     // for re-bind to our port
    struct timeval timeout;
    int readsockets; // number of sockets ready for reading

    initialize_server_arguments(&arg); // initialize argument (port) with invalid value
    while ((opt = getopt(argc, argv, ":p:")) != -1)
    {
        switch (opt)
        {
        case 'p':
            arg->portNum = atoi(optarg);
            printf("portNum is: %d\n", arg->portNum);
            break;
        case ':':
            error_handling(1);
            return 1;
            break;
        case '?':
            error_handling(2);
            return 1;
            break;
        }
    }
    printf("\n\n");
    checking_server_arguments(arg); // checking if arguments (port) are valid
    port = arg->portNum;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof(reuse_address)); //re-bind without problems
    set_file_nonblock(&sock);

    memset((char *)&server, 0, sizeof(server)); // initialize
    server.sin_family = AF_INET;                /* Internet domain */
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port); /* The given port */

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0) // bind
    {
        perror("bind");
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (listen(sock, CONNECT_LIST_SIZE) < 0) // listen from clients
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    highest_socket = sock;
    memset((char *)&connected_sockets, 0, sizeof(connected_sockets)); // initialize connected_sockets
    while (1)
    {
        make_array_of_select(&sock, &sockets, &highest_socket, connected_sockets);
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        if ((readsockets = select(highest_socket + 1, &sockets, NULL, NULL, &timeout)) < 0) // select
        {
            perror("select");
            exit(EXIT_FAILURE);
        }
        if (readsockets == 0) // wait to read something from client
        {
            printf(".");
            fflush(stdout);
        }
        else // read from client
        {
            printf("+\n");
            read_sockets(&sock, &sockets, connected_sockets, &iports);
        }
    }
}