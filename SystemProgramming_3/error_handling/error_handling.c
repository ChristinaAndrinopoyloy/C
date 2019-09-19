#include "error_handling.h"

void error_handling(int error_no)
{
    switch (error_no)
    {
    case 1:
        printf("Error: app command: argument without value\n"); // getopt from main
        exit(EXIT_FAILURE);
        break;
    case 2:
        printf("Error: app command: unknown argument\n"); // getopt from main
        exit(EXIT_FAILURE);
        break;
    case 3:
        printf("Error: server command: invalid number of port\n"); // from checking_server_arguments
        exit(EXIT_FAILURE);
        break;
    case 4:
        printf("Error: client command: directory does not exists\n"); // from checking_client_arguments
        exit(EXIT_FAILURE);
        break;
    case 5:
        printf("Error: client command: invalid portNum\n"); // from checking_client_arguments
        exit(EXIT_FAILURE);
        break;
    case 6:
        printf("Error: client command: invalid number of workerThreads\n"); // from checking_client_arguments
        exit(EXIT_FAILURE);
        break;
    case 7:
        printf("Error: client command: invalid bufferSize\n"); // from checking_client_arguments
        exit(EXIT_FAILURE);
        break;
    case 8:
        printf("Error: client command: invalid serverPort\n"); // from checking_client_arguments
        exit(EXIT_FAILURE);
        break;
    case 9:
        printf("Error: server: invalid message from client\n"); // from assign_server_message
        exit(EXIT_FAILURE);
        break;
    case 10:
        printf("Error: server: problem with LOG_ON\n"); // from serve_request
        break;
    case 11:
        printf("Error: server: problem with GET_CLIENTS\n"); // from serve_request
        break;
    case 12:
        printf("Error: server: problem with LOG_OFF\n"); // from serve_request
        break;
    case 13:
        printf("Error: server: cannot serve this request\n"); // from serve_request
        break;
    case 14:
        printf("Error: Not in presentation format\n");
        exit(EXIT_FAILURE);
        break;
    case 15:
        printf("Error: client: cyclic buffer is full\n");
        break;
    case 16:
        printf("Error: client: cyclic buffer is empty\n");
        break;
    case 17:
        printf("Error: client: cannot serve this request\n"); // from serve_request (client)
        break;
    case 18:
        printf("Error: client: problem with GET_FILE_LIST\n"); // from serve_request (client)
        break;
    case 19:
        printf("Error: client: problem with GET_FILE\n"); // from serve_request (client)
        break;
    case 20:
        printf("Error: client: problem with LOG_OFF\n"); // from serve_request (client)
        break;
    case 21:
        printf("Error: file\n"); // from file.c
        break;
    case 22:
        printf("Error: client: problem with FILE_LIST\n"); // from file.c
        break;
    case 23:
        printf("Error: client: problem with USER_ON\n"); // from file.c
        break;
    default:
        break;
    }
}