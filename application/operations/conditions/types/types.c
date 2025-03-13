#include <string.h>

#include "types.h"

#include "../../../parsing/parser.h"

int get_operation_type(const char* operation, OperationType* result) {
    if (strncmp(operation, "==", OPERATOR_LENGTH) == 0) {
        *result = Equal;
        return 0;
    }

    if (strncmp(operation, "!=", OPERATOR_LENGTH) == 0) {
        *result = NotEqual;
        return 0;
    }

    if (strncmp(operation, ">", OPERATOR_LENGTH) == 0) {
        *result = More;
        return 0;
    }

    if (strncmp(operation, ">=", OPERATOR_LENGTH) == 0) {
        *result = MoreOrEqual;
        return 0;
    }

    if (strncmp(operation, "<", OPERATOR_LENGTH) == 0) {
        *result = Less;
        return 0;
    }

    if (strncmp(operation, "<=", OPERATOR_LENGTH) == 0) {
        *result = LessOrEqual;
        return 0;
    }

    if (strncmp(operation, "/in/", OPERATOR_LENGTH) == 0) {
        *result = In;
        return 0;
    }

    if (strncmp(operation, "/not_in/", OPERATOR_LENGTH) == 0) {
        *result = NotIn;
        return 0;
    }

    return -1;
}

int match_float(float first, float second, OperationType operation, int* result) {
    if (operation == Equal) {
        *result = first == second;
        return 0;
    }

    if (operation == NotEqual) {
        *result = first != second;
        return 0;
    }

    if (operation == More) {
        *result = first > second;
        return 0;
    }

    if (operation == MoreOrEqual) {
        *result = first >= second;
        return 0;
    }

    if (operation == Less) {
        *result = first < second;
        return 0;
    }

    if (operation == LessOrEqual) {
        *result = first <= second;
        return 0;
    }

    return -1;
}

int match_int(int first, int second, OperationType operation, int* result) {
    return match_float(first, second, operation, result);
}

int match_string(const char* first, const char* second, OperationType operation, int* result) {
    int compared = strncmp(first, second, VALUE_LENGTH + 1);
    if (operation == Equal) {
        *result = compared == 0;
        return 0;
    }

    if (operation == NotEqual) {
        *result = compared != 0;
        return 0;
    }

    if (operation == More) {
        *result = compared > 0;
        return 0;
    }

    if (operation == MoreOrEqual) {
        *result = compared >= 0;
        return 0;
    }

    if (operation == Less) {
        *result = compared < 0;
        return 0;
    }

    if (operation == LessOrEqual) {
        *result = compared <= 0;
        return 0;
    }

    return -1;
}

int match_time(struct tm first, struct tm second, OperationType operation, int* result) {
    return match_int(
        first.tm_hour * 3600 + first.tm_min * 60 + first.tm_sec,
        second.tm_hour * 3600 + second.tm_min * 60 + second.tm_sec,
        operation,
        result);
}