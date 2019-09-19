#include "client_helpers.h"

void get_address(my_string **ip)
{
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sa;
     char *addr;
	//memset(&addr[0], 0, sizeof(addr));
     getifaddrs(&ifap);
     for (ifa = ifap; ifa; ifa = ifa->ifa_next)
     {
         if (ifa->ifa_addr->sa_family == AF_INET)
         {
             sa = (struct sockaddr_in *)ifa->ifa_addr;
	//	strcpy(addr, inet_ntoa(sa->sin_addr));         
    addr = inet_ntoa(sa->sin_addr);
      //       if (strcmp(ifa->ifa_name, "wlo1") == 0)
        //     {
                 straight_assign_string((*ip), addr);
          //   }
         }
     }
     freeifaddrs(ifap);

    return;
}

void read_from_server(char *message_buffer, int socket, client_message **message)
{
    int ret;
    char buffer[MESSAGE_BUFFER];
    bool received_all = false;
    memset(&message_buffer[0], 0, sizeof(message_buffer));

    while (received_all == false)
    {
        if ((ret = read(socket, buffer, MESSAGE_BUFFER)) <= 0)
        {
            continue;
        }
        else // read
        {
            if (buffer[ret - 1] == '\0') // message finished
            {
                received_all = true;
                strcat(message_buffer, buffer);
                printf("\nReceived |%s|\n", message_buffer);
                prepare_client_message(message, message_buffer); // initialization of message
                assign_client_message(*message, message_buffer);
            }
            else
            {
                buffer[ret] = '\0';
                strcat(message_buffer, buffer);
            }
        }
    }
    return;
}

void send_log_on(my_string *ip, int port, int socket)
{
    sleep(3);
    char log_on[50];
    memset(&log_on[0], 0, sizeof(log_on)); // clean log_on buffer

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

    sprintf(log_on, "LOG_ON %lu,%lu", htonl((unsigned int)(temp.sin_addr.s_addr)), htonl(port)); // LOG_ON IP,port

    int i;
    for (i = 0; log_on[i] != '\0'; i++)
    {
        if (write(socket, log_on + i, 1) < 0) // write char by char
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }
    if (write(socket, log_on + i, 1) < 0)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }
    printf("                        send %s\n", log_on);
    return;
}

void send_get_clients(int socket, client_list **alive_clients)
{
    sleep(5);
    int index = 0;
    int counter = 0; // counts the ' '
    char curr[1];
    bool clients_flag = false;
    bool received_all = false;
    int ret;
    char get_clients[13];
    memset(&get_clients[0], 0, sizeof(get_clients));
    strcpy(get_clients, "GET_CLIENTS");
    get_clients[strlen(get_clients)] = '\0';
    client_message *message;

    if (write(socket, get_clients, 12) < 0)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }
    printf("                        send %s\n", get_clients);

    char returned_clients[200];
    read_from_server(returned_clients, socket, &message);

    for (int j = 0; j < message->counter; j++)
    {
        insert_list(alive_clients, message->client_ip[j], message->client_port[j]);
    }
    print_list(*alive_clients);

    return;
}

void send_log_off(int socket)
{
    char log_off[8];
    memset(&log_off[0], 0, sizeof(log_off));
    strcpy(log_off, "LOG_OFF");
    log_off[7] = '\0';
    int i;

    if (write(socket, log_off, 8) < 0)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }

    printf("                        send : %s\n", log_off);
    return;
}

cyclic_buffer_node *send_get_file_list(char *ip, int port, int *size, file_storage **saved_files)
{
    sleep(5);
    /////////////////////////////////////////prepare/////////////////////////////////////////
    char get_file_list[14];
    memset(&get_file_list[0], 0, sizeof(get_file_list));
    strcpy(get_file_list, "GET_FILE_LIST");
    get_file_list[13] = '\0';
    /////////////////////////////////////////write at socket/////////////////////////////////////////
    int sock;
    struct hostent *rem;
    struct sockaddr_in my_server;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) // create socket
    {
        exit(EXIT_FAILURE);
    }

    /* Find server address */
    // pthread_mutex_lock(&lock);
    // while ((rem = gethostbyname(ip)) == NULL)
    // {
    //     // pthread_mutex_unlock(&lock);
    //     printf(")");
    //     usleep(200);
    //     // exit(1);
    // }

    my_server.sin_family = AF_INET; // Internet domain
    // memcpy(&my_server.sin_addr.s_addr, rem->h_addr, rem->h_length);

    my_server.sin_addr.s_addr = inet_addr(ip);
    my_server.sin_port = htons(port);
    while (connect(sock, (struct sockaddr *)&my_server, sizeof(my_server)) < 0)
        ; // innitiate connection
    printf("                        send to %s %d : %s\n", ip, port, get_file_list);
    if (write(sock, get_file_list, 14) < 0)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }

    int ret;
    char buffer[500];
    memset(&buffer[0], 0, sizeof(buffer));
    char message_buffer[500];
    memset(&message_buffer[0], 0, sizeof(message_buffer));
    while (true)
    {
        if ((ret = read(sock, buffer, 500)) <= 0) // connection lost
        {
            continue;
        }
        else // read
        {
            if (buffer[ret] == '\0') // message finished
            {
                strcat(message_buffer, buffer);
                printf("\nReceived: %s \n", message_buffer);
                break;
            }
            else
            {
                buffer[ret] = '\0';
                strcat(message_buffer, buffer);
            }
        }
    }
    close(sock);

    int digits = 0; // for strtok
    int index = 0;
    char curr[1];
    char number[3];
    curr[0] = message_buffer[10];
    memset(&number[0], 0, sizeof(number));

    while ((digits < strlen(message_buffer) - 10) && (curr[0] != ' '))
    {
        digits++;
        strcat(number, curr);
        curr[0] = message_buffer[10 + digits];
    }

    int n = atoi(number);
    *size = n;
    cyclic_buffer_node *returned_nodes;
    returned_nodes = (cyclic_buffer_node *)malloc(n * sizeof(cyclic_buffer_node));
    int i = 0;
    char pathname[60];
    memset(&pathname[0], 0, sizeof(pathname));
    char version[10];
    memset(&version[0], 0, sizeof(version));
    bool pathname_flag = true;
    file_storage *current_file = *saved_files;

    char *token = strtok(message_buffer + (10 + digits + 1), ",");
    while (token != NULL)
    {
        if (pathname_flag)
        {
            strcpy(pathname, token);
            strcpy(returned_nodes[i].IP, ip);
            returned_nodes[i].port = port;
            strcpy(returned_nodes[i].pathname, pathname);
            pathname_flag = false;
        }
        else
        {
            strcpy(version, token);
            returned_nodes[i].version = atoi(version);
            i++;
            pathname_flag = true;
        }
        token = strtok(NULL, ",");
    }
    sleep(3);
    return returned_nodes;
    /////////////////////////////////////////////////////////////////////////////////////////////////
}

