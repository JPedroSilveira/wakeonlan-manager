#ifndef ELECTION_LISTENER_HEADER
#define ELECTION_LISTENER_HEADER

/**************************************************
 * Starts an election when the number of erros
 * trying to contact the manager reachs an threshold
 **************************************************/

#include "../../entity/state/state.h"
#include "../../utils/alive-utils/alive-utils.h"
#include "../../constants/network-port-constants/network-port-constants.h"
#include "../../constants/election-constants/election-constants.h"

void ElectionListenerProcess(State* state);

#endif