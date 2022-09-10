#ifndef MACHINES_TABLE_REPLIER_HEADER
#define MACHINES_TABLE_REPLIER_HEADER

/**************************************************
 * If manager: send members-replier packages 
 * to all machines with the updated members table, 
 * allowing this information to be replicated in 
 * all of members
 * 
 * Else: receives members-replier packages and 
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

void MachinesTableReplierProcess(State* state);

#endif