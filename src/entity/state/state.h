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
#include "../../exception/manager-not-found-exception/manager-not-found-exception.h"

class State
{
private:
    Manager manager;
    int failToContactManagerCount;
    bool alive;
public:
    Member self;
    Member getManagerMember();
    State();
    Manager* getManager();
    void kill();
    bool isAlive();
    int getFailToContactManagerCount();
    void increaseFailToContactManagerCount();
    void resetFailToContactManagerCount();
};

#endif
