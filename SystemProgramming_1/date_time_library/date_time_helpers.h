#ifndef _DATE_TIME_HELPERS_H_
#define _DATE_TIME_HELPERS_H_

#include <string.h>

void concatinate_date_and_time(char string_date[10], char string_time[5], char *moment);
char *get_current_date();
char *get_current_time();

#endif