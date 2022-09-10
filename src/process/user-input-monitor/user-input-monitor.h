#ifndef USER_INPUT_MONITOR_HEADER
#define USER_INPUT_MONITOR_HEADER

/**************************************************
 * Process user inputs on command line.
 * 
 * If manager: accepts WAKEUP command to wake up
 * a member machine.
 * 
 * Else: accepts EXIT command to close the 
 * application.
 **************************************************/

#include <iostream>

#include "../../utils/alive-utils/alive-utils.h"
#include "../../utils/print-utils/print-utils.h"
#include "../../utils/wake-on-lan-utils/wake-on-lan-utils.h"
#include "../../entity/state/state.h"
#include "../../exception/item-not-found-exception/item-not-found-exception.h"

void UserInputMonitorProcess(State* state);

#endif
