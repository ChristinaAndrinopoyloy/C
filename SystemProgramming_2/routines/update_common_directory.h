#ifndef _UPDATE_COMMON_DIRECTORY_H_
#define _UPDATE_COMMON_DIRECTORY_H_

#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include "./../string_library/string_library.h"
#include "./../error_handling/error_handling.h"

bool create_file(my_string *path_name, int *filedes);
bool write_file(int filedes, my_string *buffer);
bool update_common_directory(my_string *common_directory, int processID, int clientID);

#endif