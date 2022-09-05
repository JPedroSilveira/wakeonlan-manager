#ifndef PRINT_UTILS_HEADER
#define PRINT_UTILS_HEADER

/**************************************************
 * Define default methods to print data on terminal.
 * printLine: print a string in one line and flush
 **************************************************/

#include <iostream>
#include <list>

void printLine(std::string message);

void printTable(std::string header, std::list<std::string> lines);

#endif