#ifndef CONVERTER_H
#define CONVERTER_H

#include <time.h>

#include "../models.h"

void trim_quotation(const char*, char*);
int string_to_int(const char*, int*);
int string_to_time(const char*, struct tm*);
int string_to_float(const char*, float*);
int string_to_status(const char*, Status*);

#endif //CONVERTER_H
