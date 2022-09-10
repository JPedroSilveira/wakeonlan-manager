#ifndef STATE_HEADER
#define STATE_HEADER

/**************************************************
 * Application state
 **************************************************/

#include <iostream>
#include <vector>
#include "../member/member.h"
#include "../manager/manager.h"
#include "../../utils/machine-info-utils/machine-info-utils.h"

class State
{
private:
    Manager manager;
    bool alive;
public:
    Member self;
    State();
    Manager* getManager();
    void kill();
    bool isAlive();
};

#endif
