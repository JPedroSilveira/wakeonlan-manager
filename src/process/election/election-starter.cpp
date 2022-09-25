#include "election-starter.h"

void listenAndTriggerElectionIfNecessary(State* state)
{
    while(true) 
    {
        throwExceptionIfNotAlive(state);

        int failToContactManagerCount = state->failToContactManagerCountListener();

        if (failToContactManagerCount >= FAIL_TO_CONTACT_MANAGER_COUNT_THRESHOLD) 
        {
            state->awaitForStartElectionTrigger();
        }
        
        state->unlockFailToContactManagerCountLock();
    }
}

void ElectionStarterProcess(State* state) 
{
    try
    {
        listenAndTriggerElectionIfNecessary(state);
    }
    catch (FatalErrorException& e) 
    {
        return;
    }
    catch (NotAliveException& e)
    {
        return;
    }
}