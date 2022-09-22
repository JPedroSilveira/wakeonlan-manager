#ifndef MANAGER_LIFE_MONITOR_LISTENER_HEADER
#define MANAGER_LIFE_MONITOR_LISTENER_HEADER

/**************************************************
 * Listen and answer manager life monitor packets
 **************************************************/

#include <netdb.h>

#include "../../entity/state/state.h"
#include "../../utils/alive-utils/alive-utils.h"
#include "../../constants/network-port-constants/network-port-constants.h"
#include "../../constants/network-packet-constants/network-packet-constants.h"
#include "../../exception/fatal-error-exception/fatal-error-exception.h"
#include "../../exception/not-alive/not-alive-exception.h"

void ManagerLifeMonitorListenerProcess(State* state);

#endif
