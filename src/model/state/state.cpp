#include "state.h"

State::State(Member self, MembersManager membersManager)
{
    this->self = self;
    this->membersManager = membersManager;
}
