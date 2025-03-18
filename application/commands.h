#ifndef DB_COMMANDS_H
#define DB_COMMANDS_H

#include "domain/models.h"
#include "parsing/parser.h"

int insert_command(Database*, ParsedCommand, void (*)(char*));
int select_command(const Database*, ParsedCommand, void (*)(char*));
int delete_command(Database*, ParsedCommand, void (*)(char*));
int update_command(const Database*, ParsedCommand, void (*)(char*));
int uniq_command(Database*, ParsedCommand, void (*)(char*));
int sort_command(Database*, ParsedCommand, void (*)(char*));

#endif //DB_COMMANDS_H
