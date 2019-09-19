#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include "./error_handling/error_handling.h"
#include "./structures/structures.h"
#include "./string_library/string_library.h"
#include "./routines/update_common_directory.h"
#include "./routines/remove.h"
#include "./routines/communication.h"

arguments *arg = NULL;
extern communication_statistics statistics;
static void handlerr(int signo, siginfo_t *siginfo, void *context)
{
    fprintf(logfile, "%s\n", "allclients ");
    fprintf(logfile, "%d\n", statistics.number_of_clients);
    fprintf(logfile, "%s\n", "max ");
    fprintf(logfile, "%d\n", statistics.max_id);
    fprintf(logfile, "%s\n", "min ");
    fprintf(logfile, "%d\n", statistics.min_id);
    fprintf(logfile, "%s\n", "dead ");
    fprintf(logfile, "%d\n", statistics.number_of_dead_clients);
    fclose(logfile);

    if (!(remove_directory(arg->mirror_dir)))
    {
        error_handling(10);
    }
    if (!(remove_file(arg->common_dir, arg->id_client)))
    {
        error_handling(11);
    }
    destroy_arguments(arg);
    printf("\nend of app\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    int opt;
    initialize_arguments(&arg);

    struct sigaction signalAction;
    signalAction.sa_sigaction = &handlerr;
    signalAction.sa_flags = SA_SIGINFO;
    sigemptyset(&signalAction.sa_mask);
    sigaddset(&signalAction.sa_mask, SIGINT);
    sigaddset(&signalAction.sa_mask, SIGQUIT);
    sigaction(SIGINT, &signalAction, NULL);
    sigaction(SIGQUIT, &signalAction, NULL);

    while ((opt = getopt(argc, argv, ":n:c:i:m:b:l:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            arg->id_client = atoi(optarg);
            printf("id is: %d\n", arg->id_client);
            break;
        case 'c':
            straight_assign_string(arg->common_dir, optarg);
            printf("common dir is: %s\n", arg->common_dir->str);
            break;
        case 'i':
            straight_assign_string(arg->input_dir, optarg);
            printf("input dir is: %s\n", arg->input_dir->str);
            break;
        case 'm':
            straight_assign_string(arg->mirror_dir, optarg);
            printf("mirror dir is: %s\n", arg->mirror_dir->str);
            break;
        case 'b':
            arg->buffer_size = atoi(optarg);
            printf("buffer size is: %d\n", arg->buffer_size);
            break;
        case 'l':
            straight_assign_string(arg->log_file, optarg);
            printf("log file is: %s\n", arg->log_file->str);
            break;
        case ':':
            error_handling(1);
            return 1;
            break;
        case '?':
            error_handling(2);
            return 1;
            break;
        }
    }
    printf("\n\n");
    checking_arguments(arg);

    if (!(update_common_directory(arg->common_dir, (int)getpid(), arg->id_client)))
    {
        error_handling(7);
    }

    if (!(communication(arg)))
    {
        error_handling(13);
    }

    return 0;
}
