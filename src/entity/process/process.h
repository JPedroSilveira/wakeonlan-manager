#ifndef PROCESS_HEADER
#define PROCESS_HEADER

/**************************************************
 * Class to control a thread process
 * Needs a method as start point to be created.
 * start: initialize the process
 * getStatus: retrieve the current process status
 **************************************************/

#include <iostream>
#include <thread>
#include <future>

#include "../state/state.h"

class Process
{
private:
    std::future<void> future;
    std::function<void(State*)> method;

public:
    Process(std::function<void(State*)> method);
    void start(State* input);
    std::future_status getStatus();
};

#endif