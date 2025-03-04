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
} Record;

typedef struct {
    Record* data;
    struct Node* next;
} Node;

#endif //DB_RECORDS_H
