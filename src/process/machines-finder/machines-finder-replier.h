#ifndef MACHINES_FINDER_REPLIER_HEADER
#define MACHINES_FINDER_REPLIER_HEADER

/**************************************************
 * Answer is-member packets to let manager
 * knows that the machine want to be part of the
 * members network
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

#include "../../utils/alive-utils/alive-utils.h"
#include "../../utils/print-utils/print-utils.h"
#include "../../entity/state/state.h"
#include "../../constants/network-port-constants/network-port-constants.h"
#include "../../constants/network-package-constants/network-package-constants.h"
#include "../../exception/fatal-error-exception/fatal-error-exception.h"
#include "../../exception/not-alive/not-alive-exception.h"

void MachinesFinderReplierProcess(State* state);

#endif
