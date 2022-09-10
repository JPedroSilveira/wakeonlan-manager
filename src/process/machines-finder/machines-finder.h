#ifndef MACHINES_FINDER_HEADER
#define MACHINES_FINDER_HEADER

/**************************************************
 * If manager: find out machines on local network
 * sending is-member packets
 * 
 * Else: answer is-member packets to let manager
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

void MachinesFinderProcess(State* state);

#endif
