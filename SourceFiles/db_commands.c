#include <string.h>

#include "../HeaderFiles/db_stats.h"
#include "../HeaderFiles/db_commands.h"

#include <stdio.h>

void insert_command(Database* db, ParsedCommand command) {
    if (command.type != Insert)
        return;
}