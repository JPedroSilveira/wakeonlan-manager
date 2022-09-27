#include "alive-utils.h"

void throwExceptionIfNotAlive(State* state) {
    if (!state->isAlive()) {
        throw NotAliveException();
    }
}