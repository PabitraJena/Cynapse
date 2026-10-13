#pragma once

#include <stdlib.h>

static float rand_float(){
    return ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
}