#ifndef USER_INPUT_MONITOR_HEADER
#define USER_INPUT_MONITOR_HEADER

/**************************************************
 * Process user inputs on command line.
 * - If manager it accepts WAKEUP command.
 * - If not it accepts EXIT command.
 **************************************************/

#include <iostream>

#include "../../utils/alive-utils/alive-utils.h"
#include "../../utils/print-utils/print-utils.h"
#include "../../waker/waker.h"
#include "../../model/state/state.h"
#include "../../exception/item-not-found/item-not-found-exception.h"

void UserInputMonitorProcess(State* state);

#endif
