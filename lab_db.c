#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "application/commands.h"
#include "application/parsing/parser.h"

void print_command(const ParsedCommand *cmd) {
    printf("Command type: %d\n", cmd->type);

    printf("Fields:\n");
    for (int i = 0; i < cmd->fields_count; i++) {
        printf("  %s = %s\n", cmd->fields[i].field,
               cmd->fields[i].has_value ? cmd->fields[i].value : "(no value)");
    }

    printf("Conditions:\n");
    for (int i = 0; i < cmd->conditions_count; i++) {
        printf("  %s %s %s\n", cmd->conditions[i].field,
               cmd->conditions[i].operator, cmd->conditions[i].value);
    }
}

void print(char* line) {
    printf("%s\n", line);
}

int main() {
    const char *commands[] = {
        "update   kern_tm='18:40:52',,,priority=15, priority<=004 status/not_in/['paused'] pid<=15",
        "sort pid=desc,file_tm=desc,cpu_usage=asc",
        "insert    kern_tm='23:05:59',cpu_usage=7.01,status='paused',file_tm='18:42:00',name=\"Cisco \"Control\",v.1.01\",priority=-00000000000051,pid=57",
        "select          pid,cpu_usage,name,status,priority",
        "uniq pid,name"
    };

    Database database;
    init_database(&database);

    ParsedCommand cmd;
    parse_command(commands[2], &cmd);
    insert_command(&database, cmd, print);
    insert_command(&database, cmd, print);

    ParsedCommand cmd2;
    parse_command(commands[3], &cmd2);
    select_command(&database, cmd2, print);

    ParsedCommand cmd3;
    parse_command(commands[4], &cmd3);
    uniq_command(&database, cmd3, print);
}
