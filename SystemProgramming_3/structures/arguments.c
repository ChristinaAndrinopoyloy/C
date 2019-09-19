#include "arguments.h"

void initialize_server_arguments(server_arguments **arg)
{
    *arg = (server_arguments *)malloc(sizeof(server_arguments));
    if (*arg == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    (*arg)->portNum = -1;
}

void checking_server_arguments(server_arguments *arg)
{
    if (arg->portNum < 1024)
    {
        error_handling(3);
    }
}

void destroy_server_arguments(server_arguments *arg)
{
    free(arg);
}

void initialize_client_arguments(client_arguments **arg)
{
    *arg = (client_arguments *)malloc(sizeof(client_arguments));
    if (*arg == NULL)
    {
        perror("malloc");
        exit(1);
    }
    (*arg)->portNum = -1;
    (*arg)->workerThreads = -1;
    (*arg)->bufferSize = -1;
    (*arg)->serverPort = -1;
    initialize_string(&((*arg)->dirName));
    initialize_string(&((*arg)->serverIP));
}

void checking_client_arguments(client_arguments *arg)
{
    // checking for directory (if does not exist => exit)
    DIR *directory = opendir(arg->dirName->str);
    if (directory) // directory exists
    {
        closedir(directory);
    }
    else if (ENOENT == errno) // directory does not exist
    {
        error_handling(4);
    }
    else // opendir failed
    {
        perror("opendir");
        exit(1);
    }

    // checking port
    if (arg->portNum < 1024)
    {
        error_handling(5);
    }

    // checking number of threads
    if (arg->workerThreads <= 0)
    {
        error_handling(6);
    }

    // checking buffer size
    if (arg->bufferSize <= 0)
    {
        error_handling(7);
    }

    // checking server port
    if (arg->serverPort < 0)
    {
        error_handling(8);
    }
}

void destroy_client_arguments(client_arguments *arg)
{
    destroy_string(arg->dirName);
    destroy_string(arg->serverIP);
    free(arg);
}
