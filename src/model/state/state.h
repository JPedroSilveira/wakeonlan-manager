#ifndef STATE_HEADER
#define STATE_HEADER

/**************************************************
 * Input entry for a generic process
 **************************************************/

#include <iostream>
#include <vector>
#include "../member/member.h"
#include "../../members-manager/members-manager.h"
#include "../../machine-info/machine-info.h"

class State
{
private:
    MembersManager membersManager;
    bool alive;
public:
    Member self;
    State();
    MembersManager* getMembersManager();
    void kill();
    bool isAlive();
};

#endif
