#ifndef _STRING_LIBRARY_H_
#define _STRING_LIBRARY_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

typedef struct MyString
{
    char *str;
} my_string;

void initialize_string(my_string **source);
void assign_string(my_string *destination, my_string *source);
void straight_assign_string(my_string *destination, char *source);
my_string *concat_strings(my_string *first, my_string *last);
int length_of_int(int number);
my_string *itoa(int number, int length);
void destroy_string(my_string *source);

#endif