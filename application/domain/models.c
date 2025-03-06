#include "models.h"

const char* const status_string[STATUS_COUNT] = {
    "running", "ready", "paused", "blocked", "dying", "sleeping"
};

const char* const fields_string[FIELDS_COUNT] = {
    "pid", "name", "priority", "kern_tm", "file_tm", "cpu_usage", "status"
};

void init_database(Database* database) {
    database->head = NULL;
    database->tail = NULL;
    database->length = 0;
}