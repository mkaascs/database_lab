#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HeaderFiles/db_command_parser.h"

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

int main() {
    /*
    const char *commands[] = {
        "sort pid=desc,file_tm=desc,cpu_usage=asc",
        "uniq file_tm,cpu_usage,status",
        "update   priority=15,kern_tm='18:40:52' priority<=004 status/not_in/['paused'] pid<=15",
        "select kern_tm,name kern_tm!='18:42:00' file_tm<'18:40:52' name>\"alert\" name!=\"alert\"",
        "delete    name<=\"halt\" status=='running' file_tm=='23:59:59'"
    };

    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        ParsedCommand cmd;
        if (parse_command(commands[i], &cmd) == 0) {
            print_command(&cmd);
            printf("\n");
        } else {
            printf("Error parsing: %s\n", commands[i]);
        }
    }
    */
}
