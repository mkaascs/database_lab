#ifndef DB_RECORDS_H
#define DB_RECORDS_H

#define NAME_LENGTH 256
#define TIME_LENGTH 20

typedef enum {
    Running, Ready, Paused, Blocked, Dying, Sleeping, Undefined
} Status;

typedef struct {
    int pid;
    char name[NAME_LENGTH];
    int priority;
    char kern_tm[TIME_LENGTH];
    char file_tm[TIME_LENGTH];
    float cpu_usage;
    Status status;
} Record;

typedef struct {
    Record* data;
    struct Node* next;
} Node;

#endif //DB_RECORDS_H
