#include "state.h"

State::State()
{
    this->self = getMachineInfo();
    this->manager = Manager{};
    this->alive = true;
    this->failToContactManagerCount = 0;
}

Member State::getManagerMember()
{
    for (Member member : this->getManager()->getMembers()) 
    {
        if (member.isManager) {
            return member;
        }
    }
    throw ManagerNotFoundException();
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

int State::getFailToContactManagerCount()
{
    return this->failToContactManagerCount;
}

void State::increaseFailToContactManagerCount()
{
    this->failToContactManagerCount++;
}

void State::resetFailToContactManagerCount()
{
    this->failToContactManagerCount = 0;
}