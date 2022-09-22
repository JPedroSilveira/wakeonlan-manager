#ifndef ELECTION_STARTER_HEADER
#define ELECTION_STARTER_HEADER

/**************************************************
 * Starts an election when the number of erros
 * trying to contact the manager reachs an threshold
 **************************************************/

#include <stdlib.h>
#include <string.h>

#include "../../utils/alive-utils/alive-utils.h"
#include "../../entity/state/state.h"
#include "../../utils/print-utils/print-utils.h"
#include "../../constants/election-constants/election-constants.h"
#include "../../exception/fatal-error-exception/fatal-error-exception.h"
#include "../../exception/not-alive/not-alive-exception.h"

void ElectionStarterProcess(State* state);

#endif