#ifndef MACHINE_MANAGER_HEADER
#define MACHINE_MANAGER_HEADER

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
#include "../output/output.h"

Member getMachineInfo();

#endif
