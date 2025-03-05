#ifndef DB_COMMANDS_H
#define DB_COMMANDS_H

#include "domain/models.h"
#include "parsing/parser.h"

int insert_command(Database*, ParsedCommand);
void select_command(Node*, ParsedCommand);
void delete_records(Node*, ParsedCommand);
void update_records(Node*, ParsedCommand);
void uniq_records(Node*, ParsedCommand);
void sort_records(Node*, ParsedCommand);

#endif //DB_COMMANDS_H
