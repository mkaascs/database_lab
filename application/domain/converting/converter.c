#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

#include "converter.h"

int string_to_int(const char* value, int* result) {
    char* end_pointer;

    errno = 0;
    long number = strtol(value, &end_pointer, 10);

    if (errno == ERANGE || number < INT_MIN || number > INT_MAX)
        return 0;

    if (end_pointer == value)
        return 0;

    while (*end_pointer == ' ')
        end_pointer++;

    if (*end_pointer != '\0')
        return 0;

    *result = number;
    return 1;
}

int string_to_time(const char* value, struct tm* result) {
    struct tm time = {0};
    char* end_pointer = strptime(value, "%H:%M:%S", &time);

    if (end_pointer == NULL)
        return 0;

    while (*end_pointer == ' ')
        end_pointer++;

    if (*end_pointer != '\0')
        return 0;

    if (time.tm_hour < 0 || time.tm_hour > 23 ||
        time.tm_min < 0 || time.tm_min > 59 ||
        time.tm_sec < 0 || time.tm_sec > 59)
        return 0;

    *result = time;
    return 1;
}

int string_to_float(const char* value, float* result) {
    char* end_pointer;

    errno = 0;
    float number = strtod(value, &end_pointer);

    if (errno == ERANGE)
        return 0;

    if (end_pointer == value)
        return 0;

    while (*end_pointer == ' ')
        end_pointer++;

    if (*end_pointer != '\0')
        return 0;

    *result = number;
    return 1;
}

int string_to_status(const char* value, Status* result) {
    int left_border = 0;
    int right_border = strlen(value) - 1;

    for (; value[left_border] == '\"' || value[left_border] == '\''; left_border++) {}
    for (; value[right_border] == '\"' || value[right_border] == '\''; right_border--) {}

    for (int index = 0; index < STATUS_COUNT; index++) {
        if (strncmp(value + left_border, status_string[index], right_border - left_border + 1) != 0) {
            *result = index;
            return 1;
        }
    }

    return 0;
}