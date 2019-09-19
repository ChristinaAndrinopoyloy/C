#include "communication.h"

communication_handler_struct comm_infos;
communication_statistics statistics;

list_node *common_history = NULL;

// check if a new file .id is on common
// based on lecture
bool check_common_dir(arguments *arg, list_node **common_history, list_node *subdirs, list_node *files)
{
    my_string *name, *name2, *me, *extension, *deleted_file;
    initialize_string(&name);
    initialize_string(&name2);
    initialize_string(&me);
    initialize_string(&extension);
    initialize_string(&deleted_file);
    straight_assign_string(extension, ".id");
    assign_string(me, itoa(arg->id_client, length_of_int(arg->id_client)));
    me = concat_strings(me, extension);

    DIR *directory_ptr;
    struct dirent *direntp;
    char *processID;
    int *pID = (int *)malloc(sizeof(int));
    if (pID == NULL)
    {
        perror("malloc");
    }

    if ((directory_ptr = opendir(arg->common_dir->str)) == NULL) // open directory
    {
        perror("opendir");
        return false;
    }
    else
    {
        while ((direntp = readdir(directory_ptr)) != NULL) // read directory
        {
            sleep(1);
            if (check_file_extension(direntp, ".id")) // check for .id files
            {
                processID = strtok(direntp->d_name, ".");
                *pID = atoi(processID);
                straight_assign_string(name, direntp->d_name);
                straight_assign_string(name2, direntp->d_name);
                name = concat_strings(name, extension);
                if (search_list(*common_history, name) == NULL) // a new ID
                {
                    if (strcmp(me->str, name->str)) // if this ID is not me
                    {
                        (statistics.number_of_clients)++;
                        fprintf(logfile, "%s\n", "client ");
                        fprintf(logfile, "%s\n", name2->str);
                        if (*pID > statistics.max_id)
                        {
                            statistics.max_id = *pID;
                        }
                        if (*pID < statistics.min_id)
                        {
                            statistics.min_id = *pID;
                        }
                        insert_list(common_history, name); // save the new ID at history (it's a list)
                        comm_infos.number_of_failures = 0;
                        born_sender_receiver(arg, *pID, subdirs, files); // make a sender process and a receiver process
                    }
                }
                if (*common_history != NULL)
                {
                    if (check_for_deleted_files(common_history, arg->common_dir, &deleted_file) == true) // a .id file deleted !
                    {
                        (statistics.number_of_dead_clients)++;
                        printf("GONE %s\n", deleted_file->str);
                        born_remover_from_mirror(deleted_file, arg->mirror_dir); // delete the directory from mirror
                    }
                }
            }
        }
        closedir(directory_ptr);
    }
    destroy_string(name);
    destroy_string(name2);
    destroy_string(me);
    destroy_string(extension);

    return true;
}

void born_remover_from_mirror(my_string *deleted_file, my_string *mirror_dir)
{
    pid_t remover;
    remover = fork();
    if (remover == 0) // remover
    {
        char *temp;
        my_string *command, *cd, *directory, *plus, *return_command;
        initialize_string(&command);
        initialize_string(&cd);
        initialize_string(&directory);
        initialize_string(&plus);
        initialize_string(&return_command);
        straight_assign_string(return_command, "cd ./..");
        straight_assign_string(plus, " && ");
        straight_assign_string(command, "rm -rf ");
        temp = strtok(deleted_file->str, ".");
        straight_assign_string(directory, temp);
        command = concat_strings(command, directory);

        straight_assign_string(cd, "cd ");
        cd = concat_strings(cd, mirror_dir);
        cd = concat_strings(cd, plus);
        cd = concat_strings(cd, command);
        cd = concat_strings(cd, plus);
        cd = concat_strings(cd, return_command);

        printf("%s\n", cd->str);
        system(cd->str); // deleting directory ./deleted_file/ from mirror because this client gone

        destroy_string(command);
        destroy_string(cd);
        destroy_string(directory);
        exit(0);
        destroy_string(return_command);
    }
    else // parent
    {
        __pid_t w;
        int status = 0;
        w = waitpid(remover, &status, 0); // just wait for remover
        if (w == -1)
        {
            perror("waitpid");
        }
    }
}

