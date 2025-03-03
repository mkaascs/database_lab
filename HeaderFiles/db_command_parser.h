#ifndef DB_COMMAND_PARSER_H
#define DB_COMMAND_PARSER_H

#define FIELD_LENGTH 10
#define OPERATOR_LENGTH 10
#define VALUE_LENGTH 30

#define MAX_FIELDS 10
#define MAX_CONDITIONS 10

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
    char operator[OPERATOR_LENGTH];
    char value[VALUE_LENGTH];
} Condition;

typedef struct {
    CommandType type;

    Field fields[MAX_FIELDS];
    short fields_count;

    Condition conditions[MAX_CONDITIONS];
    short conditions_count;
} ParsedCommand;

int parse_command(const char*, ParsedCommand*);

#endif //DB_COMMAND_PARSER_H
