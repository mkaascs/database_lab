#include "models.h"

#include <stdio.h>
#include <string.h>

#include "../commands.h"
#include "../parsing/parser.h"

#define MAX_INCORRECT_COMMAND_LENGTH 20

const char* const status_string[STATUS_COUNT] = {
    "running", "ready", "paused", "blocked", "dying", "sleeping"
};

const char* const fields_string[FIELDS_COUNT] = {
    "pid", "name", "priority", "kern_tm", "file_tm", "cpu_usage", "status"
};

void init_database(Database* database) {
    database->head = NULL;
    database->tail = NULL;
    database->length = 0;
}

void print_incorrect_command(const char* command, void (*presenter)(char*)) {
    char cropped_command[MAX_INCORRECT_COMMAND_LENGTH + 1];
    snprintf(cropped_command, sizeof cropped_command, "%s", command);

    char incorrect_command_line[35];
    snprintf(incorrect_command_line, sizeof incorrect_command_line, "incorrect:'%s'", cropped_command);
    presenter(incorrect_command_line);
}

int execute_command(Database* database, const char* command, void (*presenter)(char*)) {
    ParsedCommand parsed_command;
    memset(&parsed_command, 0, sizeof(ParsedCommand));
    int result = parse_command(command, &parsed_command);

    if (parsed_command.type == Insert)
        result = insert_command(database, parsed_command, presenter);

    if (parsed_command.type == Select)
        result = select_command(database, parsed_command, presenter);

    if (parsed_command.type == Uniq)
        result = uniq_command(database, parsed_command, presenter);

    if (result == -1) {
        print_incorrect_command(command, presenter);
        return -1;
    }

    return 0;
}