void send_get_file(char *ip, int port, int *size, char *pathname, int version, file_storage **saved_files, char *myip, int myport)
{
    sleep(5);
    /////////////////////////////////////////prepare/////////////////////////////////////////
    char get_file[10 + strlen(pathname) + 1 + length_of_int(version)];
    memset(&get_file[0], 0, sizeof(get_file));
    strcpy(get_file, "GET_FILE ");
    strcat(get_file, pathname);
    strcat(get_file, ",");
    strcat(get_file, itoa(version, length_of_int(version))->str);
    /////////////////////////////////////////write at socket//////////////////////////////////
    int sock;
    struct hostent *rem;
    struct sockaddr_in my_server;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) // create socket
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* Find server address */
    // while ((rem = gethostbyname(ip)) == NULL)
    // ;

    my_server.sin_family = AF_INET; // Internet domain
    my_server.sin_addr.s_addr = inet_addr(ip);
    // memcpy(&my_server.sin_addr, rem->h_addr, rem->h_length);
    my_server.sin_port = htons(port);

    while (connect(sock, (struct sockaddr *)&my_server, sizeof(my_server)) < 0)
        ; // innitiate connection

    printf("                        send to %s %d : %s\n", ip, port, get_file);
    if (write(sock, get_file, 10 + strlen(pathname) + 1 + length_of_int(version)) < 0)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }

    ////////////////////////////////////////receive from socket////////////////////////////////////////

    int ret;
    char buffer[1000];
    memset(&buffer[0], 0, sizeof(buffer));
    char message_buffer[1000];
    memset(&message_buffer[0], 0, sizeof(message_buffer));
    while (true)
    {
        if ((ret = read(sock, buffer, 1000)) <= 0)
        {
            continue;
        }
        else // read
        {
            if (buffer[ret] == '\0' || buffer[ret - 1] == '\0') // message finished
            {
                strcat(message_buffer, buffer);
                printf(">>Received: %s \n", message_buffer);
                break;
            }
            else
            {
                buffer[ret] = '\0';
                strcat(message_buffer, buffer);
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    int fileversion, filesize;
    char filecontent[1000];
    int spaces = 0;
    char *token = strtok(message_buffer, " ");
    while (token != NULL)
    {
        switch (spaces)
        {
        case 0:
            spaces++;
            break;
        case 1:
            spaces++;
            fileversion = atoi(token);
            break;
        case 2:
            spaces++;
            filesize = atoi(token);
            break;
        case 3:
            spaces++;
            strcpy(filecontent, token);
            break;
        default:
            strcat(filecontent, " ");
            strcat(filecontent, token);
            break;
        }
        token = strtok(NULL, " ");
    }

    printf("\n\nFILE: %s\n", pathname);
    printf("VERSION: %d\n", fileversion);
    printf("SIZE: %d\n", filesize);
    printf("CONTENT: %s\n", filecontent);
    char file_pathname[strlen(myip) + length_of_int(myport) + strlen(pathname) + 3];
    memset(&file_pathname[0], 0, sizeof(file_pathname));
    strcpy(file_pathname, myip);
    strcat(file_pathname, "_");
    strcat(file_pathname, itoa(myport, length_of_int(myport))->str);
    strcat(file_pathname, "/");
    strcat(file_pathname, pathname);

    my_string *ip2;
    initialize_string(&ip2);
    straight_assign_string(ip2, myip);
    my_string *pathname2;
    initialize_string(&pathname2);
    straight_assign_string(pathname2, pathname);
    if (fileversion == -2 && filesize == -2) // it's a directory not file
    {
        if (file_storage_assign(saved_files, pathname2, -2))
        {
            dirfile_creation(file_pathname, filecontent, true); // save it
        }
    }

    file_storage_assign(saved_files, pathname2, fileversion); // new file at client system
    dirfile_creation(file_pathname, filecontent, false);      // save it

    destroy_string(ip2);
    destroy_string(pathname2);
    sleep(3);
    close(sock);
    return;
    /////////////////////////////////////////////////////////////////////////////////////////////////
}