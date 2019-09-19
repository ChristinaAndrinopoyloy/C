#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <poll.h>
#include "./../string_library/string_library.h"
#include "./../error_handling/error_handling.h"
#include "./../data_structures/linked_list.h"
#include "./../structures/structures.h"
#include "./update_common_directory.h"

#define NUMBER_OF_CHILDREN 2

typedef struct Communication_Handler_Struct
{
    int sender;
    int receiver;
    int parent;
    int number_of_failures;
    // arguments for born_sender_receiver
    arguments *arg;
    int otherID;
    list_node *subdirs;
    list_node *files;
} communication_handler_struct;

typedef struct Communication_Statistics
{
    int number_of_clients;
    int min_id;
    int max_id;
    int number_of_dead_clients;
} communication_statistics;

extern communication_statistics statistics;
FILE *logfile;
int logfileid;

bool check_common_dir(arguments *arg, list_node **common_history, list_node *subdirs, list_node *files);
void born_remover_from_mirror(my_string *deleted_file, my_string *mirror_dir);
void communication_handler(int signo, siginfo_t *siginfo, void *context);
void born_sender_receiver(arguments *arg, int otherID, list_node *subdirs, list_node *files);
void sender_process(my_string *source, my_string *dest, my_string *to, my_string *slash, my_string *extension, my_string *temp, my_string *fifo_name,
                    int otherID,
                    arguments *arg,
                    list_node *subdirs, list_node *files);
void write_subdirs(my_string *source, my_string *dest, my_string *to, my_string *slash, my_string *extension, my_string *temp, my_string *fifo_name,
                   int otherID, int filedes,
                   arguments *arg,
                   list_node *subdirs);
void write_files(my_string *source, my_string *dest, my_string *to, my_string *slash, my_string *extension, my_string *temp, my_string *fifo_name,
                 int otherID, int filedes,
                 arguments *arg,
                 list_node *files);
void alarming(int signum);
void receiver_process(my_string *source, my_string *dest, my_string *to, my_string *slash, my_string *extension, my_string *temp, my_string *fifo_name,
                      int otherID,
                      arguments *arg);
bool communication_preprocess(my_string *input_dir, list_node **subdirs, list_node **files);
bool communication(arguments *arg);

#endif