#include "../HeaderFiles/db_command_parser.h"

#include <stdlib.h>
#include <string.h>

typedef int bool;
#define true 1
#define false 0

#define MAX_TOKENS 20

#define FIELD_LENGTH 20
#define VALUE_LENGTH 50

const char *const command_type_names[] = {
    "insert", "select", "update", "delete", "uniq", "sort"
};

int tokenize_command(char* command, char symbol, char** result) {
    int split_length = 0;
    int left_index = 0;
    bool in_quotation = false;

    for (int index = 0; index <= strlen(command); index++) {
        if (command[index] == '\"' || command[index] == '\'')
            in_quotation = !in_quotation;

        if ((command[index] == symbol && !in_quotation) || index == strlen(command)) {
            strncpy(result[split_length], command + left_index, index - left_index);
            result[split_length][index - left_index] = '\0';

            for (; command[index] == symbol; index++) {} // skip all symbols

            left_index = index;
            split_length++;
        }
    }

    return split_length;
}

int parse_command(const char* command_line, ParsedCommand* command) {
    char** tokens = malloc(MAX_TOKENS * sizeof(char*));
    for (int index = 0; index < MAX_TOKENS; index++) {
        tokens[index] = malloc()
    }
    
    return 0;
}