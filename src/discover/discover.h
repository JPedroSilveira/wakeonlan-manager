#ifndef DISCOVER_HEADER
#define DISCOVER_HEADER

/**************************************************
 * Discover machines on the local network.
 * - Send requests to let other machines know that
 * this one is connected.
 * Do requests to find other machines.
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

#include "../output/output.h"
#include "../model/state/state.h"

void DiscoverProcess(State* state);

#endif
