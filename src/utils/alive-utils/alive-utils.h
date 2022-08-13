#ifndef ALIVE_UTILS_HEADER
#define ALIVE_UTILS_HEADER

#include "../../entity/state/state.h"
#include "../../exception/not-alive/not-alive-exception.h"

/**************************************************
 * Helper to check if process should be alive
 **************************************************/

void throwExceptionIfNotAlive(State* state);

#endif