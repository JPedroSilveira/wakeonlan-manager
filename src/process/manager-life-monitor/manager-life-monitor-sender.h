#ifndef MANAGER_LIFE_MONITOR_SENDER_HEADER
#define MANAGER_LIFE_MONITOR_SENDER_HEADER

/**************************************************
 * Send manager life monitor packets and
 * increase the counter of how many times
 * manager did not respond to the request
 * in sequence
 **************************************************/

#include <netdb.h>

#include "../../entity/state/state.h"
#include "../../utils/alive-utils/alive-utils.h"
#include "../../constants/network-port-constants/network-port-constants.h"
#include "../../constants/network-packet-constants/network-packet-constants.h"
#include "../../exception/fatal-error-exception/fatal-error-exception.h"
#include "../../exception/not-alive/not-alive-exception.h"

void ManagerLifeMonitorSenderProcess(State* state);

#endif
