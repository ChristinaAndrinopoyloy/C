#include "date_time_library.h"

char *current_date_function()
{
    char *string_date = (char *)malloc(11 * sizeof(char));
    char buffer[80];
    struct tm *mytime;

    time_t current_time = time(NULL);
    mytime = localtime(&current_time);
    strftime(buffer, sizeof(buffer) - 1, "%d-%m-%Y", mytime);
    for (int i = 0; i < 10; i++)
    {
        string_date[i] = buffer[i];
    }
    string_date[10] = '\0';
    return string_date;
}

char *current_time_function()
{
    char *string_time = (char *)malloc(6 * sizeof(char));
    char buffer[80];
    struct tm *mytime;

    time_t current_time = time(NULL);
    mytime = localtime(&current_time);
    strftime(buffer, sizeof(buffer) - 1, "%H:%M", mytime);
    for (int i = 0; i < 5; i++)
    {
        string_time[i] = buffer[i];
    }
    string_time[5] = '\0';
    return string_time;
}

bool compare_moments(char *string_date_old, char *string_time_old, char *string_date_new, char *string_time_new)
{
    char moment_old[16];
    char moment_new[16];
    struct tm old = {0};
    struct tm new = {0};
    time_t old_moment, new_moment;

    concatinate_date_and_time(string_date_old, string_time_old, moment_old);
    concatinate_date_and_time(string_date_new, string_time_new, moment_new);
    strptime(moment_old, "%d-%m-%Y#%H:%M", &old);
    strptime(moment_new, "%d-%m-%Y#%H:%M", &new);
    old_moment = mktime(&old);
    new_moment = mktime(&new);

    if ((difftime(new_moment, old_moment) < 0) || difftime(new_moment, old_moment) > 0)
    {
        return false;
    }
    else
    {
        true;
    }
}

bool time_borders(char *string_date_down_border, char *string_time_down_border,
                  char *string_date_up_border, char *string_time_up_border,
                  char *string_date, char *string_time)
{
    char moment_down_border[16];
    char moment_up_border[16];
    char moment[16];
    struct tm moment_down = {0};
    struct tm moment_up = {0};
    struct tm moment_ = {0};
    time_t down, up, current;

    concatinate_date_and_time(string_date_down_border, string_time_down_border, moment_down_border);
    concatinate_date_and_time(string_date_up_border, string_time_up_border, moment_up_border);
    concatinate_date_and_time(string_date, string_time, moment);

    strptime(moment_down_border, "%d-%m-%Y#%H:%M", &moment_down);
    strptime(moment_up_border, "%d-%m-%Y#%H:%M", &moment_up);
    strptime(moment, "%d-%m-%Y#%H:%M", &moment_);

    down = mktime(&moment_down);
    up = mktime(&moment_up);
    current = mktime(&moment_);

    if (difftime(current, down) > 0 && difftime(up, current) > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int get_timestamp(char *timestamp_date, char *timestamp_time)
{
    char timestamp_array[16];
    struct tm stamp = {0};
    time_t timestamp;

    concatinate_date_and_time(timestamp_date, timestamp_time, timestamp_array);
    strptime(timestamp_array, "%d-%m-%Y#%H:%M", &stamp);
    timestamp = mktime(&stamp);
    return (int)timestamp;
}