void communication_handler(int signo, siginfo_t *siginfo, void *context)
{
    int status1 = 0;
    if (signo == SIGUSR1) // in case of success
    {
        sleep(5);
        printf("1/2 of communication DONE!\n");
        int w1;
        w1 = waitpid(siginfo->si_pid, &status1, WUNTRACED); // wait for child
        if (w1 == -1)
        {
            perror("waitpid");
        }
    }
    if (signo == SIGUSR2) // in case of failure
    {
        sleep(10);
        int w1;
        w1 = waitpid(siginfo->si_pid, &status1, WUNTRACED); // wait for child
        if (w1 == -1)
        {
            perror("waitpid");
            exit(2);
        }
        (comm_infos.number_of_failures)++; // count number of failures
        printf("FAILURE %d\n", comm_infos.number_of_failures);
        kill(comm_infos.sender, SIGKILL);   // kill sender
        kill(comm_infos.receiver, SIGKILL); // kill receiver

        if (comm_infos.number_of_failures < 3) // only 3 times brn sender and receiver
        {
            printf("Let's try for %d time\n", comm_infos.number_of_failures);
            born_sender_receiver(comm_infos.arg, comm_infos.otherID, comm_infos.subdirs, comm_infos.files);
        }
        else
        {
            comm_infos.number_of_failures = 0;
            check_common_dir(comm_infos.arg, &common_history, comm_infos.subdirs, comm_infos.files);
        }
    }
}

void born_sender_receiver(arguments *arg, int otherID, list_node *subdirs, list_node *files)
{
    if (comm_infos.number_of_failures == 0)
    {
        comm_infos.arg = (arguments *)malloc(sizeof(arguments));
        if (comm_infos.arg == NULL)
        {
            perror("malloc");
            return;
        }
        initialize_string(&(comm_infos.arg->mirror_dir));
        initialize_string(&(comm_infos.arg->input_dir));
        initialize_string(&(comm_infos.arg->common_dir));
        initialize_string(&(comm_infos.arg->log_file));
        assign_string(comm_infos.arg->mirror_dir, arg->mirror_dir);
        assign_string(comm_infos.arg->input_dir, arg->input_dir);
        assign_string(comm_infos.arg->common_dir, arg->common_dir);
        assign_string(comm_infos.arg->log_file, arg->log_file);
        comm_infos.arg->buffer_size = arg->buffer_size;
        comm_infos.arg->id_client = arg->id_client;

        comm_infos.otherID = otherID;
        comm_infos.subdirs = subdirs;
        comm_infos.files = files;
    }

    sigset_t mask;
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);

    struct sigaction signalAction;
    signalAction.sa_flags = SA_SIGINFO;
    signalAction.sa_handler = (void (*)(int))communication_handler;
    sigaction(SIGUSR1, &signalAction, NULL);
    sigaction(SIGUSR2, &signalAction, NULL);

    pid_t sender, receiver, wpid;

    my_string *fifo_name, *source, *dest, *to, *slash, *extension, *temp; // string staf
    initialize_string(&fifo_name);
    initialize_string(&source);
    initialize_string(&dest);
    initialize_string(&to);
    initialize_string(&slash);
    initialize_string(&extension);
    initialize_string(&temp);
    straight_assign_string(to, "_to_");
    straight_assign_string(slash, "/");
    straight_assign_string(extension, ".fifo");

    comm_infos.parent = getpid();
    sender = fork();
    if (sender == 0) // sender
    {
        comm_infos.sender = getpid();
        sender_process(source, dest, to, slash, extension, temp, fifo_name, otherID, arg, subdirs, files);
    }
    else
    {
        comm_infos.sender = sender;
        receiver = fork();
        if (receiver == 0) // receiver
        {
            receiver_process(source, dest, to, slash, extension, temp, fifo_name, otherID, arg);
        }
        else // parent
        {
            comm_infos.receiver = receiver;
        }
    }
    return;
}

