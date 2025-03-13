#include <stdio.h>
#include "application/domain/models.h"

void print(char* line) {
    printf("%s\n", line);
}

int main() {
    const char *commands[] = {
        "update kern_tm='18:40:52',,,priority=15, priority<=004 status/not_in/['paused'] pid<=15",
        "insert kern_tm='23:05:59',cpu_usage=7.01,status='dying',file_tm='18:42:00',name=\"Cisco \"Control\",v.1.01\",priority=-00000000000051,pid=59",
        "insert kern_tm='23:05:59',cpu_usage=7.01,status='paused',file_tm='18:42:00',name=\"Cisco \"Control\",v.1.01\",priority=-00000000000052,pid=57",
        "select pid,cpu_usage,name,status,priority",
        "uniq name,prty"
    };

    Database database;
    init_database(&database);

    execute_command(&database, commands[2], print);
    execute_command(&database, commands[1], print);
    execute_command(&database, commands[3], print);
    execute_command(&database, commands[4], print);
    execute_command(&database, commands[3], print);
}
