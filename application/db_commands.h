#ifndef DB_COMMANDS_H
#define DB_COMMANDS_H

#include "domain/db_records.h"
#include "../parsing/db_command_parser.h"

void insert_command(Node*, ParsedCommand);
void select_command(Node*, ParsedCommand);
void delete_records(Node*, ParsedCommand);
void update_records(Node*, ParsedCommand);
void uniq_records(Node*, ParsedCommand);
void sort_records(Node*, ParsedCommand);

#endif //DB_COMMANDS_H
