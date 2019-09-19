#include "string_library.h"

void initialize_string(my_string **source)
{
    *source = (my_string *)malloc(sizeof(my_string));
    if (source == NULL)
    {
        perror("malloc");
        exit(1);
    }
    (*source)->str = NULL;
    return;
}

void assign_string(my_string *destination, my_string *source)
{
    destination->str = (char *)malloc((strlen(source->str) + 1)* sizeof(char));
    if (destination->str == NULL)
    {
        perror("malloc");
        exit(1);
    }
    strncpy(destination->str, source->str, strlen(source->str)+1);
    // destination->str[strlen(source->str)] = '\0';
    return;
}

void straight_assign_string(my_string *destination, char *source)
{
    destination->str = (char *)malloc((strlen(source) +1) * sizeof(char));
    if (destination->str == NULL)
    {
        perror("malloc");
        exit(1);
    }
    strncpy(destination->str, source, strlen(source)+1);
    // destination->str[strlen(source)] = '\0';
    return;
}

my_string *concat_strings(my_string *first, my_string *last)
{
    my_string *concat_string;
    initialize_string(&concat_string);

    concat_string->str = (char *)malloc((strlen(first->str) + strlen(last->str) + 1)*sizeof(char));
    if (concat_string->str == NULL)
    {
        perror("malloc");
        exit(1);
    }
    strcpy(concat_string->str, first->str);
    strcat(concat_string->str, last->str);
    return concat_string;
}

int length_of_int(int number)
{
    if (number < 10)
    {
        return 1;
    }
    return 1 + length_of_int(number / 10);
}

my_string *itoa(int number, int length)
{
    my_string *returned_string;
    initialize_string(&returned_string);
    returned_string->str = (char *)malloc(length*sizeof(char));
    if (returned_string->str == NULL)
    {
        perror("malloc");
        exit(1);
    }
    sprintf(returned_string->str, "%d", number);
    return returned_string;
}

void destroy_string(my_string *source)
{
    free(source->str);
    free(source);
    return;
}