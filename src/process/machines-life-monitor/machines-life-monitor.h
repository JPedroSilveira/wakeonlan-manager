#ifndef MACHINES_LIFE_MONITOR_HEADER
#define MACHINES_LIFE_MONITOR_HEADER

/**************************************************
 * If manager: send messages to all member machines
 * to check if they are awake and update the state
 * with the results
 * 
 * Else: receives is-awake packets and answer them
 **************************************************/

#include <iostream>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <errno.h>
#include <ctype.h>
#include <thread>
#include <chrono>

#include "../../utils/alive-utils/alive-utils.h"
#include "../../utils/print-utils/print-utils.h"
#include "../../entity/state/state.h"
#include "../../constants/network-port-constants/network-port-constants.h"

void MachinesLifeMonitorProcess(State* state);

#endif
