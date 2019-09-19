#include "arguments.h"

void initialize_arguments(arguments **arg)
{
    *arg = (arguments *)malloc(sizeof(arguments));
    if (*arg == NULL)
    {
        perror("malloc");
        exit(1);
    }
    (*arg)->buffer_size = -1;
    initialize_string(&((*arg)->common_dir));
    initialize_string(&((*arg)->input_dir));
    initialize_string(&((*arg)->mirror_dir));
    initialize_string(&((*arg)->log_file));
}

void checking_arguments(arguments *arg)
{
    // checking for input directory (if does not exist => exit)
    DIR *directory = opendir(arg->input_dir->str);
    if (directory) // directory exists
    {
        closedir(directory);
    }
    else if (ENOENT == errno) // directory does not exist
    {
        error_handling(3);
    }
    else // opendir failed
    {
        perror("opendir");
        exit(1);
    }

    // checking for mirror directory (if exists => exit, if does not exist => makedir)
    directory = opendir(arg->mirror_dir->str);
    if (directory) // directory exists
    {
        closedir(directory);
        error_handling(4);
    }
    else if (ENOENT == errno) // directory does not exist
    {
        if (mkdir(arg->mirror_dir->str, S_IRWXU) == -1)
        {
            perror("mkdir");
            exit(1);
        }
    }
    else // opendir failed
    {
        perror("opendir");
        exit(1);
    }

    // checking for common directory (if does not exist => makedir)
    directory = opendir(arg->common_dir->str);
    if (directory) // directory exists
    {
        closedir(directory);
    }
    else if (ENOENT == errno) // directory does not exist
    {
        if (mkdir(arg->common_dir->str, S_IRWXU) == -1)
        {
            perror("mkdir");
            exit(1);
        }
    }
    else // opendir failed
    {
        perror("opendir");
        exit(1);
    }

    // checking buffer size
    if (arg->buffer_size <= 0)
    {
        error_handling(5);
    }
}

void destroy_arguments(arguments *arg)
{
    destroy_string(arg->common_dir);
    destroy_string(arg->input_dir);
    destroy_string(arg->mirror_dir);
    destroy_string(arg->log_file);
    free(arg);
}