#include "factory.h"

////////////////////////////////////////////////////////////////////////////// Consruction //////////////////////////////////////////////////////////////////////////////////////////////////
/* Step of CONSTRUCTION */
void construction(int Y, int IDvalue, int border_of_ID, int type, int sem_for_dyer, int empty, int full, shared_memory *dyer, double *average_wait_time)
{
    int ID = IDvalue;
    double wait_time;
    double sum_wait_time = 0.0;

    for (int i = 0; i < Y; i++)
    {
        component c;
        make_component(&c, &ID, border_of_ID, type);
        dyer_routine(sem_for_dyer, empty, full, dyer, &c, &wait_time);
        sum_wait_time += wait_time;
    }
    *average_wait_time = sum_wait_time / Y;
}

/* routine of making component */
void make_component(component *c, int *ID, int border_of_ID, int type)
{
    int sleep_time = ((rand() + (*ID)) % 9000);

    usleep(sleep_time);
    clock_t start = clock();
    c->start = start;
    if (++(*ID) <= border_of_ID)
        c->ID = (*ID) + 1;
    c->type_of_component = type;
    c->colour = 'n'; // n for none colour
    c->checked = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////// Dyer //////////////////////////////////////////////////////////////////////////////////////////////////
/* Paint one component at dyer */
void dyer_routine(int sem_for_dyer, int empty, int full, shared_memory *dyer, component *c, double *wait_time)
{
    clock_t start, end;

    start = clock();
    down(empty, 0);
    down(sem_for_dyer, 0);
    end = clock();
    // CS
    memcpy(dyer->ptr, c, sizeof(component));
    if (((rand() + c->ID) % 10) > 5)
    {
        c->colour = 'b'; // b for black
    }
    else
    {
        c->colour = 'w'; // w for white
    }
    usleep((c->type_of_component * 200));
    up(sem_for_dyer, 0);
    up(full, 0);
    *wait_time = (double)(end - start) / CLOCKS_PER_SEC;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////// Consume //////////////////////////////////////////////////////////////////////////////////////////////

void consume_from_dyer(int sem_for_dyer, int empty, int full, shared_memory *dyer, component *c)
{

    down(full, 0);
    down(sem_for_dyer, 0);
    // CS
    memcpy(c, dyer->ptr, sizeof(component));
    up(sem_for_dyer, 0);
    up(empty, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////// Transporter /////////////////////////////////////////////////////////////////////////////////////////////

void transporter(int Y, int sem_for_dyer, int empty, int full, shared_memory *dyer,
                 int *sem_for_checker, int *empty_checker, int *full_checker, shared_memory *checker1, shared_memory *checker2, shared_memory *checker3)
{
    component component_for_checking;

    for (int i = 0; i < 3 * Y; i++)
    {
        consume_from_dyer(sem_for_dyer, empty, full, dyer, &component_for_checking);

        if (component_for_checking.type_of_component == 1)
        {
            down(empty_checker[0], 0);
            down(sem_for_checker[0], 0);
            // CS
            memcpy(checker1->ptr, &component_for_checking, sizeof(component));
            up(sem_for_checker[0], 0);
            up(full_checker[0], 0);
        }
        else if (component_for_checking.type_of_component == 2)
        {
            down(empty_checker[1], 0);
            down(sem_for_checker[1], 0);
            // CS
            memcpy(checker2->ptr, &component_for_checking, sizeof(component));
            up(sem_for_checker[1], 0);
            up(full_checker[1], 0);
        }
        else
        {
            down(empty_checker[2], 0);
            down(sem_for_checker[2], 0);
            // CS
            memcpy(checker3->ptr, &component_for_checking, sizeof(component));
            up(sem_for_checker[2], 0);
            up(full_checker[2], 0);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////// Checking /////////////////////////////////////////////////////////////////////////////////////////////
int checking(int Y, int sem_for_checker, int empty_checker, int full_checker, shared_memory *checker_memory,
             int sem_for_put_together_mem, int empty_put_together_mem, int full_put_together_mem, shared_memory *put_together_mem)
{
    component c;
    struct final_step_helper *current;

    pthread_t *threads;
    threads = malloc(sizeof(pthread_t) * Y);

    for (int i = 0; i < Y; i++)
    {
        down(full_checker, 0);
        down(sem_for_checker, 0);
        // CS
        memcpy(&c, checker_memory->ptr, sizeof(component));
        c.checked = 1;
        usleep((c.type_of_component * 100));
        current = malloc(sizeof(struct final_step_helper));
        current->current_component = c;
        current->memory = put_together_mem;
        current->semaphore_ID_empty = empty_put_together_mem;
        current->semaphore_ID_full = full_put_together_mem;
        current->semaphore_ID_sem = sem_for_put_together_mem;
        if (pthread_create(&threads[i], NULL, put_together_helper, current))
        {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
        up(sem_for_checker, 0);
        up(empty_checker, 0);
    }

    for (int i = 0; i < Y; i++)
    {
        if (pthread_join(threads[i], NULL))
        {
            fprintf(stderr, "Error joining thread\n");
            return 2;
        }
    }
    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////// Put Together ///////////////////////////////////////////////////////////////////////////////////////////////////
void put_together(int Y, int *sem_for_put_together_mem, int *empty_put_together_mem, int *full_put_together_mem, shared_memory **put_together_mem)
{
    component c;
    int barcode[3];
    final_product current_final_product;
    char *determinant = (char *)malloc(9);
    clock_t total_end;
    clock_t min_start = 100000;
    double sum = 0.0;

    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            down(full_put_together_mem[j], 0);
            down(sem_for_put_together_mem[j], 0);
            // CS
            memcpy(&c, put_together_mem[j]->ptr, sizeof(component));
            c.end = clock();
            if (c.start < min_start)
            {
                min_start = c.start;
            }
            barcode[j] = c.ID;
            up(sem_for_put_together_mem[j], 0);
            up(empty_put_together_mem[j], 0);
        }
        usleep(100);
        final_product_determinant(determinant, barcode[0], barcode[1], barcode[2]);
        total_end = clock();
        current_final_product.determinant = determinant;
        current_final_product.average_total_time = (double)(total_end - min_start) / CLOCKS_PER_SEC;
        sum += current_final_product.average_total_time;
        printf("Συναρμολογήθηκε το %s προιόν σε χρόνο %f \n", current_final_product.determinant, current_final_product.average_total_time);
        min_start = 100000;
    }
    printf("Ο μέσος χρόνος κατασκευής για κάθε αντικείμενο είναι %f \n", sum / (double)Y);
}

void *put_together_helper(void *current)
{
    down(((struct final_step_helper *)current)->semaphore_ID_empty, 0);
    down(((struct final_step_helper *)current)->semaphore_ID_sem, 0);
    // CS
    memcpy(((struct final_step_helper *)current)->memory->ptr, &(((struct final_step_helper *)current)->current_component), sizeof(component));
    up(((struct final_step_helper *)current)->semaphore_ID_sem, 0);
    up(((struct final_step_helper *)current)->semaphore_ID_full, 0);
    free(current);
    return NULL;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////