#ifndef DB_COMMANDS_H
#define DB_COMMANDS_H

#include "domain/models.h"
#include "parsing/parser.h"

int insert_command(Database*, ParsedCommand, void (*)(char*));
int select_command(Database*, ParsedCommand, void (*)(char*));
void delete_records(Node*, ParsedCommand);
void update_records(Node*, ParsedCommand);
void uniq_records(Node*, ParsedCommand);
void sort_records(Node*, ParsedCommand);

#endif //DB_COMMANDS_H
