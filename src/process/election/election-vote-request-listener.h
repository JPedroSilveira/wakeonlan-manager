#ifndef ELECTION_VOTE_REQUEST_LISTENER_HEADER
#define ELECTION_VOTE_REQUEST_LISTENER_HEADER

/**************************************************
 * Await for vote requests packtes and answer then
 * based on process pid
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
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <thread>
#include <chrono>

#include "../../entity/state/state.h"
#include "../../utils/alive-utils/alive-utils.h"
#include "../../constants/network-port-constants/network-port-constants.h"
#include "../../constants/network-packet-constants/network-packet-constants.h"
#include "../../constants/election-constants/election-constants.h"
#include "../../exception/fatal-error-exception/fatal-error-exception.h"
#include "../../exception/not-alive/not-alive-exception.h"

void ElectionVoteRequestListenerProcess(State* state);

#endif
