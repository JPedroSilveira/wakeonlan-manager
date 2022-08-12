#ifndef OUTPUT_HEADER
#define OUTPUT_HEADER

/**************************************************
 * Define default methods to print data on
 * user output.
 * printLine: print a string in one line and flush
 **************************************************/

#include <iostream>
#include "../members-manager/members-manager.h"
#include "../model/member/member.h"

void printLine(std::string message);

void printMembersTable(MembersManager manager);

#endif