void sender_process(my_string *source, my_string *dest, my_string *to, my_string *slash, my_string *extension, my_string *temp, my_string *fifo_name,
                    int otherID,
                    arguments *arg,
                    list_node *subdirs, list_node *files)
{
    my_string *rm, *rm_command;
    initialize_string(&rm);
    initialize_string(&rm_command);
    straight_assign_string(rm, "rm ");

    assign_string(source, itoa(arg->id_client, length_of_int(arg->id_client))); // build name of fifo
    assign_string(dest, itoa(otherID, length_of_int(otherID)));
    temp = concat_strings(source, to);
    temp = concat_strings(temp, dest);
    temp = concat_strings(slash, temp);
    temp = concat_strings(arg->common_dir, temp);
    fifo_name = concat_strings(temp, extension);
    rm_command = concat_strings(rm, fifo_name);
    if (mkfifo(fifo_name->str, 0666) == -1) // create a fifo
    {
        if (errno != EEXIST)
        {
            perror("mkfifo");
            destroy_string(rm);
            destroy_string(rm_command);
            kill(getppid(), SIGUSR2);
            exit(2);
        }
    }

    int filedes;
    if ((filedes = open(fifo_name->str, O_WRONLY)) < 0) // open fifo
    {
        perror("fifo open error");
        system(rm_command->str);
        destroy_string(rm);
        destroy_string(rm_command);
        kill(getppid(), SIGUSR2);
        exit(2);
    }
    write_subdirs(source, dest, to, slash, extension, temp, fifo_name, otherID, filedes, arg, subdirs); // write at fifo the directories of input
    write_files(source, dest, to, slash, extension, temp, fifo_name, otherID, filedes, arg, files);     /// write at the fifo the files of input

    destroy_string(fifo_name);
    destroy_string(source);
    destroy_string(dest);
    destroy_string(to);
    destroy_string(slash);
    destroy_string(temp);
    destroy_string(extension);
    kill(getppid(), SIGUSR1); // successful sender
    exit(0);
}

void write_subdirs(my_string *source, my_string *dest, my_string *to, my_string *slash, my_string *extension, my_string *temp, my_string *fifo_name,
                   int otherID, int filedes,
                   arguments *arg,
                   list_node *subdirs)
{
    my_string *rm, *rm_command;
    initialize_string(&rm);
    initialize_string(&rm_command);
    straight_assign_string(rm, "rm ");
    rm_command = concat_strings(rm, fifo_name);

    int nwrite;
    char buffer[arg->buffer_size];
    list_node *current = subdirs;
    my_file *file_infos = NULL;
    short int temp_short, ts;
    unsigned int temp_int, ti;
    int number_of_bytes = 0;
    do
    {
        if (!(initialize_file(current->data, &file_infos, false))) // return informations about directory
        {
            error_handling(17);
            system(rm_command->str);
            destroy_string(rm);
            destroy_string(rm_command);
            kill(getppid(), SIGUSR2);
            exit(2);
        }
        memset(buffer, '\0', arg->buffer_size);
        temp_short = file_infos->length_of_name;
        memcpy(buffer, &temp_short, 2);
        buffer[3] = '\0';
        memcpy(&ts, buffer, 2);
        if ((nwrite = write(filedes, buffer, 2)) == -1) // write length of name oof directory
        {
            perror("Error in Writing");
            system(rm_command->str);
            destroy_string(rm);
            destroy_string(rm_command);
            kill(getppid(), SIGUSR2);
            exit(2);
        }
        number_of_bytes = number_of_bytes + 2;

        memset(buffer, '\0', arg->buffer_size);
        memcpy(buffer, file_infos->name_of_file->str, file_infos->length_of_name);
        if ((nwrite = write(filedes, buffer, file_infos->length_of_name)) == -1) // write name of directory
        {
            perror("Error in Writing");
            system(rm_command->str);
            destroy_string(rm);
            destroy_string(rm_command);
            kill(getppid(), SIGUSR2);
            exit(2);
        }
        number_of_bytes = number_of_bytes + file_infos->length_of_name;

        memset(buffer, '\0', arg->buffer_size);
        temp_int = file_infos->length_of_file;
        memcpy(buffer, &temp_int, 4);
        buffer[4] = '\0';
        memcpy(&ti, buffer, 4);
        if ((nwrite = write(filedes, buffer, 4)) == -1) // write length of directory (== 0)
        {
            perror("Error in Writing");
            system(rm_command->str);
            destroy_string(rm);
            destroy_string(rm_command);
            kill(getppid(), SIGUSR2);
            exit(2);
        }
        number_of_bytes = number_of_bytes + 4;
        current = current->next;
    } while (current != NULL);
    fprintf(logfile, "%s\n", "bs ");
    fprintf(logfile, "%d\n", number_of_bytes);
    destroy_string(rm);
    destroy_string(rm_command);
}

