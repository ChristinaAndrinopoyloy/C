#include "update_common_directory.h"

bool create_file(my_string *path_name, int *filedes)
{
    struct stat stat_buffer;
    if (stat(path_name->str, &stat_buffer) == 0)
    {
        error_handling(9);
    }

    mode_t mode = S_IRWXU | S_IRGRP | S_IROTH;

    if (((*filedes) = open(path_name->str, O_CREAT | O_RDWR, mode)) == -1)
    {
        perror("open");
        return false;
    }
    else
    {
        return true;
    }
}

bool write_file(int filedes, my_string *buffer)
{
    if (write(filedes, buffer->str, sizeof(buffer->str)) == -1)
    {
        perror("write");
        return false;
    }
    return true;
}

bool update_common_directory(my_string *common_directory, int processID, int clientID)
{
    int filedes;
    my_string *pID, *client_ID, *extension, *slash;
    my_string *name_of_file1, *name_of_file2, *path_name;
    initialize_string(&pID);
    initialize_string(&client_ID);
    initialize_string(&extension);
    initialize_string(&slash);
    initialize_string(&name_of_file1);
    initialize_string(&name_of_file2);
    initialize_string(&path_name);
    int length = length_of_int(processID);
    assign_string(pID, itoa(processID, length));
    length = length_of_int(clientID);
    assign_string(client_ID, itoa(clientID, length));
    straight_assign_string(extension, ".id");
    straight_assign_string(slash, "/");
    name_of_file1 = concat_strings(slash, client_ID);
    name_of_file2 = concat_strings(name_of_file1, extension);
    path_name = concat_strings(common_directory, name_of_file2);

    if (create_file(path_name, &filedes))
    {
        if (!(write_file(filedes, pID)))
        {
            error_handling(8);
            return false;
        }
    }
    else
    {
        error_handling(6);
        return false;
    }

    close(filedes);
    destroy_string(pID);
    destroy_string(client_ID);
    destroy_string(extension);
    destroy_string(slash);
    destroy_string(name_of_file1);
    destroy_string(name_of_file2);
    destroy_string(path_name);

    return true;
}