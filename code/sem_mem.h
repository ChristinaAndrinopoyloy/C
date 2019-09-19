#ifndef _SEM_MEM_H_
#define _SEM_MEM_H_

//includes
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <errno.h>
#include "product_types.h"

//typedefs
typedef struct shared_memory
{
    int id;
    int size;
    void *ptr;
} shared_memory;

// Function prototypes
// Semaphores
int create_semaphore(int number_of_semaphores);             //routine for creating semaphore
void initialize_semaphore(int semID, int value, int index); // routine for initializing semaphore
void down(int semID, int index);                            // routine for down
void up(int semID, int index);                              // routine for up
void delete_semaphore(int semID, int index);                // routine for deleting semaphore
// Shared memory
shared_memory *get_shared_memory(int size);                                       // routine for creating shared memory
void delete_shared_memory(shared_memory *sm);                                     // routine for deleting shared memory

#endif