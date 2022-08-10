#ifndef INPUT_HEADER
#define INPUT_HEADER

/**************************************************
 * Process user inputs on command line.
 * - If manager it accepts WAKEUP command.
 * - If not it accepts EXIT command.
 **************************************************/

#include <iostream>

#include "../model/state/state.h"
#include "../output/output.h"
#include "../model/member/member.h"
#include "../waker/waker.h"

void InputProcess(State* state);

#endif
