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
#include "../../utils/print-utils/print-utils.h"
#include "../../utils/machine-info-utils/machine-info-utils.h"
#include "../../exception/manager-not-found-exception/manager-not-found-exception.h"

class State
{
private:
    Manager manager;
    bool alive;
    bool electionStarted;
    std::string ipv4;
    std::string ipv6;
    std::string hostname;
    int failToContactManagerCount;
    #ifdef __APPLE__
        dispatch_semaphore_t updateFailToContactManagerCountSemaphore;
        dispatch_semaphore_t electionStartedSemaphore;
    #else
        sem_t updateFailToContactManagerCountSemaphore;
        sem_t electionStartedSemaphore;
    #endif
    pthread_mutex_t changeFailToContactManagerCountLock;
    pthread_mutex_t electionStartedLock;
    void postFailToContactManagerCountUpdate();
public:
    State();
    Member getSelf();
    Member getManagerMember();
    Manager* getManager();
    void setIsManagerByArgs(std::vector<std::string> args);
    void kill();
    bool isAlive();
    int failToContactManagerCountListener();
    void increaseFailToContactManagerCount();
    void increaseFailtToContactManagerCountBy(int quantity);
    void resetFailToContactManagerCount();
    void unlockFailToContactManagerCountLock();
    void resetAndUnlockFailToContactManagerCountLock();
    void tryStartElection();
    void awaitForElectionStart();
    void finishElection();
    bool isDoingElection();
};

#endif
