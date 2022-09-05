#ifndef REPLIER_HEADER
#define REPLIER_HEADER

/**************************************************
 * Monitor members and send them to all
 * machines in the wakeonlan network
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

void ReplierProcess(State* state);

#endif