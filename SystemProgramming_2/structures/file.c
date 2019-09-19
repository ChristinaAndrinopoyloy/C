#include "file.h"

bool get_subdirs_and_files(my_string *pathname, list_node **subdirs, list_node **files)
{
    my_string *path, *slash, *temp;
    initialize_string(&path);
    initialize_string(&slash);
    initialize_string(&temp);
    straight_assign_string(slash, "/");
    struct dirent *dp;

    DIR *dir = opendir(pathname->str);
    if (!dir)
    {
        perror("opendir");
        return false;
    }

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            if (dp->d_type == DT_REG)
            {
                straight_assign_string(path, pathname->str + 2);
                path = concat_strings(path, slash);
                straight_assign_string(temp, dp->d_name);
                path = concat_strings(path, temp);
                insert_list(files, path);
            }
            if (dp->d_type == DT_DIR)
            {
                assign_string(path, pathname);
                path = concat_strings(path, slash);
                straight_assign_string(temp, dp->d_name);
                path = concat_strings(path, temp);
                insert_list(subdirs, path);
                if (!(get_subdirs_and_files(path, subdirs, files)))
                {
                    error_handling(16);
                    return false;
                }
            }
        }
    }
    closedir(dir);
    destroy_string(path);
    destroy_string(slash);
    destroy_string(temp);

    return true;
}

bool existance_of_file(char *pathname)
{
    struct stat buffer;
    return (stat(pathname, &buffer) == 0);
}

bool check_for_deleted_files(list_node **common_history, my_string *directory, my_string **deleted_file)
{
    bool returned_value;
    FILE *fp;
    my_string *name_of_file;
    my_string *pathname;
    my_string *slash;
    initialize_string(&name_of_file);
    initialize_string(&pathname);
    initialize_string(&slash);
    straight_assign_string(slash, "/");
    list_node *current = *common_history;
    list_node *previous = current;
    do
    {
        assign_string(name_of_file, current->data);
        assign_string(*deleted_file, current->data);
        assign_string(pathname, directory);
        pathname = concat_strings(pathname, slash);
        pathname = concat_strings(pathname, name_of_file);
        if (existance_of_file(pathname->str))
        {
            returned_value = false;
        }
        else
        {
            returned_value = true;
            break;
        }
        previous = current;
        current = current->next;
    } while (current != NULL);

    if (returned_value == true)
    {
        printf("\n I will remove %s\n", (*deleted_file)->str);
        delete_list(common_history, *deleted_file);
    }

    destroy_string(slash);
    destroy_string(name_of_file);
    destroy_string(pathname);
    return returned_value;
}

bool set_file(char *name_of_file, char *file_content, unsigned int legth_of_file, my_string *dest_dir, int id)
{
    char *name = (char *)malloc((strlen(name_of_file) + length_of_int(id) + 3 + strlen(dest_dir->str)) * sizeof(char));
    if (name == NULL)
    {
        perror("malloc");
        return false;
    }
    memset(name, '\0', strlen(name));
    name = strcat(name, dest_dir->str);
    name = strcat(name, "/");
    name = strcat(name, itoa(id, length_of_int(id))->str);
    name = strcat(name, "/");
    name = strcat(name, name_of_file);
    int filedes;
    if ((filedes = open(name, O_CREAT | O_TRUNC | O_RDWR, S_IRWXU | S_IRGRP | S_IROTH)) == -1)
    {
        perror("open");
        return false;
    }
    if ((write(filedes, file_content, legth_of_file)) == -1)
    {
        perror("write");
        return false;
    }
    if ((close(filedes)) == -1)
    {
        perror("close");
        return false;
    }
    return true;
}

bool set_directory(char *pathname, my_string *dest_dir, int id)
{
    char *command = (char *)malloc((strlen("mkdir -p ") + strlen(pathname) + strlen(dest_dir->str) + length_of_int(id) + 2) * sizeof(char));
    if (command == NULL)
    {
        perror("malloc");
        return false;
    }
    command = strcat(command, "mkdir -p ");
    command = strcat(command, dest_dir->str);
    command = strcat(command, "/");
    command = strcat(command, itoa(id, length_of_int(id))->str);
    command = strcat(command, pathname + 1);
    system(command);
    return true;
}

my_string *file_name_preprocess(my_string *pathname)
{
    char *token = NULL;
    my_string *name_of_file;
    initialize_string(&name_of_file);

    token = strtok(pathname->str, "/");
    while (token != NULL)
    {
        straight_assign_string(name_of_file, token);
        token = strtok(NULL, "/");
    }
    return name_of_file;
}

// return informations about file
bool initialize_file(my_string *pathname, my_file **file_infos, bool file_flag)
{
    (*file_infos) = (my_file *)malloc(sizeof(my_file));
    if ((*file_infos) == NULL)
    {
        perror("malloc");
        return false;
    }

    if (file_flag == true)
    {
        FILE *file = fopen(pathname->str, "rb");
        if (file)
        {
            fseek(file, 0, SEEK_END);
            (*file_infos)->length_of_file = (unsigned int)ftell(file);
            fseek(file, 0, SEEK_SET);
            (*file_infos)->buffer = (char *)malloc((*file_infos)->length_of_file * sizeof(char));
            if ((*file_infos)->buffer)
            {
                fread((*file_infos)->buffer, 1, (*file_infos)->length_of_file, file);
            }
            else
            {
                perror("malloc");
                return false;
            }
            fclose(file);
        }
        else
        {
            perror("file open");
            return false;
        }

        initialize_string(&((*file_infos)->name_of_file));
        assign_string((*file_infos)->name_of_file, pathname);
        (*file_infos)->length_of_name = (short int)strlen((*file_infos)->name_of_file->str) + 1;
    }
    else
    {
        initialize_string(&((*file_infos)->name_of_file));
        assign_string((*file_infos)->name_of_file, pathname);
        (*file_infos)->length_of_name = (short int)strlen((*file_infos)->name_of_file->str) + 1;
        (*file_infos)->length_of_file = 0;
        (*file_infos)->buffer = NULL;
    }
    return true;
}

// check if file extension == wanted_extension
bool check_file_extension(struct dirent *direntp, char *wanted_extension)
{
    char *extension = strrchr(direntp->d_name, '.');
    if (!extension)
    {
        return false;
    }
    else
    {
        if (strcmp(wanted_extension, extension) == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}