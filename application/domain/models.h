#ifndef DB_RECORDS_H
#define DB_RECORDS_H

#include <time.h>

#define NAME_LENGTH 50

#define STATUS_COUNT 6
#define FIELDS_COUNT 7

typedef enum {
    Running, Ready, Paused, Blocked, Dying, Sleeping
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
    struct Node* previous;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    size_t length;
} Database;

const char* const status_string[STATUS_COUNT];
const char* const fields_string[FIELDS_COUNT];

void init_database(Database*);
int execute_command(Database*, const char*, void (*)(char*));

#endif //DB_RECORDS_H
