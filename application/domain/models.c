#include "models.h"

void init_database(Database* database) {
    database->head = NULL;
    database->tail = NULL;
    database->length = 0;
}