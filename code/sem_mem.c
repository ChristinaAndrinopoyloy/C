#include "sem_mem.h"

/////////////////////////////////////////////////////////// Semaphores (inspired from semaphores_code1 Lab 2017-2018) //////////////////////////////////////////////////////////

/* routine for creating specified number of semaphores */
int create_semaphore(int number_of_semaphores)
{
    int semID;

    /* create new semaphore set of semaphores */
    if ((semID = semget(IPC_PRIVATE, number_of_semaphores, IPC_CREAT | 0600)) < 0) //0600 = read/alter by user
    {
        perror("ERROR! Cannot construct semaphore(s)");
        exit(1);
    }
    return semID;
}

/* routine for initializing semaphore */
void initialize_semaphore(int semID, int value, int index)
{
    if (semctl(semID, index, SETVAL, value) < 0)
    {
        perror("ERROR! Cannot initialize semaphore");
        exit(1);
    }
}

/* routine for down */
void down(int semID, int index)
{
    struct sembuf sops[1]; /* only one semaphore operation to be executed */

    sops[0].sem_num = index; /* define operation on semaphore with given index */
    sops[0].sem_op = -1;     /* subtract 1 to value for P operation */
    sops[0].sem_flg = 0;     /* type "man semop" in shell window for details */

    if (semop(semID, sops, 1) == -1)
    {
        perror("ERROR at down");
        exit(1);
    }
}

/* routine for up */
void up(int semID, int index)
{
    struct sembuf sops[1]; /* define operation on semaphore with given index */

    sops[0].sem_num = index; /* define operation on semaphore with given index */
    sops[0].sem_op = 1;      /* add 1 to value for V operation */
    sops[0].sem_flg = 0;     /* type "man semop" in shell window for details */

    if (semop(semID, sops, 1) == -1)
    {
        perror("ERROR at up");
        exit(1);
    }
}

/* routine for deleting semaphore */
void delete_semaphore(int semID, int index)
{
    if (semctl(semID, index, IPC_RMID) < 0)
    {
        perror("ERROR at deleting semaphore");
        exit(1);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////// Shared memory /////////////////////////////////////////////////////////////////////////////////////

/* routine for creating shared memory */
shared_memory *get_shared_memory(int size)
{
    shared_memory *s_m = (shared_memory *)malloc(sizeof(shared_memory));

    s_m->size = size;

    if ((s_m->id = shmget(IPC_PRIVATE, s_m->size, IPC_CREAT | 0600)) == -1)
    {
        perror("ERROR with shmget()");
        exit(1);
    }

    if ((s_m->ptr = (void *)shmat(s_m->id, 0, 0)) == (void *)-1)
    {
        perror("ERROR with shmat()");
        exit(1);
    }

    return s_m;
}

/* routine for deleting shared memory */
void delete_shared_memory(shared_memory *s_m)
{
    if (shmdt(s_m->ptr) == -1)
    {
        perror("ERROR with shmdt()");
        exit(1);
    }

    if (shmctl(s_m->id, IPC_RMID, 0) == -1)
    {
        perror("ERROR with shmctl()");
        exit(1);
    }

    free(s_m);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////