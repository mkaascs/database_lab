#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include "../../../memory/stats.h"

#include "converter.h"

#include <ctype.h>

void trim_quotation(const char* origin, char* result) {
    const int length = strlen(origin);

    int left_border = origin[0] == '\'' || origin[0] == '\"';
    int right_border = length - 1 - (origin[length - 1] == '\'' || origin[length - 1] == '\"');

    strncpy(result, origin + left_border, right_border - left_border + 1);
    result[right_border - left_border + 1] = '\0';
}

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
    char trim_value[24];
    trim_quotation(value, trim_value);

    struct tm time;
    memset(&time, 0, sizeof(struct tm));
    char* end_pointer = strptime(trim_value, "%H:%M:%S", &time);

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

int string_to_status(const char* value, Status* status) {
    size_t length = strlen(value);
    int left_border = *value == '\'' || *value == '"';
    int right_border = length - 1 - (*(value + length - 1) == '\'' || *(value + length - 1) == '"');

    for (int index = 0; index < STATUS_COUNT; index++) {
        if (strncmp(status_string[index], value + left_border, right_border - left_border) == 0) {
            *status = index;
            return 1;
        }
    }

    return 0;
}

int string_to_statuses(const char* value, Status** statuses, int* statuses_count) {
    if (value == NULL || statuses == NULL || statuses_count == NULL)
        return 0;

    *statuses = NULL;
    *statuses_count = 0;

    size_t value_length = strlen(value);

    const char* start = value + (*value == '[');
    const char* end = value + value_length - 1 - (*(value + value_length - 1) == ']');

    char buffer[32];
    int buffer_index = 0;
    Status* temp_statuses = (Status*)track_malloc(STATUS_COUNT * sizeof(Status));

    if (temp_statuses == NULL)
        return 0;

    int count = 0;
    int in_quote = 0;
    for (const char* pointer = start; pointer <= end; pointer++) {
        if (*pointer == '\'' || *pointer == '"')
            in_quote = !in_quote;

        else if (*pointer == ',' && !in_quote) {
            buffer[buffer_index] = '\0';
            Status status;
            if (string_to_status(buffer, &status))
                temp_statuses[count++] = status;

            buffer_index = 0;
        }

        else if (!isspace(*pointer) || in_quote)
            if (buffer_index < sizeof(buffer) - 1)
                buffer[buffer_index++] = *pointer;
    }

    if (buffer_index > 0) {
        buffer[buffer_index] = '\0';
        Status status;
        if (string_to_status(buffer, &status))
            temp_statuses[count++] = status;
    }

    if (count == 0) {
        track_free(temp_statuses);
        return 0;
    }

    *statuses = malloc(count * sizeof(Status));
    if (!*statuses) {
        track_free(temp_statuses);
        return 0;
    }

    memcpy(*statuses, temp_statuses, count * sizeof(Status));
    *statuses_count = count;
    track_free(temp_statuses);

    return 1;
}