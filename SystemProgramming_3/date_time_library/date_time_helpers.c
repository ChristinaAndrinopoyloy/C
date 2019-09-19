#include "date_time_helpers.h"

void concatinate_date_and_time(char string_date[10], char string_time[5], char *moment)
{
    strcpy(moment, string_date);
    strcat(moment, "#");
    strcat(moment, string_time);
    return;
}