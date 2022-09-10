#ifndef PRINT_UTILS_HEADER
#define PRINT_UTILS_HEADER

/**************************************************
 * Provide methods to print data on terminal.
 **************************************************/

#include <iostream>
#include <list>

void printLine(std::string message);

void printTable(std::string header, std::list<std::string> lines);

#endif