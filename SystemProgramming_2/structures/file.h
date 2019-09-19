#ifndef _FILE_H_
#define _FILE_H_

#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "./../error_handling/error_handling.h"
#include "./../data_structures/linked_list.h"
#include "./../string_library/string_library.h"

typedef struct File
{
    my_string *name_of_file;
    short int length_of_name;
    unsigned int length_of_file;
    char *buffer;
} my_file;

bool get_subdirs_and_files(my_string *pathname, list_node **subdirs, list_node **files);
bool existance_of_file(char *pathname);
bool check_for_deleted_files(list_node **common_history, my_string *directory, my_string **deleted_file);
bool set_file(char *name_of_file, char *file_content, unsigned int legth_of_file, my_string *dest_dir, int id);
bool set_directory(char *pathname, my_string *dest_dir, int id);
my_string *file_name_preprocess(my_string *pathname);
bool initialize_file(my_string *pathname, my_file **file_infos, bool file_flag);
bool check_file_extension(struct dirent *direntp, char *wanted_extension);

#endif