void write_files(my_string *source, my_string *dest, my_string *to, my_string *slash, my_string *extension, my_string *temp, my_string *fifo_name,
                 int otherID, int filedes,
                 arguments *arg,
                 list_node *files)
{
    unsigned int temp_int, ti;
    short int temp_short, ts;
    int sum, out_of_order, temp_sum;
    temp_sum = 0;
    my_string *rm, *rm_command;
    initialize_string(&rm);
    initialize_string(&rm_command);
    straight_assign_string(rm, "rm ");
    rm_command = concat_strings(rm, fifo_name);

    int number_of_files = 0;
    int number_of_bytes = 0;
    int nwrite;
    char buffer[arg->buffer_size];
    list_node *current = files;
    my_file *file_infos = NULL;
    do
    {
        if (!(initialize_file(current->data, &file_infos, true))) // return informations about file
        {
            error_handling(17);
            system(rm_command->str);
            destroy_string(rm);
            destroy_string(rm_command);
            kill(getppid(), SIGUSR2);
            exit(2);
        }
        (number_of_files)++;
        memset(buffer, '\0', arg->buffer_size);
        temp_short = file_infos->length_of_name;
        memcpy(buffer, &temp_short, 2);
        buffer[3] = '\0';
        memcpy(&ts, buffer, 2);
        if ((nwrite = write(filedes, buffer, 2)) == -1) // write length of name of file
        {
            perror("Error in Writing");
            system(rm_command->str);
            destroy_string(rm);
            destroy_string(rm_command);
            kill(getppid(), SIGUSR2);
            exit(2);
        }
        number_of_bytes = number_of_bytes + 2;

        memset(buffer, '\0', arg->buffer_size);
        memcpy(buffer, file_infos->name_of_file->str, file_infos->length_of_name);
        if ((nwrite = write(filedes, buffer, file_infos->length_of_name)) == -1) // write name of file
        {
            perror("Error in Writing");
            system(rm_command->str);
            destroy_string(rm);
            destroy_string(rm_command);
            kill(getppid(), SIGUSR2);
            exit(2);
        }
        number_of_bytes = number_of_bytes + file_infos->length_of_name;

        memset(buffer, '\0', arg->buffer_size);
        temp_int = file_infos->length_of_file;
        memcpy(buffer, &temp_int, 4);
        buffer[4] = '\0';
        memcpy(&ti, buffer, 4);
        if ((nwrite = write(filedes, buffer, 4)) == -1) // write length of content of file
        {
            perror("Error in Writing");
            system(rm_command->str);
            destroy_string(rm);
            destroy_string(rm_command);
            kill(getppid(), SIGUSR2);
            exit(2);
        }
        number_of_bytes = number_of_bytes + 4;

        sum = file_infos->length_of_file;
        my_string *temp_buffer;
        initialize_string(&temp_buffer);
        straight_assign_string(temp_buffer, file_infos->buffer);
        for (char *i = temp_buffer->str; *i != '\0'; i = i + (arg->buffer_size)) // write content of file
        {
            memset(buffer, '\0', sizeof(buffer));
            sum -= arg->buffer_size;
            if (sum >= 0)
            {
                snprintf(buffer, arg->buffer_size, i);
                temp_sum += arg->buffer_size;
                if ((nwrite = write(filedes, buffer, arg->buffer_size)) == -1)
                {
                    perror("Error in Writing");
                    system(rm_command->str);
                    destroy_string(rm);
                    destroy_string(rm_command);
                    kill(getppid(), SIGUSR2);
                    exit(2);
                }
            }
            else
            {
                out_of_order = sum;
                sum += arg->buffer_size;
                snprintf(buffer, sum, i);
                temp_sum += sum;
                if ((nwrite = write(filedes, buffer, sum)) == -1)
                {
                    perror("Error in Writing");
                    system(rm_command->str);
                    destroy_string(rm);
                    destroy_string(rm_command);
                    kill(getppid(), SIGUSR2);
                    exit(2);
                }
                break;
            }
        }
        current = current->next;
    } while (current != NULL);
    number_of_bytes = number_of_bytes + temp_sum;
    memset(buffer, '\0', arg->buffer_size);
    temp_short = 0;
    memcpy(buffer, &temp_short, 2);
    buffer[3] = '\0';
    memcpy(&ts, buffer, 2);
    if ((nwrite = write(filedes, buffer, sizeof(short))) == -1) // write the END
    {
        perror("Error in Writing");
        system(rm_command->str);
        destroy_string(rm);
        destroy_string(rm_command);
        kill(getppid(), SIGUSR2);
        exit(2);
    }
    fprintf(logfile, "%s\n", "bs ");
    fprintf(logfile, "%d\n", number_of_bytes);
    fprintf(logfile, "%s\n", "fs ");
    fprintf(logfile, "%d\n", number_of_files);
    destroy_string(rm);
    destroy_string(rm_command);
}

