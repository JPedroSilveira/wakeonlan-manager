#ifndef MEMBERS_MANAGER_HEADER
#define MEMBERS_MANAGER_HEADER

/**************************************************
 * Manages the list of members
 **************************************************/
#ifdef __APPLE__
#include <dispatch/dispatch.h>
#else
#include <semaphore.h>
#endif

#include <iostream>
#include <list>

#include "../model/member/member.h"
#include "../exception/item-not-found/item-not-found-exception.h"
#include "../exception/mutex-init-failure/mutex-init-failure-exception.h"
#include <pthread.h>

class MembersManager
{
private:
    std::list<Member> members;
    #ifdef __APPLE__
        dispatch_semaphore_t    updateMembersSemaphore;
    #else
        sem_t                   updateMembersSemaphore;
    #endif
    pthread_mutex_t changeMembersLock;

    int removeByIPv4(std::string ipv4);
    int removeByIPv6(std::string ipv6);
    int removeByHostname(std::string hostname);
    void postMembersUpdate();

public:
    MembersManager();

    void addMembersByMessages(std::list<std::string> messages);

    void updateToAwakeByIPv4(std::string ipv4);
    void updateToAwakeByIPv6(std::string ipv6);
    void updateToAwakeByHostname(std::string hostname);

    void updateToSleepingByIPv4(std::string ipv4);
    void updateToSleepingByIPv6(std::string ipv6);
    void updateToSleepingByHostname(std::string hostname);

    Member getByIPv4(std::string ipv4);
    Member getByIPv6(std::string ipv6);
    Member getByHostname(std::string hostname);

    std::list<Member> getMembers();

    std::list<Member> getMembersWhenUpdatedAndLock();
    void unlock();
};

#endif
