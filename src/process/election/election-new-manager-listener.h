#ifndef ELECTION_NEW_MANAGER_LISTENER_HEADER
#define ELECTION_NEW_MANAGER_LISTENER_HEADER

/**************************************************
 * Await for new manager packets
 **************************************************/

#include "../../entity/state/state.h"
#include "../../utils/alive-utils/alive-utils.h"
#include "../../constants/network-port-constants/network-port-constants.h"
#include "../../constants/network-packet-constants/network-packet-constants.h"
#include "../../constants/election-constants/election-constants.h"
#include "../../exception/fatal-error-exception/fatal-error-exception.h"
#include "../../exception/not-alive/not-alive-exception.h"

void ElectionNewManagerListenerProcess(State* state);

#endif