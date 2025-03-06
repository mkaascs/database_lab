#include <stdio.h>
#include <stdlib.h>

#include "../memory/stats.h"
#include "operations/conditions/conditions.h"
#include "operations/fields.h"
#include "commands.h"

#define MAX_FIELDS_COUNT 7

int insert_command(Database* database, ParsedCommand command, void (*presenter)(char*)) {
    if (command.type != Insert)
        return -1;

    if (command.fields_count != MAX_FIELDS_COUNT)
        return -1;

    ProcessInfo record;
    if (!set_values(command, &record))
        return -1;

    Node* node = (Node*)track_malloc(sizeof(Node));

    node->data = record;
    node->next = NULL;

    if (database->length == 0) {
        database->head = node;
        database->tail = node;
    }

    else {
        database->tail->next = node;
        database->tail = node;
    }

    database->length++;

    char buffer[32];
    sprintf(buffer, "insert:%d", database->length);
    presenter(buffer);
    return 0;
}

int select_command(Database* database, ParsedCommand command, void (*presenter)(char*)) {
    if (command.type != Select)
        return -1;

    Node* current = database->head;
    while (current != NULL) {
        bool is_match = false;
        for (int index = 0; index < command.conditions_count; index++) {
            const int code = match(command.conditions[index], current->data, &is_match);
            if (code == -1)
                return -1;

            if (!is_match)
                break;
        }

        if (!is_match)
            continue;
    }

    return 0;
}