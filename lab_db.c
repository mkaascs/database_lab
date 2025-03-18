#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "application/domain/models.h"
#include "memory/stats.h"

#define INPUT_FILENAME "input.txt"
#define OUTPUT_FILENAME "output.txt"
#define MEMSTAT_FILENAME "memstat.txt"
#define MAX_LINE_LENGTH 256

void write_line(char* line) {
    FILE* output_file = fopen(OUTPUT_FILENAME, "a");
    if (output_file == NULL)
        return;

    fprintf(output_file, "%s\n", line);
    fclose(output_file);
}
void dont_write_line(char* line) {}

int main() {
    FILE* input_file = fopen(INPUT_FILENAME, "r");
    if (input_file == NULL)
        return EXIT_FAILURE;

    FILE* output_file = fopen(OUTPUT_FILENAME, "w");
    if (output_file == NULL) {
        free(input_file);
        return EXIT_FAILURE;
    }

    fclose(output_file);

    Database database;
    init_database(&database);

    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, sizeof buffer, input_file)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        execute_command(&database, buffer, write_line);
    }

    fclose(input_file);
    execute_command(&database, "delete", dont_write_line);

    FILE* memstat_file = fopen(MEMSTAT_FILENAME, "w");
    if (memstat_file == NULL)
        return EXIT_FAILURE;

    DataStats memstat;
    get_memstat(&memstat);

    fprintf(memstat_file, "malloc:%lu\nrealloc:%lu\ncalloc:%lu\nfree:%lu",
        memstat.malloc_state, memstat.realloc_state, memstat.calloc_state, memstat.free_count);

    fclose(memstat_file);
    return 0;
}
