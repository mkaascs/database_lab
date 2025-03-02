#ifndef DB_COMMAND_PARSER_H
#define DB_COMMAND_PARSER_H

#define FIELD_LENGTH 20
#define VALUE_LENGTH 50
#define ORDER_LENGTH 5

#define MAX_FIELDS 10
#define MAX_CONDITIONS 10
#define MAX_SORT_FIELDS 10

typedef enum {
    Insert, Select, Update, Delete, Uniq, Sort
} CommandType;

typedef struct {
    char field[FIELD_LENGTH];
    char value[VALUE_LENGTH];
    short has_value;
} Field;

typedef struct {
    char field[FIELD_LENGTH];
    char operator[10];
    char value[VALUE_LENGTH];
} Condition;

typedef struct {
    char field[FIELD_LENGTH];
    char order[ORDER_LENGTH];
} SortField;

typedef struct {
    CommandType type;

    Field fields[MAX_FIELDS];
    short fields_count;

    Condition conditions[MAX_CONDITIONS];
    short conditions_count;

    SortField sort_fields[MAX_SORT_FIELDS];
    short sort_fields_count;
} ParsedCommand;

int parse_command(const char*, ParsedCommand*);

#endif //DB_COMMAND_PARSER_H
