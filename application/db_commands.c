#include <string.h>

#include "../memory/stats.h"
#include "db_commands.h"

#include <stdio.h>

void insert_command(Node* database, ParsedCommand command) {
    if (command.type != Insert)
        return;
}