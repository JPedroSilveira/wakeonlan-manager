#ifndef STATE_HEADER
#define STATE_HEADER

/**************************************************
 * Input entry for a generic process
 **************************************************/

#include <iostream>
#include <vector>
#include "../member/member.h"
#include "../manager/manager.h"
#include "../../machine-info/machine-info.h"

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
