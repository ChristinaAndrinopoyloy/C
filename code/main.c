#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include "factory.h"
#include "helper.h"
#include "sem_mem.h"

#define NUMBER_OF_PROCESSES 3

int main(int argc, char const *argv[])
{
    int Y;

    if (argc == 1)
    {
        Y = 100; // Y <= 3333
    }
    else
    {
        Y = atoi(argv[1]);
        if (Y > 3333)
        {
            printf("ERROR! Y must be under 3333 \n");
            return 1;
        }
    }
    int IDvalue[3] = {-1, 3332, 6665};

    /////////////////////////////////////////////////////////////////////

    int sem_for_dyer = create_semaphore(1);
    int empty = create_semaphore(1);
    int full = create_semaphore(1);
    initialize_semaphore(sem_for_dyer, 1, 0);
    initialize_semaphore(empty, 1, 0);
    initialize_semaphore(full, 0, 0);

    shared_memory *dyer = get_shared_memory(1);

    shared_memory *checker1 = get_shared_memory(1);
    shared_memory *checker2 = get_shared_memory(1);
    shared_memory *checker3 = get_shared_memory(1);

    int sem_for_checker[3], empty_checker[3], full_checker[3];
    for (int i = 0; i < 3; i++)
    {
        sem_for_checker[i] = create_semaphore(1);
        empty_checker[i] = create_semaphore(1);
        full_checker[i] = create_semaphore(1);
    }

    for (int i = 0; i < 3; i++)
    {
        initialize_semaphore(sem_for_checker[i], 1, 0);
        initialize_semaphore(empty_checker[i], 1, 0);
        initialize_semaphore(full_checker[i], 0, 0);
    }

    shared_memory *put_together_mem[3];
    int sem_for_put_together_mem[3], empty_put_together_mem[3], full_put_together_mem[3];
    for (int i = 0; i < 3; i++)
    {
        put_together_mem[i] = get_shared_memory(1);
        sem_for_put_together_mem[i] = create_semaphore(1);
        empty_put_together_mem[i] = create_semaphore(1);
        full_put_together_mem[i] = create_semaphore(1);
    }

    for (int i = 0; i < 3; i++)
    {
        initialize_semaphore(sem_for_put_together_mem[i], 1, 0);
        initialize_semaphore(empty_put_together_mem[i], 1, 0);
        initialize_semaphore(full_put_together_mem[i], 0, 0);
    }
    //////////////////////////////////////////////////////////////////////////
    double average_paint_time[3];

    srand((unsigned)time(NULL));

    for (int i = 0; i < (2 * NUMBER_OF_PROCESSES) + 1; i++)
    {
        if (fork() == 0)
        {
            if (i < 3)
            {
                construction(Y, IDvalue[i], IDvalue[i] + 3333, i + 1, sem_for_dyer, empty, full, dyer, &average_paint_time[i]);
                if (i == 2)
                {
                    printf("Η αναμονη για το ΒΑΦΕΙΟ είναι %f \n", (average_paint_time[0] + average_paint_time[1] + average_paint_time[2]) / (double)3);
                }
            }
            else
            {
                if (i == 3)
                {
                    checking(Y, sem_for_checker[i - 3], empty_checker[i - 3], full_checker[i - 3], checker1,
                             sem_for_put_together_mem[i - 3], empty_put_together_mem[i - 3], full_put_together_mem[i - 3], put_together_mem[i - 3]);
                }
                else if (i == 4)
                {
                    checking(Y, sem_for_checker[i - 3], empty_checker[i - 3], full_checker[i - 3], checker2,
                             sem_for_put_together_mem[i - 3], empty_put_together_mem[i - 3], full_put_together_mem[i - 3], put_together_mem[i - 3]);
                }
                else if (i == 5)
                {
                    checking(Y, sem_for_checker[i - 3], empty_checker[i - 3], full_checker[i - 3], checker3,
                             sem_for_put_together_mem[i - 3], empty_put_together_mem[i - 3], full_put_together_mem[i - 3], put_together_mem[i - 3]);
                }
                else
                {
                    put_together(Y, sem_for_put_together_mem, empty_put_together_mem, full_put_together_mem, put_together_mem);
                }
            }
            exit(0);
        }
    }
    transporter(Y, sem_for_dyer, empty, full, dyer, sem_for_checker, empty_checker, full_checker, checker1, checker2, checker3);
    for (int i = 1; i < 3 * Y; i++)
    {
        wait(NULL);
    }

    delete_semaphore(full, 0);
    delete_semaphore(empty, 0);
    delete_semaphore(sem_for_dyer, 0);
    delete_shared_memory(dyer);
    delete_shared_memory(checker1);
    delete_shared_memory(checker2);
    delete_shared_memory(checker3);
    for (int i = 0; i < 3; i++)
    {
        delete_semaphore(sem_for_checker[i], 0);
        delete_semaphore(empty_checker[i], 0);
        delete_semaphore(full_checker[i], 0);
        delete_semaphore(sem_for_put_together_mem[i], 0);
        delete_semaphore(empty_put_together_mem[i], 0);
        delete_semaphore(full_put_together_mem[i], 0);
        delete_shared_memory(put_together_mem[i]);
    }

    return 0;
}