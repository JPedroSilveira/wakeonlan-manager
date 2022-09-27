#ifndef MACHINES_TABLE_REPLICATOR_RECEIVER_HEADER
#define MACHINES_TABLE_REPLICATOR_RECEIVER_HEADER

/**************************************************
 * Receives machines table update packets and 
 * save the updated list of member on application 
 * state
 **************************************************/

#include <thread>
#include <iostream>
#include <chrono>
#include <list>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sstream>
#include <algorithm>

#include "../../utils/alive-utils/alive-utils.h"
#include "../../entity/state/state.h"
#include "../../entity/member/member.h"
#include "../../constants/network-port-constants/network-port-constants.h"
#include "../../constants/network-packet-constants/network-packet-constants.h"
#include "../../exception/fatal-error-exception/fatal-error-exception.h"
#include "../../exception/not-alive/not-alive-exception.h"

void MachinesTableReplicatorReceiverProcess(State* state);

#endif