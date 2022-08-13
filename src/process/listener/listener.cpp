#include "listener.h"

const int LISTENING_SLEEP_IN_SEC = 5;

void listenAndPrintUpdates(State* state)
{
    while(true)
    {
        throwExceptionIfNotAlive(state);

        std::list<Member> members = state->getMembersManager()->getMembersWhenUpdatedAndLock();

        printMembersTable(members);

        state->getMembersManager()->unlock();

        std::this_thread::sleep_for(std::chrono::seconds(LISTENING_SLEEP_IN_SEC));
    }
}

void doNothing(State* state)
{
    while(true) 
    {
        throwExceptionIfNotAlive(state);
        std::this_thread::sleep_for(std::chrono::seconds(LISTENING_SLEEP_IN_SEC));
    }
}

void ListenerProcess(State* state)
{
    if (state->self.isManager)
    {
        listenAndPrintUpdates(state);
    }
    else
    {
        doNothing(state);
    }
}