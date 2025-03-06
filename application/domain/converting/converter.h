#ifndef CONVERTER_H
#define CONVERTER_H

#include <time.h>

#include "../models.h"
#include "../../bool.h"

bool string_to_int(const char*, int*);
bool string_to_time(const char*, struct tm*);
bool string_to_float(const char*, float*);
bool string_to_status(const char*, Status*);

#endif //CONVERTER_H
