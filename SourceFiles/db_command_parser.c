#include "../HeaderFiles/db_command_parser.h"
#include <string.h>
#include <stdio.h>

#define FIELD_LENGTH 20
#define VALUE_LENGTH 50
#define ORDER_LENGTH 10

#define MAX_FIELDS 10
#define MAX_CONDITIONS 10

char* tokenize(char** origin, const char* delim) {
    char *token = strsep(origin, delim);
    return token && *token
        ? token
        : NULL;
}

char* skip_spaces(char* str) {
    while (*str == ' ') str++;
    return str;
}

int is_condition(const char* token) {
    return strpbrk(token, "=!<>/") != NULL;
}

int parse_command(const char* command_line, ParsedCommand* command) {
    char buffer[256];
    strncpy(buffer, command_line, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    char* pointer = buffer;
    char* command_name = tokenize(&pointer, " ");
    if (!command_name) return -1;

    pointer = skip_spaces(pointer);

    if (strcmp(command_name, "insert") == 0)
        command->type = Insert;

    else if (strcmp(command_name, "select") == 0)
        command->type = Select;

    else if (strcmp(command_name, "delete") == 0)
        command->type = Delete;

    else if (strcmp(command_name, "update") == 0)
        command->type = Update;

    else if (strcmp(command_name, "uniq") == 0)
        command->type = Uniq;

    else if (strcmp(command_name, "sort") == 0)
        command->type = Sort;

    else return -1;

    command->fields_count = 0;
    command->conditions_count = 0;
    command->sort_fields_count = 0;

    int parsing_conditions = 0;
    if (command->type == Update) {
        char* conditions_start = strstr(pointer, " ");
        if (conditions_start) {
            *conditions_start = '\0';
            conditions_start = skip_spaces(conditions_start + 1);
        }

        char* field_ptr = pointer;
        while (field_ptr) {
            char *field_token = tokenize(&field_ptr, ",");
            if (!field_token) break;

            char *equal = strchr(field_token, '=');
            if (!equal) continue;

            *equal = '\0';
            strncpy(command->fields[command->fields_count].field, field_token, FIELD_LENGTH - 1);
            strncpy(command->fields[command->fields_count].value, equal + 1, VALUE_LENGTH - 1);
            command->fields[command->fields_count].has_value = 1;
            command->fields_count++;
        }

        if (conditions_start) {
            pointer = conditions_start;
            parsing_conditions = 1;
        }
    }

    while (pointer) {
        char* token = tokenize(&pointer, ", ");
        if (!token) break;
        token = skip_spaces(token);

        if (!parsing_conditions) {
            if (command->type == Insert || command->type == Update) {
                char* equal = strchr(token, '=');
                if (!equal) continue;

                *equal = '\0';
                strncpy(command->fields[command->fields_count].field, token, FIELD_LENGTH - 1);
                strncpy(command->fields[command->fields_count].value, equal + 1, VALUE_LENGTH - 1);
                command->fields[command->fields_count].has_value = 1;
                command->fields_count++;
            }

            else if (command->type == Select || command->type == Uniq) {
                strncpy(command->fields[command->fields_count].field, token, FIELD_LENGTH - 1);
                command->fields[command->fields_count].has_value = 0;
                command->fields_count++;
            }

            else if (command->type == Sort) {
                char* equal = strchr(token, '=');
                if (!equal) continue;

                *equal = '\0';
                strncpy(command->sort_fields[command->sort_fields_count].field, token, FIELD_LENGTH - 1);
                strncpy(command->sort_fields[command->sort_fields_count].order, equal + 1, ORDER_LENGTH - 1);
                command->sort_fields_count++;
            }
        }

        else {
            char* operator_position = strpbrk(token, "=!<>/");
            if (!operator_position) continue;

            char *last_slash = strrchr(operator_position, '/');

            strncpy(command->conditions[command->conditions_count].field, token, operator_position - token);
            command->conditions[command->conditions_count].field[operator_position - token] = '\0';

            if (last_slash) {
                strncpy(command->conditions[command->conditions_count].operator, operator_position, last_slash - operator_position + 1);
                command->conditions[command->conditions_count].operator[last_slash - operator_position + 1] = '\0';

                strncpy(command->conditions[command->conditions_count].value, last_slash + 1, VALUE_LENGTH - 1);

            } else {
                char* operator_end = operator_position +
                    (operator_position[1] == '=' || operator_position[0] == operator_position[1] ? 2 : 1);

                strncpy(command->conditions[command->conditions_count].operator, operator_position, operator_end - operator_position);
                command->conditions[command->conditions_count].operator[operator_end - operator_position] = '\0';

                strncpy(command->conditions[command->conditions_count].value, operator_end, VALUE_LENGTH - 1);
            }

            command->conditions_count++;
        }
    }

    return 0;
}