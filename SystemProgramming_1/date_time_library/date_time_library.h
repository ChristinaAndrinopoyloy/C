#ifndef _DATE_TIME_LIBRARY_H_
#define _DATE_TIME_LIBRARY_H_

#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <time.h>
#include "date_time_helpers.h"
#include "./../error_handling/error_handling.h"

char *current_date_function();
char *current_time_function();
bool compare_moments(char *string_date_old, char *string_time_old, char *string_date_new, char *string_time_new);
bool time_borders(char *string_date_down_border, char *string_time_down_border, char *string_date_up_border, char *string_time_up_border, char *string_date, char *string_time);

#endif