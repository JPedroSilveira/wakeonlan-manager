#include "state.h"

State::State()
{
    this->self = getMachineInfo();
    this->membersManager = MembersManager{};
    this->alive = true;
}

MembersManager* State::getMembersManager() 
{
    return &(this->membersManager);
}

bool State::isAlive()
{
    return this->alive;
}

void State::kill()
{
    this->alive = false;
}