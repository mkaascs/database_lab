#include "../parsing/parser.h"
#include "../bool.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 12

const char *const command_type_names[] = {
    "insert", "select", "update", "delete", "uniq", "sort"
};

int tokenize(const char* origin, char symbol, char (*result)[FIELD_LENGTH + VALUE_LENGTH]) {
    int split_length = 0;
    int left_index = 0;
    bool in_quotation = false;
    const int length = strlen(origin);

    for (int index = 0; index <= length; index++) {
        if (origin[index] == '\"' || origin[index] == '\'')
            in_quotation = !in_quotation;

        if ((origin[index] == symbol && !in_quotation) || index == length) {
            strncpy(result[split_length], origin + left_index, index - left_index);
            result[split_length][index - left_index] = '\0';

            for (; origin[index] == symbol && index < length; index++) {}

            left_index = index;
            split_length++;
        }
    }

    return split_length;
}

void parse_command_type(const char* command_type, ParsedCommand* command) {
    for (int index = 0; index < 6; index++) {
        if (strcmp(command_type, command_type_names[index]) == 0) {
            command->type = index;
            break;
        }
    }
}

void parse_fields(const char* fields, ParsedCommand* command) {
    char tokens[MAX_FIELDS][FIELD_LENGTH + VALUE_LENGTH];

    command->fields_count = tokenize(fields, ',', tokens);

    for (int index = 0; index < command->fields_count; index++) {
        int length = strlen(tokens[index]);
        int equal_index = strcspn(tokens[index], "=");
        if (equal_index == length) {
            command->fields[index].has_value = false;
            equal_index = length;
        }

        else command->fields[index].has_value = true;

        strncpy(command->fields[index].field, tokens[index], equal_index);
        command->fields[index].field[equal_index] = '\0';
        strncpy(command->fields[index].value, tokens[index] + equal_index + 1, VALUE_LENGTH);
    }
}

void parse_condition(const char* condition, Condition* parsed) {
    int left_operator_border = -1;
    int right_operator_border = -1;

    for (int index = 0; index < strlen(condition); index++) {
        if (strchr("><!=/", condition[index])) {
            left_operator_border = left_operator_border == -1
                ? index
                : left_operator_border;

            right_operator_border = index;
        }
    }

    if (left_operator_border == -1)
        return;

    strncpy(parsed->field, condition, left_operator_border);
    parsed->field[left_operator_border] = '\0';
    strncpy(parsed->operator, condition + left_operator_border, right_operator_border - left_operator_border + 1);
    parsed->operator[right_operator_border - left_operator_border + 1] = '\0';
    strncpy(parsed->value, condition + right_operator_border + 1, VALUE_LENGTH);
}

int parse_command(const char* command_line, ParsedCommand* command) {
    char tokens[MAX_TOKENS][FIELD_LENGTH + VALUE_LENGTH];
    int length = tokenize(command_line, ' ', tokens);

    parse_command_type(tokens[0], command);
    int has_fields = command->type != Delete;

    if (has_fields)
        parse_fields(tokens[1], command);

    for (int index = 1 + has_fields; index < length; index++) {
        parse_condition(tokens[index], &command->conditions[index - 1 - has_fields]);
        command->conditions_count++;
    }

    return 0;
}