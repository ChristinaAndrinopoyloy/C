#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <getopt.h>
#include <pthread.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <poll.h>
#include "../string_library/string_library.h"
#include "./../structures/arguments.h"
#include "./routines/client_helpers.h"
#include "./client_data_structures/client_list.h"
#include "./client_data_structures/cyclic_buffer.h"
#include "./routines/server_client_helpers.h"
#include "./modals/file_storage.h"

#define MESSAGE_SIZE 256

pthread_mutex_t mtx;
pthread_cond_t cond_nonempty;
pthread_cond_t cond_nonfull;

client_list *alive_clients = NULL;
cyclic_buffer *Cbuffer;
client_arguments *arg = NULL;
my_string *ip;
bool finish_flag = false;

pthread_t *workers;
int sock;
int number_of_cyclic_nodes = 200;
file_storage *saved_files = NULL;
char **root_files;
int number_of_root_files;

void communication_handler(int signo, siginfo_t *siginfo, void *context)
{
    finish_flag = true;
    pthread_cond_broadcast(&cond_nonempty);
    pthread_cond_broadcast(&cond_nonfull);

    printf("------------handler --------------\n");
    send_log_off(sock);
    for (int i = 0; i < arg->workerThreads; i++)
    {
        printf("i = %d\n", i);
        pthread_join(workers[i], NULL);
    }

    free(workers);
    free(root_files);
    free_list(alive_clients);
    destroy_string(ip);
    destroy_client_arguments(arg);
    pthread_cond_destroy(&cond_nonempty);
    pthread_cond_destroy(&cond_nonfull);
    pthread_mutex_destroy(&mtx);
    close(sock); // close socket and exit

    exit(EXIT_SUCCESS);
}

