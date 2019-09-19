#include "server_client_request_routines.h"

bool serve_request(client_message **message, int *connected_sockets, int me, my_string *pathname, client_list **alive_clients, file_storage **saved_files)
{
    switch ((*message)->id)
    {
    case 1: // USER_ON
        if (!user_on_routine(alive_clients, *message))
        {
            error_handling(13);
            return false;
        }
        break;
    case 5: // GET_FILE_LIST
        if (!get_file_list_routine(connected_sockets, me, pathname, saved_files))
        {
            error_handling(18);
            return false;
        }
        break;
    case 6: // GET_FILE pathname,version
        if (!get_file_routine(connected_sockets, me, *message, pathname->str, saved_files))
        {
            error_handling(19);
            return false;
        }
        break;
    case 3: // LOG_OFF IP,port
        if (!log_off_routine(*message, alive_clients))
        {
            error_handling(20);
            return false;
        }
        break;
    default:
        return false;
        break;
    }
    return true;
}

// receive USER_ON
bool user_on_routine(client_list **alive_clients, client_message *message)
{
    insert_list(alive_clients, message->client_ip[0], message->client_port[0]);
    printf("now clients are: ");
    print_list(*alive_clients);
    return true;
}

// receive GET_FILE_LIST and send FILE_LIST
bool get_file_list_routine(int *connected_sockets, int me, my_string *pathname, file_storage **saved_files)
{
    char **all_files;
    char buffer[500];
    memset(&buffer[0], 0, sizeof(buffer)); // clean buffer

    int number_of_files = 0;
    get_number_of_files_of(pathname, &number_of_files);
    strcpy(buffer, "FILE_LIST ");                                               // FILE_LIST
    strcat(buffer, itoa(number_of_files, length_of_int(number_of_files))->str); // FILE_LIST N
    strcat(buffer, " ");

    all_files = (char **)malloc(number_of_files * sizeof(char *));
    if (all_files == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    get_files_of(pathname, all_files, &index);

    file_storage *current_file = *saved_files;
    for (int i = 0; i < number_of_files; i++)
    {
        strcat(buffer, all_files[i]);
        strcat(buffer, ",");
        while (current_file != NULL)
        {
            if (strcmp(all_files[i], current_file->path->str) == 0)
            {
                current_file = *saved_files;
                strcat(buffer, itoa(current_file->version, length_of_int(current_file->version))->str);
                strcat(buffer, ",");
                break;
            }
            current_file = current_file->next;
        }
    }

    buffer[strlen(buffer) - 1] = '\0';

    if (write(connected_sockets[me], buffer, strlen(buffer)) < 0)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }
    printf("                        send %s\n", buffer);

    return true;
}

// receive GET_FILE and send FINE_NOT_FOUND / FILE_UP_TO_DATE / FILE_SIZE version n byte0byte1..byten
bool get_file_routine(int *connected_sockets, int me, client_message *message, char *my_dir, file_storage **saved_files)
{
    int filesize;
    char *filecontent;
    char buffer[1000];
    memset(&buffer[0], 0, sizeof(buffer)); // clean buffer
    char path[strlen(message->pathnames) + 1];
    memset(&path[0], 0, sizeof(path));
    strcat(path, message->pathnames);

    if (existance_of_file(path)) // file exists
    {
        my_string *temp_path;
        initialize_string(&temp_path);
        straight_assign_string(temp_path, message->pathnames);
        file_storage *returned_saved_file = file_storage_search(saved_files, temp_path, message->version);
        if (returned_saved_file != NULL)
        {
            file_storage_assign(saved_files, temp_path, message->version);
            file_storage_print(*saved_files);
            strcpy(buffer, "FILE_SIZE ");
            strcat(buffer, itoa(returned_saved_file->version, length_of_int(returned_saved_file->version))->str);
            strcat(buffer, " ");
            filesize = size_of_file(path);
            strcat(buffer, itoa(filesize, length_of_int(filesize))->str);
            strcat(buffer, " ");
            filecontent = (char *)malloc(filesize);
            get_file_content(path, filecontent, filesize);
            strcat(buffer, filecontent);
            buffer[strlen(buffer)] = '\0';
        }
        destroy_string(temp_path);
    }
    else if (dir_exist(path)) // directory exists
    {
        strcpy(buffer, "FILE_SIZE ");
        strcat(buffer, itoa(message->version, length_of_int(message->version))->str);
        strcat(buffer, " ");
        strcat(buffer, "-2");
        strcat(buffer, " ");
        strcat(buffer, "-2");
        buffer[strlen(buffer) - 1] = '\0';
    }
    else // directory or file does not exist
    {
        strcpy(buffer, "FILE_NOT_FOUND");
        buffer[15] = '\0';
    }
    if (write(connected_sockets[me], buffer, strlen(buffer)) < 0)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }
    printf("                        send %s\n", buffer);

    return true;
}

bool log_off_routine(client_message *message, client_list **alive_clients)
{
    delete_list(alive_clients, message->client_ip[0], message->client_port[0]);
    printf("NOW CLIENTS ARE: ");
    print_list(*alive_clients);
    return true;
}