#ifndef MACHINE_INFO_HEADER
#define MACHINE_INFO_HEADER

/**************************************************
 * Collects and provides current machine
 * information.
 **************************************************/

#include <thread>
#include <chrono>
#include <iostream>
#include <unistd.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <tuple>
#include <vector>

#include "../model/member/member.h"
#include "../utils/print-utils/print-utils.h"

Member getMachineInfo();

#endif
