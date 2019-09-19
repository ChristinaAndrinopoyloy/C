#ifndef _REMOVE_DIRECTORY_H_
#define _REMOVE_DIRECTORY_H_

#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "./../error_handling/error_handling.h"
#include "./../string_library/string_library.h"

bool remove_directory(my_string *path);
bool remove_file(my_string *path, int clientID);

#endif