void *work(void *argS)
{
    cyclic_buffer_node *received_nodes = NULL;
    cyclic_buffer_node *returned_node = NULL;

    while (true)
    {
        usleep(500000);
        while (Cbuffer->readen_nodes > 0 || number_of_cyclic_nodes > 0)
        {
            initialize_cyclic_buffer_node(&returned_node);
            read_from_cyclic_buffer(arg->bufferSize, &returned_node); // consume from cyclic buffer
            pthread_cond_signal(&cond_nonfull);
            usleep(500000);
            if (returned_node != NULL) // valid consume
            {
                if (returned_node->port != -1 && returned_node->version == -1) // worker will send GET_FILE_LIST message
                {
                    my_string *temp_ip;
                    initialize_string(&temp_ip);
                    straight_assign_string(temp_ip, returned_node->IP);
                    client_list *client = search_list(alive_clients, temp_ip, returned_node->port);
                    if (client != NULL) // i have this client at system
                    {
                        int size;
                        received_nodes = send_get_file_list(returned_node->IP, returned_node->port, &size, &saved_files); // send GET_FILE_LIST receive FILE_LIST
                        number_of_cyclic_nodes = size;
                        while (number_of_cyclic_nodes > 0)
                        {
                            for (int i = 0; i < size; i++)
                            {
                                write_at_cyclic_buffer(&(received_nodes[i]));
                                pthread_cond_signal(&cond_nonempty);
                                usleep(0);
                                number_of_cyclic_nodes--;
                            }
                        }
                    }
                    destroy_string(temp_ip);
                }
                if (returned_node->port != -1 && returned_node->version != -1) // send GET_FILE
                {
                    my_string *temp_ip;
                    my_string *temp_path;
                    initialize_string(&temp_ip);
                    initialize_string(&temp_path);
                    straight_assign_string(temp_ip, returned_node->IP);
                    client_list *client = search_list(alive_clients, temp_ip, returned_node->port);
                    if (client != NULL) // i have this client at system
                    {
                        straight_assign_string(temp_path, returned_node->pathname);
                        int size;
                        send_get_file(returned_node->IP, returned_node->port, &size, returned_node->pathname, returned_node->version, &saved_files, ip->str, arg->portNum); // send GET_FILE
                    }
                    destroy_string(temp_ip);
                    destroy_string(temp_path);
                }
            }
            destroy_cyclic_buffer_node(returned_node);
        }
        if (finish_flag == true)
        {
            pthread_mutex_unlock(&mtx);
            pthread_cond_broadcast(&cond_nonempty);
            pthread_cond_broadcast(&cond_nonfull);
            pthread_exit(NULL);
        }
    }
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

    int ret;
    int opt;
    int port, i;
    char buf[MESSAGE_SIZE];
    char buf2[MESSAGE_SIZE];
    char message_buffer[MESSAGE_SIZE];
    struct sockaddr_in my_server;
    struct sockaddr *serverptr = (struct sockaddr *)&my_server;
    struct hostent *rem;
    bool received_all_message = false;
    initialize_string(&ip);
    get_address(&ip);
    printf("ip: %s\n", ip->str);

    initialize_client_arguments(&arg);
    while ((opt = getopt(argc, argv, ":d:p:w:b:s:i:")) != -1)
    {
        switch (opt)
        {
        case 'd':
            straight_assign_string(arg->dirName, optarg);
            printf("dirName is: %s\n", arg->dirName->str);
            break;
        case 'p':
            arg->portNum = atoi(optarg);
            printf("portNum is: %d\n", arg->portNum);
            break;
        case 'w':
            arg->workerThreads = atoi(optarg);
            printf("workerThreads are: %d\n", arg->workerThreads);
            break;
        case 'b':
            arg->bufferSize = atoi(optarg);
            printf("bufferSize is: %d\n", arg->bufferSize);
            number_of_cyclic_nodes = arg->bufferSize;
            break;
        case 's':
            arg->serverPort = atoi(optarg);
            printf("serverPort is: %d\n", arg->serverPort);
            break;
        case 'i':
            straight_assign_string(arg->serverIP, optarg);
            printf("serverIP is: %s\n", arg->serverIP->str);
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
    checking_client_arguments(arg);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    my_string *myip;
    my_string *mypath;
    initialize_string(&myip);
    initialize_string(&mypath);
    straight_assign_string(myip, ip->str);
    int index = 0;
    get_number_of_files_of(arg->dirName, &number_of_root_files);
    root_files = (char **)malloc(number_of_root_files * sizeof(char *));
    if (root_files == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    get_files_of(arg->dirName, root_files, &index);
    int stamp = get_timestamp(current_date_function(), current_time_function());
    for (int i = 0; i < number_of_root_files; i++)
    {
        straight_assign_string(mypath, root_files[i]);
        file_storage_assign(&saved_files, mypath, stamp);
    }
    file_storage_print(saved_files);
    destroy_string(mypath);
    destroy_string(myip);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) // create socket
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* Find server address */
    if ((rem = gethostbyname(arg->serverIP->str)) == NULL)
    {
        herror("gethostbyname");
        exit(1);
    }

    port = arg->serverPort;
    my_server.sin_family = AF_INET; // Internet domain
    memcpy(&my_server.sin_addr, rem->h_addr, rem->h_length);
    my_server.sin_port = htons(port);

    if (connect(sock, (struct sockaddr *)&my_server, sizeof(my_server)) < 0) // innitiate connection
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    printf("Connecting to %s port %d\n\n", arg->serverIP->str, port);

    send_log_on(ip, arg->portNum, sock);    // send LOG_ON to server
    send_get_clients(sock, &alive_clients); // send GET_CLIENTS to server

    pthread_mutex_init(&mtx, 0); // mutex and conditions for worker threads
    pthread_cond_init(&cond_nonempty, 0);
    pthread_cond_init(&cond_nonfull, 0);

    client_list *current_at_list = alive_clients;
    initialize_cyclic_buffer(&Cbuffer, arg->bufferSize); // born cyclic buffer
    while (current_at_list != NULL)
    {
        cyclic_buffer_node *temp_cyclic_buffer_node;
        initialize_cyclic_buffer_node(&temp_cyclic_buffer_node);
        assign_cyclic_buffer_node(&temp_cyclic_buffer_node, NULL, -1, current_at_list->ip->str, current_at_list->port); // make a cyclic buffer node with data from list (ip, port)
        write_at_cyclic_buffer(temp_cyclic_buffer_node);
        pthread_cond_signal(&cond_nonempty);
        destroy_cyclic_buffer_node(temp_cyclic_buffer_node);
        current_at_list = current_at_list->next;
    }

    int error;
    workers = malloc(arg->workerThreads * sizeof(pthread_t));
    if (workers == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < arg->workerThreads; i++) // born workers
    {
        if (error = pthread_create(workers + i, NULL, work, NULL))
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int _sock;                                 // socket file descriptor
    int _connected_sockets[CONNECT_LIST_SIZE]; // array of connected sockets
    fd_set _sockets;                           // socket file descriptors for select
    int _highest_socket;
    int _port = arg->portNum;
    struct sockaddr_in _server; // bind struct
    int _reuse_address = 1;     // for re-bind to our port
    struct timeval _timeout;
    int _readsockets; // number of sockets ready for reading
    int previous_num_alive_clients = 0;

    if ((_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &_reuse_address, sizeof(_reuse_address)); //re-bind without problems
    set_file_nonblock(&_sock);

    memset((char *)&_server, 0, sizeof(_server)); // initialize
    _server.sin_family = AF_INET;                 /* Internet domain */
    _server.sin_addr.s_addr = htonl(INADDR_ANY);
    _server.sin_port = htons(_port); /* The given port */

    if (bind(_sock, (struct sockaddr *)&_server, sizeof(_server)) < 0) // bind
    {
        perror("bind");
        close(_sock);
        exit(EXIT_FAILURE);
    }

    if (listen(_sock, CONNECT_LIST_SIZE) < 0) // listen from clients
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    _highest_socket = _sock;
    memset((char *)&_connected_sockets, 0, sizeof(_connected_sockets)); // initialize connected_sockets
    while (1)
    {
        make_array_of_select(&_sock, &_sockets, &_highest_socket, _connected_sockets);
        _timeout.tv_sec = 1;
        _timeout.tv_usec = 0;

        if ((_readsockets = select(_highest_socket + 1, &_sockets, NULL, NULL, &_timeout)) < 0) // select
        {
            perror("select");
            exit(EXIT_FAILURE);
        }
        if (_readsockets == 0) // wait to read something from client
        {
            printf(".");
            fflush(stdout);
        }
        else // read from client
        {
            previous_num_alive_clients = length_list(alive_clients);
            read_sockets(&_sock, &_sockets, _connected_sockets, &alive_clients, arg->dirName, &saved_files);
            if (previous_num_alive_clients < length_list(alive_clients))
            {
                client_list *pre;
                client_list *cur = alive_clients;
                while (cur != NULL)
                {
                    pre = cur;
                    cur = cur->next;
                }
                cyclic_buffer_node *temp_cb_node;
                initialize_cyclic_buffer_node(&temp_cb_node);
                assign_cyclic_buffer_node(&temp_cb_node, NULL, -1, pre->ip->str, pre->port); // make a cyclic buffer node with data from list (ip, port)
                write_at_cyclic_buffer(temp_cb_node);
                pthread_cond_signal(&cond_nonempty);
                destroy_cyclic_buffer_node(temp_cb_node);
                for (int i = 0; i < arg->bufferSize; i++)
                {
                    if (Cbuffer->cbuf[i]->port != -1)
                    {
                        number_of_cyclic_nodes++;
                    }
                }
            }
        }
    }

    destroy_string(ip);
    return 0;
}

void read_from_cyclic_buffer(int size_of_buffer, cyclic_buffer_node **returned_node)
{
    pthread_mutex_lock(&mtx);
    while (Cbuffer->readen_nodes <= 0)
    {
        // printf(">> Found  Buffer  Empty \n");
        pthread_cond_wait(&cond_nonempty, &mtx);
        if (finish_flag == true)
        {
            pthread_mutex_unlock(&mtx);
            pthread_cond_broadcast(&cond_nonfull);
            pthread_cond_broadcast(&cond_nonempty);
            pthread_exit(NULL);
        }
    }

    assign_cyclic_buffer_node(returned_node, Cbuffer->cbuf[Cbuffer->start]->pathname, Cbuffer->cbuf[Cbuffer->start]->version,
                              Cbuffer->cbuf[Cbuffer->start]->IP, Cbuffer->cbuf[Cbuffer->start]->port);
    Cbuffer->start = (Cbuffer->start + 1) % size_of_buffer;
    Cbuffer->readen_nodes = Cbuffer->readen_nodes - 1;
    pthread_mutex_unlock(&mtx);
}

void write_at_cyclic_buffer(cyclic_buffer_node *node)
{
    pthread_mutex_lock(&mtx);
    while (Cbuffer->readen_nodes >= arg->bufferSize)
    {
        // printf(">> Found  Buffer  Full \n");
        pthread_cond_wait(&cond_nonfull, &mtx);
        if (finish_flag == true)
        {
            pthread_mutex_unlock(&mtx);
            pthread_cond_broadcast(&cond_nonempty);
            pthread_cond_broadcast(&cond_nonfull);
            pthread_exit(NULL);
        }
    }
    Cbuffer->end = (Cbuffer->end + 1) % (arg->bufferSize);
    assign_cyclic_buffer_node(&((Cbuffer->cbuf)[Cbuffer->end]), node->pathname, node->version, node->IP, node->port); // assign
    Cbuffer->readen_nodes = Cbuffer->readen_nodes + 1;

    pthread_mutex_unlock(&mtx);
}