#ifndef PROCESS_INPUT_HEADER
#define PROCESS_INPUT_HEADER

/**************************************************
 * Input entry for a generic process
 **************************************************/

#include <iostream>
#include <vector>
#include "../member/member.h"
#include "../../members-manager/members-manager.h"

class State
{
public:
    Member self;
    MembersManager membersManager;
    State(Member self, MembersManager membersManager);
};

#endif
