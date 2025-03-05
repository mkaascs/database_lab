#ifndef DB_RECORDS_H
#define DB_RECORDS_H

#include <time.h>

#define NAME_LENGTH 256

typedef enum {
    Running, Ready, Paused, Blocked, Dying, Sleeping, Undefined
} Status;

typedef struct {
    int pid;
    char name[NAME_LENGTH];
    int priority;
    struct tm kern_tm;
    struct tm file_tm;
    float cpu_usage;
    Status status;
} ProcessInfo;

typedef struct Node {
    ProcessInfo data;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    size_t length;
} Database;

void init_database(Database*);

#endif //DB_RECORDS_H
