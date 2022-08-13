#include "state.h"

State::State()
{
    this->self = getMachineInfo();
    this->manager = Manager{};
    this->alive = true;
}

Manager* State::getManager() 
{
    return &(this->manager);
}

bool State::isAlive()
{
    return this->alive;
}

void State::kill()
{
    this->alive = false;
}