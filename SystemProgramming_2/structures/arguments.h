#ifndef _ARGUMENTS_H_
#define _ARGUMNETS_H_

#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "./../string_library/string_library.h"
#include "./../error_handling/error_handling.h"

typedef struct Arguments
{
    int id_client;
    int buffer_size;
    my_string *common_dir;
    my_string *input_dir;
    my_string *mirror_dir;
    my_string *log_file;
} arguments;

#endif

void initialize_arguments(arguments **arg);
void checking_arguments(arguments *arg);
void destroy_arguments(arguments *arg);