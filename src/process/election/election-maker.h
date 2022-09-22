#ifndef ELECTION_MAKER_HEADER
#define ELECTION_MAKER_HEADER

/**************************************************
 * Make an election following the bully algorithm 
 * 
 * Send vote request packets to find out if should
 * be the new manager
 * If new manager: trigger event to send 
 * new manager packets
 * Else: await new manager packets to finish the
 * election
 * 
 * After a certain time if the election was 
 * not completed by one of these two events 
 * it will try everything again
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
#include "../../entity/state/state.h"
#include "../../utils/print-utils/print-utils.h"
#include "../../constants/network-port-constants/network-port-constants.h"
#include "../../constants/network-packet-constants/network-packet-constants.h"
#include "../../constants/election-constants/election-constants.h"
#include "../../exception/election-exception/election-exception.h"
#include "../../exception/fatal-error-exception/fatal-error-exception.h"
#include "../../exception/not-alive/not-alive-exception.h"

void ElectionMakerProcess(State* state);

#endif