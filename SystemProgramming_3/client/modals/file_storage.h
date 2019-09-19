#ifndef _FILE_STORAGE_H_
#define _FILE_STORAGE_H_

#include <stdbool.h>
#include "./../../date_time_library/date_time_library.h"
#include "./../../string_library/string_library.h"

typedef struct File_Storage
{
    my_string *path;
    int version;
    struct File_Storage *next;
} file_storage;

bool file_storage_check_empty(file_storage *head);
int file_storage_size(file_storage *head);
bool file_storage_assign(file_storage **head, my_string *pathname, int version);
file_storage *file_storage_search(file_storage **head, my_string *pathname, int version);
void file_storage_remove(file_storage **head, my_string *pathname, int version);
void file_storage_print(file_storage *head);
void file_storage_free(file_storage *head);

#endif