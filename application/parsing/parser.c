#include "../parsing/parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../memory/stats.h"

const char *const command_type_names[] = {
    "insert", "select", "update", "delete", "uniq", "sort"
};

char* tokenize(char* origin, const char symbol, char** save_pointer) {
    if (origin) *save_pointer = origin;
    else if (!*save_pointer || **save_pointer == '\0') return NULL;

    char* start = *save_pointer;
    while (*start && *start == symbol) start++;

    if (*start == '\0') return NULL;

    int in_quotation = 0;
    char quote_symbol = 0;

    char* token = start;
    while (**save_pointer) {
        int backslash_count = 0;
        char* check_pointer = *save_pointer - 1;

        while (check_pointer >= token && *check_pointer == '\\') {
            backslash_count++;
            check_pointer--;
        }

        int is_escaped = backslash_count % 2 == 1;
        if ((**save_pointer == '"' || **save_pointer == '\'') && !is_escaped) {
            if (!in_quotation) {
                in_quotation = 1;
                quote_symbol = **save_pointer;
            }

            else if (quote_symbol == **save_pointer)
                in_quotation = 0;
        }

        else if (!in_quotation && **save_pointer == symbol) {
            **save_pointer = '\0';
            (*save_pointer)++;

            while (**save_pointer && **save_pointer == symbol) (*save_pointer)++;

            return token;
        }

        (*save_pointer)++;
    }

    *save_pointer = NULL;
    return token;
}

int parse_command_type(const char* command_type, ParsedCommand* command) {
    for (int index = 0; index < 6; index++) {
        if (strcmp(command_type, command_type_names[index]) == 0) {
            command->type = index;
            return 0;
        }
    }

    return -1;
}

void parse_fields(const char* fields, ParsedCommand* command) {
    const size_t fields_length = strlen(fields);
    char* fields_copy = (char*)track_malloc(fields_length + 1);
    strncpy(fields_copy, fields, fields_length + 1);

    char* save_pointer;
    char* token = tokenize(fields_copy, ',', &save_pointer);

    while (token != NULL) {
        char* equal = strchr(token, '=');
        if (equal == NULL) {
            equal = token + strlen(token) - 1;
            *(equal + 1) = '\0';
        }

        else *equal = '\0';

        char* field_name = token;
        char* value = equal + 1;
        strncpy(command->fields[command->fields_count].field, field_name, FIELD_LENGTH);

        if (strlen(value) > 0) {
            strncpy(command->fields[command->fields_count].value, value, VALUE_LENGTH);
            command->fields[command->fields_count].has_value = 1;
        }

        else command->fields[command->fields_count].has_value = 0;

        command->fields_count++;
        token = tokenize(NULL, ',', &save_pointer);
    }

    track_free(fields_copy);
}

int parse_condition(const char* condition, Condition* parsed) {
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
        return -1;

    strncpy(parsed->field, condition, left_operator_border);
    parsed->field[left_operator_border] = '\0';
    strncpy(parsed->operator, condition + left_operator_border, right_operator_border - left_operator_border + 1);
    parsed->operator[right_operator_border - left_operator_border + 1] = '\0';
    strncpy(parsed->value, condition + right_operator_border + 1, VALUE_LENGTH);
    return 0;
}

int parse_command(const char* command_line, ParsedCommand* command) {
    const int command_length = strlen(command_line);
    char* command_copy = (char*)track_malloc(command_length + 1);
    strncpy(command_copy, command_line, command_length + 1);

    char* save_pointer;
    char* token = tokenize(command_copy, ' ', &save_pointer);

    if (parse_command_type(token, command) == -1) {
        track_free(command_copy);
        return -1;
    }

    int has_fields = command->type != Delete;
    token = tokenize(NULL, ' ', &save_pointer);

    if (has_fields) {
        parse_fields(token, command);
        token = tokenize(NULL, ' ', &save_pointer);
    }

    while (token != NULL) {
        if (parse_condition(token, &command->conditions[command->conditions_count++]) == -1) {
            track_free(command_copy);
            return -1;
        }

        token = tokenize(NULL, ' ', &save_pointer);
    }

    track_free(command_copy);

    return 0;
}