void alarming(int signum) // more than 30 seconds for reading => FAILURE
{
    printf(" Alarm received from kernel \n ");
    kill(comm_infos.parent, SIGUSR2);
}

void receiver_process(my_string *source, my_string *dest, my_string *to, my_string *slash, my_string *extension, my_string *temp, my_string *fifo_name,
                      int otherID,
                      arguments *arg)
{
    signal(SIGALRM, alarming);
    my_string *rm, *rm_command;
    initialize_string(&rm);
    initialize_string(&rm_command);
    straight_assign_string(rm, "rm ");

    int number_of_files = 0;
    int number_of_bytes = 0;

    assign_string(source, itoa(otherID, length_of_int(otherID))); // build name of fifo
    assign_string(dest, itoa(arg->id_client, length_of_int(arg->id_client)));
    temp = concat_strings(source, to);
    temp = concat_strings(temp, dest);
    temp = concat_strings(slash, temp);
    temp = concat_strings(arg->common_dir, temp);
    fifo_name = concat_strings(temp, extension);
    rm_command = concat_strings(rm, fifo_name);

    if (mkfifo(fifo_name->str, 0666) == -1) // create a fifo
    {
        if (errno != EEXIST)
        {
            perror("mkfifo");
            destroy_string(rm);
            destroy_string(rm_command);
            kill(getppid(), SIGUSR2);
            exit(2);
        }
    }

    char buffer[arg->buffer_size];
    int filedes;
    if ((filedes = open(fifo_name->str, O_RDONLY)) < 0) // open fifo
    {
        perror("open");
        system(rm_command->str);
        kill(getppid(), SIGUSR2);
        exit(2);
    }

    int previous_readed = 0;
    int readed = 0;
    int sum = 0;
    short int legth_of_title;
    unsigned int legth_of_file;
    int temp_sum = 0;

    while (true)
    {
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        memset(buffer, '\0', sizeof(buffer));
        alarm(30);
        readed = read(filedes, buffer, 2); //read from fifo length of name
        number_of_bytes += readed;
        previous_readed = readed;
        sum += readed;
        if (readed < 0) // failure of read
        {
            perror("read");
            system(rm_command->str);
            destroy_string(rm);
            destroy_string(rm_command);
            kill(getppid(), SIGUSR2);
            exit(2);
        }
        while (readed > 0 && sum < 2)
        {
            alarm(30);
            readed = read(filedes, buffer + (sum * sizeof(char)), 2 - sum);
            number_of_bytes += readed;
            if (readed < 0 || readed == previous_readed)
            {
                perror("read");
                system(rm_command->str);
                destroy_string(rm);
                destroy_string(rm_command);
                kill(getppid(), SIGUSR2);
                exit(2);
            }
            previous_readed += readed;
            sum += readed;
        }
        buffer[3] = '\0';
        memcpy(&legth_of_title, buffer, 2);
        if (legth_of_title == 0) // END
        {
            break;
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        number_of_files++;
        memset(buffer, '\0', sizeof(buffer));
        sum = 0;
        alarm(30);
        readed = read(filedes, buffer, legth_of_title); // read from fifo name of file
        number_of_bytes += readed;
        if (readed < 0) // failure of read
        {
            perror("read");
            system(rm_command->str);
            destroy_string(rm);
            destroy_string(rm_command);
            kill(getppid(), SIGUSR2);
            exit(2);
        }
        previous_readed += readed;
        sum += readed;
        while (readed > 0 && sum <= legth_of_title)
        {
            alarm(30);
            readed = read(filedes, buffer + (sum * sizeof(char)), legth_of_title - sum);
            number_of_bytes += readed;
            if (readed < 0) // failure of read
            {
                perror("read");
                system(rm_command->str);
                destroy_string(rm);
                destroy_string(rm_command);
                kill(getppid(), SIGUSR2);
                exit(2);
            }
            previous_readed += readed;
            sum += readed;
        }
        char *name_of_file = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
        memcpy(name_of_file, buffer, legth_of_title);
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        memset(buffer, '\0', sizeof(buffer));
        sum = 0;
        alarm(30);
        readed = read(filedes, buffer, 4); // read from fifo length of content of file
        number_of_bytes += readed;
        if (readed < 0) // failure of read
        {
            perror("read");
            system(rm_command->str);
            destroy_string(rm);
            destroy_string(rm_command);
            kill(getppid(), SIGUSR2);
            exit(2);
        }
        previous_readed += readed;
        sum += readed;
        while (readed > 0 && sum < 4)
        {
            alarm(30);
            readed = read(filedes, buffer + (sum * sizeof(char)), 4 - sum);
            number_of_bytes += readed;
            if (readed < 0) // failure of read
            {
                perror("read");
                system(rm_command->str);
                destroy_string(rm);
                destroy_string(rm_command);
                kill(getppid(), SIGUSR2);
                exit(2);
            }
            previous_readed += readed;
            sum += readed;
        }
        buffer[4] = '\0';
        memcpy(&legth_of_file, buffer, 4);
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        char file_content[legth_of_file];
        memset(file_content, '\0', legth_of_file);
        if (legth_of_file > 0)
        {
            memset(buffer, '\0', sizeof(buffer));
            sum = 0;
            alarm(30);
            readed = read(filedes, buffer, arg->buffer_size); // read from fifo content of file
            temp_sum += readed;
            if (readed < 0) // failure of read
            {
                perror("read");
                system(rm_command->str);
                destroy_string(rm);
                destroy_string(rm_command);
                kill(getppid(), SIGUSR2);
                exit(2);
            }
            previous_readed += readed;
            sum += readed;
            memcpy(file_content, buffer, arg->buffer_size);
            while (readed > 0 && sum < legth_of_file - 1)
            {
                memset(buffer, '\0', arg->buffer_size);
                if (legth_of_file - sum >= arg->buffer_size)
                {
                    alarm(30);
                    readed = read(filedes, buffer, arg->buffer_size);
                    temp_sum += readed;
                    if (readed < 0) // failure of read
                    {
                        perror("read");
                        system(rm_command->str);
                        destroy_string(rm);
                        destroy_string(rm_command);
                        kill(getppid(), SIGUSR2);
                        exit(2);
                    }
                    previous_readed += readed;
                    strcat(file_content, buffer);
                }
                else
                {
                    alarm(30);
                    readed = read(filedes, buffer, legth_of_file - sum);
                    temp_sum += readed;
                    if (readed < 0) // failure of read
                    {
                        perror("read");
                        system(rm_command->str);
                        destroy_string(rm);
                        destroy_string(rm_command);
                        kill(getppid(), SIGUSR2);
                        exit(2);
                    }
                    previous_readed += readed;
                    strcat(file_content, buffer);
                }
                sum += readed;
            }
        }
        number_of_bytes += temp_sum;
        fprintf(logfile, "%s\n", "br ");
        fprintf(logfile, "%d\n", number_of_bytes);
        fprintf(logfile, "%s\n", "fr ");
        fprintf(logfile, "%d\n", number_of_files);
        if (name_of_file[0] == '.') // if name of file begins with . => it's a directory
        {
            if (!(set_directory(name_of_file, arg->mirror_dir, otherID))) // make this directory at mirror
            {
                error_handling(18);
                system(rm_command->str);
                destroy_string(rm);
                destroy_string(rm_command);
                kill(getppid(), SIGUSR2);
                exit(2);
            }
        }
        else // else it's a file
        {
            if (!(set_file(name_of_file, file_content, legth_of_file, arg->mirror_dir, otherID))) // make this file at mirror
            {
                error_handling(18);
                system(rm_command->str);
                destroy_string(rm);
                destroy_string(rm_command);
                kill(getppid(), SIGUSR2);
                exit(2);
            }
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    system(rm_command->str);
    destroy_string(rm);
    destroy_string(rm_command);
    destroy_string(fifo_name);
    destroy_string(source);
    destroy_string(dest);
    destroy_string(to);
    destroy_string(slash);
    destroy_string(temp);
    destroy_string(extension);
    kill(getppid(), SIGUSR1); // succesful receiver
    exit(0);
}

bool communication_preprocess(my_string *input_dir, list_node **subdirs, list_node **files)
{
    if (!(get_subdirs_and_files(input_dir, subdirs, files))) // take a list of files and subdirectories (for input directory)
    {
        error_handling(14);
        return false;
    }
    return true;
}

// communication routine that includes subroutines for communication
bool communication(arguments *arg)
{
    statistics.number_of_clients = 0;
    statistics.max_id = 0;
    statistics.min_id = 123456789;
    statistics.number_of_dead_clients = 0;

    list_node *subdirs = NULL; // all directories of this client
    list_node *files = NULL;   // all files of this client

    if (!(communication_preprocess(arg->input_dir, &subdirs, &files))) // returns directories and files of this client
    {
        error_handling(15);
        return false;
    }

    logfile = NULL;
    logfile = fopen(arg->log_file->str, "w");

    while (check_common_dir(arg, &common_history, subdirs, files))
        ;

    free_list(files);
    free_list(subdirs);
    free_list(common_history);
    return true;
}