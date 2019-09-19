#ifndef _ARGUMENTS_H_
#define _ARGUMNETS_H_

#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "./../string_library/string_library.h"
#include "./../error_handling/error_handling.h"

typedef struct Server_Arguments
{
    int portNum;
} server_arguments;

#endif

typedef struct Client_Arguments
{
    my_string *dirName;
    int portNum;
    int workerThreads;
    int bufferSize;
    int serverPort;
    my_string *serverIP;
} client_arguments;


void initialize_server_arguments(server_arguments **arg);
void checking_server_arguments(server_arguments *arg);
void destroy_server_arguments(server_arguments *arg);

void initialize_client_arguments(client_arguments **arg);
void checking_client_arguments(client_arguments *arg);
void destroy_client_arguments(client_arguments *arg);