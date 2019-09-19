#ifndef _FILE_H_
#define _FILE_H_

#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "./../../string_library/string_library.h"
#include "./../../error_handling/error_handling.h"

bool get_number_of_files_of(my_string *pathname, int *counter);
bool get_files_of(my_string *pathname, char **files, int *index);
bool dir_exist(char *dir_name);
bool existance_of_file(char *pathname);
int size_of_file(char *path);
void get_file_content(char *path, char *content, int size);
void dirfile_creation(char *pathname, char *content, bool dir_flag);

#endif