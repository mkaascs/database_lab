#include "conditions.h"

#include "../fields.h"
#include "../../domain/converting/converter.h"
#include "types/types.h"

int match(Condition condition, ProcessInfo record, int* result) {
    FieldInfo field_info;
    if (!get_value(condition.field, &record, &field_info))
        return -1;

    OperationType operator;
    if (get_operation_type(condition.operator, &operator) == -1)
        return -1;

    if (field_info.type == Int) {
        int second;
        if (!string_to_int(condition.value, &second))
            return -1;

        if (match_int(*(int*)field_info.value, second, operator, result) == -1)
            return -1;
    }

    if (field_info.type == Float) {
        float second;
        if (!string_to_float(condition.value, &second))
            return -1;

        if (match_float(*(float*)field_info.value, second, operator, result) == -1)
            return -1;
    }

    if (field_info.type == String) {
        if (match_string((char*)field_info.value, condition.value, operator, result) == -1)
            return -1;
    }

    if (field_info.type == Time) {
        struct tm second;
        if (!string_to_time(condition.value, &second))
            return -1;

        if (match_time(*(struct tm*)field_info.value, second, operator, result) == -1)
            return -1;
    }

    return 0;
}
