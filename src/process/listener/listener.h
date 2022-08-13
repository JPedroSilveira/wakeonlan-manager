#ifndef LISTENER_HEADER
#define LISTENER_HEADER

/**************************************************
 * Monitor members list and print members list
 * when something changes
 **************************************************/

#include <thread>
#include <chrono>
#include <list>

#include "../../utils/alive-utils/alive-utils.h"
#include "../../model/state/state.h"
#include "../../model/member/member.h"

void ListenerProcess(State* state);

#endif