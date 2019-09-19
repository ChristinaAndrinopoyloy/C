#ifndef _MAKE_PROCESS_H_
#define _MAKE_PROCESS_H_

// includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "product_types.h"
#include "helper.h"
#include "sem_mem.h"

// structs

struct final_step_helper
{
    shared_memory *memory;
    int semaphore_ID_full;
    int semaphore_ID_empty;
    int semaphore_ID_sem;
    component current_component;
};

// definitions
void construction(int Y, int IDvalue, int border_of_ID, int type, int sem_for_dyer, int empty, int full, shared_memory *dyer, double *average_wait_time);
void make_component(component *c, int *ID, int border_of_ID, int type);
void dyer_routine(int sem_for_dyer, int empty, int full, shared_memory *dyer, component *c, double *wait_time);
int checking(int Y, int sem_for_checker, int empty_checker, int full_checker, shared_memory *checker_memory,
             int sem_for_put_together_mem, int empty_put_together_mem, int full_put_together_mem, shared_memory *put_together_mem);
void consume_from_dyer(int sem_for_dyer, int empty, int full, shared_memory *dyer, component *c);
void transporter(int Y, int sem_for_dyer, int empty, int full, shared_memory *dyer,
                 int *sem_for_checker, int *empty_checker, int *full_checker, shared_memory *checker1, shared_memory *checker2, shared_memory *checker3);
void put_together(int Y, int *sem_for_put_together_mem, int *empty_put_together_mem, int *full_put_together_mem, shared_memory **put_together_mem);
void *put_together_helper(void *current);

#endif