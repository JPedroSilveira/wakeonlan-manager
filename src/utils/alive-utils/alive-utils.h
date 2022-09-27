#ifndef ALIVE_UTILS_HEADER
#define ALIVE_UTILS_HEADER

/**************************************************
 * Helper to check if process should be alive
 * Used to close the application when 
 * state.isAlive is false.
 **************************************************/

#include "../../entity/state/state.h"
#include "../../exception/not-alive/not-alive-exception.h"

void throwExceptionIfNotAlive(State* state);

#endif