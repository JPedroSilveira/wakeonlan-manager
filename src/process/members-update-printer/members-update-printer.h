#ifndef MEMBERS_UPDATE_PRINTER_HEADER
#define MEMBERS_UPDATE_PRINTER_HEADER

/**************************************************
 * Monitor members table in application 
 * state and print the table everytime it changes
 **************************************************/

#include <thread>
#include <chrono>
#include <list>

#include "../../utils/alive-utils/alive-utils.h"
#include "../../entity/state/state.h"
#include "../../entity/member/member.h"
#include "../../exception/not-alive/not-alive-exception.h"

void MembersUpdatePrinter(State* state);

#endif