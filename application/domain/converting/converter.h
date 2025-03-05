#ifndef CONVERTER_H
#define CONVERTER_H

#include <time.h>
#include "../models.h"

typedef int bool;
#define true 1
#define false 0

bool string_to_int(const char*, int*);
bool string_to_time(const char*, struct tm*);
bool string_to_float(const char*, float*);
bool string_to_status(const char*, Status*);

#endif //CONVERTER_H
