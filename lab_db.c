#include <stdio.h>
#include "application/domain/models.h"

void print(char* line) {
    printf("%s\n", line);
}

int main() {
    Database database;
    init_database(&database);

    execute_command(&database, "insert kern_tm='23:05:59',cpu_usage=7.01,status='paused',file_tm='18:42:30',name=\"Cisco \"Control\",v.1.01\",priority=00000000000052,pid=57", print);
    execute_command(&database, "insert kern_tm='23:05:59',cpu_usage=7.01,status='dying',file_tm='18:42:00',name=\"Cisco \"Control\",v.1.01\",priority=-00000000000051,pid=59", print);
    execute_command(&database, "insert kern_tm='23:05:59',cpu_usage=7.01,status='ready',file_tm='18:42:00',name=\"ZOV GOIDA\",priority=-00000000000054,pid=563", print);
    execute_command(&database, "insert kern_tm='23:05:59',cpu_usage=7.01,status='blocked',file_tm='18:42:00',name=\"SEMYCH, MIZINA\",priority=-00000000000051,pid=59", print);
    execute_command(&database, "select pid,cpu_usage,name,status file_tm>='18:42:00' priority<0", print);
    execute_command(&database, "uniq pid,priority", print);
    execute_command(&database, "delete priority>0 cpu_usage>=10", print);
    execute_command(&database, "update status='sleeping',pid=28 priority<0", print);
    execute_command(&database, "select status,priority,pid", print);
}
