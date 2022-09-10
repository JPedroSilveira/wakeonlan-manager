#ifndef MANAGER_LIFE_MONITOR_HEADER
#define MANAGER_LIFE_MONITOR_HEADER

/**************************************************
 * If manager: answer manager-is-alive packages
 * 
 * Else: send manager-is-alive-packages and saves 
 * how many times the manager did not respond 
 * to the request
 **************************************************/

#include "../../entity/state/state.h"

void ManagerLifeMonitorProcess(State* state);

#endif
