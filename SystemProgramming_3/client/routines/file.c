#include "file.h"

bool get_number_of_files_of(my_string *pathname, int *counter)
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
                (*counter)++;
            }
            if (dp->d_type == DT_DIR)
            {
                assign_string(path, pathname);
                path = concat_strings(path, slash);
                straight_assign_string(temp, dp->d_name);
                path = concat_strings(path, temp);
                if (!(get_number_of_files_of(path, counter)))
                {
                    error_handling(20);
                    return false;
                }
            }
        }
    }
    closedir(dir);
    return true;
}

bool get_files_of(my_string *pathname, char **files, int *index)
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
                straight_assign_string(path, pathname->str);
                path = concat_strings(path, slash);
                straight_assign_string(temp, dp->d_name);
                path = concat_strings(path, temp);
                files[*index] = (char *)malloc(strlen(path->str));
                if (files[*index] == NULL)
                {
                    perror("malloc");
                    exit(EXIT_FAILURE);
                }
                strcpy(files[*index], path->str);
                (*index)++;
            }
            if (dp->d_type == DT_DIR)
            {
                assign_string(path, pathname);
                path = concat_strings(path, slash);
                straight_assign_string(temp, dp->d_name);
                path = concat_strings(path, temp);
                if (!(get_files_of(path, files, index)))
                {
                    error_handling(20);
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

bool dir_exist(char *dir_name)
{
    struct stat s;
    int ret = stat(dir_name, &s);
    if (ret == -1)
    {
        if (ENOENT == errno) // directory (path) does not exist, so mkdir
        {
            return false;
        }
    }
    return true;
}

bool existance_of_file(char *pathname)
{
    struct stat buffer;
    return (stat(pathname, &buffer) == 0);
}

int size_of_file(char *path)
{
    struct stat temp;
    int status;

    status = stat(path, &temp);
    if (status == 0)
    {
        return temp.st_size;
    }
}

void get_file_content(char *path, char *content, int size)
{
    memset(&content[0], 0, sizeof(content));
    FILE *file = fopen(path, "rb");

    if (file)
    {
        fread(content, 1, size, file);
        fclose(file);
    }
}

void dirfile_creation(char *pathname, char *content, bool dir_flag)
{
    char path[200];
    char previous[200];
    char full_path[200];
    memset(&path[0], 0, sizeof(path));
    memset(&previous[0], 0, sizeof(previous));
    memset(&full_path[0], 0, sizeof(full_path));
    strcpy(full_path, pathname);
    char *token = strtok(pathname, "/");
    while (token != NULL)
    {
        strcpy(previous, path);
        strcat(path, token);
        strcat(path, "/");
        token = strtok(NULL, "/");
    }

    char command[200];
    struct stat s;
    int ret = stat(previous, &s);
    if (ret == -1)
    {
        if (ENOENT == errno) // directory (path) does not exist, so mkdir
        {
            memset(&command[0], 0, sizeof(command));
            strcpy(command, "mkdir -p ");
            strcat(command, previous);
            system(command);
        }
        else
        {
            perror("stat");
            exit(EXIT_FAILURE);
        }
    }

    if (dir_flag == true)
    {
        return;
    }

    memset(&command[0], 0, sizeof(command));
    strcpy(command, "touch ");
    strcat(command, pathname);
    system(command);

    FILE *file;
    file = fopen(full_path, "w");
    if (file != NULL)
    {
        fputs(content, file);
        fclose(file);
    }
    else
    {
        printf("deeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee THA FTIAKSO\n");
    }
}