#ifndef ELECTION_HEADER
#define ELECTION_HEADER

/**************************************************
 * Starts an election when the number of erros
 * trying to contact the manager reachs an threshold
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
#include "../../constants/election-constants/election-constants.h"
#include "../../exception/election-exception/election-exception.h"

void ElectionProcess(State *state);

#endif