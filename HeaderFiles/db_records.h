#ifndef DB_RECORDS_H
#define DB_RECORDS_H
#include <sys/_types/_size_t.h>

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
    size_t count;
    size_t capacity;
} Database;

#endif //DB_RECORDS_H
