#include "remove.h"

bool remove_directory(my_string *path)
{
    my_string *command, *full_command;
    initialize_string(&command);
    initialize_string(&full_command);
    straight_assign_string(command, "rm -rf ");
    full_command = concat_strings(command, path);

    system(full_command->str);

    destroy_string(command);
    destroy_string(full_command);
    
    return true;
}

bool remove_file(my_string *path, int clientID)
{
    my_string *client_ID, *extension, *slash;
    my_string *name_of_file1, *name_of_file2, *command, *full_command, *pathname;
    initialize_string(&client_ID);
    initialize_string(&extension);
    initialize_string(&slash);
    initialize_string(&name_of_file1);
    initialize_string(&name_of_file2);
    initialize_string(&command);
    initialize_string(&full_command);
    initialize_string(&pathname);

    assign_string(client_ID, itoa(clientID, length_of_int(clientID)));
    straight_assign_string(extension, ".id");
    straight_assign_string(slash, "/");
    straight_assign_string(command, "rm ");
    name_of_file1 = concat_strings(slash, client_ID);
    name_of_file2 = concat_strings(name_of_file1, extension);
    pathname = concat_strings(path, name_of_file2);
    full_command = concat_strings(command, pathname);

    system(full_command->str);

    destroy_string(client_ID);
    destroy_string(extension);
    destroy_string(slash);
    destroy_string(name_of_file1);
    destroy_string(name_of_file2);
    destroy_string(command);
    destroy_string(full_command);

    return true;
}