#ifndef MONITOR_HEADER
#define MONITOR_HEADER

/**************************************************
 * Constantly does requests do update connected
 * machines status.
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

void MonitorProcess(State* state);

#endif
