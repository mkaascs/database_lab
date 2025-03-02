#ifndef DB_COMMANDS_H
#define DB_COMMANDS_H
#include "db_records.h"
#include "db_command_parser.h"

void insert_command(Database*, ParsedCommand);
void select_command(Database*, ParsedCommand);
void delete_records(Database*, ParsedCommand);
void update_records(Database*, ParsedCommand);
void uniq_records(Database*, ParsedCommand);
void sort_records(Database*, ParsedCommand);

#endif //DB_COMMANDS_H
