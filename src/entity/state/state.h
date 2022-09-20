#ifndef STATE_HEADER
#define STATE_HEADER

/**************************************************
 * Application state
 **************************************************/
#ifdef __APPLE__
#include <dispatch/dispatch.h>
#else
#include <semaphore.h>
#endif

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
    bool alive;
    int failToContactManagerCount;
    #ifdef __APPLE__
        dispatch_semaphore_t    updateFailToContactManagerCountSemaphore;
    #else
        sem_t                   updateFailToContactManagerCountSemaphore;
    #endif
    pthread_mutex_t changeFailToContactManagerCountLock;
    pthread_mutex_t isDoingElectionLock;
    void postFailToContactManagerCountUpdate();
public:
    bool isDoingElection;
    Member self;
    Member getManagerMember();
    State();
    Manager* getManager();
    void kill();
    bool isAlive();
    int getFailToContactManagerCountWhenUpdated();
    void increaseFailToContactManagerCount();
    void increaseFailtToContactManagerCountBy(int quantity);
    void resetFailToContactManagerCount();
    void unlockFailToContactManagerCountLock();
    void resetAndUnlockFailToContactManagerCountLock();
};

#endif
