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

int main() {
    const char *commands[] = {
        "update   kern_tm='18:40:52',,,priority=15, priority<=004 status/not_in/['paused'] pid<=15",
        "sort pid=desc,file_tm=desc,cpu_usage=asc",
        "insert kern_tm='23:5:59',cpu_usage=7.0,status='paused',file_tm='18:42:00',name=\"Cisco \"Control\",v.1.01\",priority=-00000000000051,pid=57",
        "select    kern_tm,,,name kern_tm!='18:42:00' file_tm<'18:40:52' name>\"ale    rt\" name!=\"al ert\"",
        "delete priority>41 kern_tm>'18:42:00'"
    };

    /*
    for (int i = 0; i < 5; i++) {
        ParsedCommand cmd;
        memset(&cmd, 0, sizeof(ParsedCommand));
        if (parse_command(commands[i], &cmd) == 0) {
            print_command(&cmd);
            printf("\n");
        } else {
            printf("Error parsing: %s\n", commands[i]);
        }
    }
    */

    Database database;
    init_database(&database);

    ParsedCommand cmd;
    parse_command(commands[2], &cmd);
    insert_command(&database, cmd);

    printf("%lu\n", database.length);
    printf("cpu: %f\npid: %d\npriority: %d\nname: %s\nstatus: %d\nkern: %d:%d:%d\nfile: %d:%d:%d", database.head->data.cpu_usage, database.head->data.pid, database.head->data.priority, database.head->data.name, database.head->data.status,
        database.head->data.kern_tm.tm_hour, database.head->data.kern_tm.tm_min, database.head->data.kern_tm.tm_sec,
        database.head->data.file_tm.tm_hour, database.head->data.file_tm.tm_min, database.head->data.file_tm.tm_sec);
}
