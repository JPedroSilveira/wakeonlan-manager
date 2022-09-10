#ifndef MACHINE_INFO_UTILS_HEADER
#define MACHINE_INFO_UTILS_HEADER

/**************************************************
 * Collects and provides information about the
 * machine that application is running on.
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
#include <sys/types.h>

#include "../entity/member/member.h"
#include "../utils/print-utils/print-utils.h"

Member getMachineInfo();

#endif
