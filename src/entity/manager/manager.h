#ifndef MANAGER_HEADER
#define MANAGER_HEADER

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
#include <pthread.h>

#include "../member/member.h"
#include "../../exception/item-not-found-exception/item-not-found-exception.h"
#include "../../exception/mutex-init-failure/mutex-init-failure-exception.h"

class Manager
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
    bool exists(Member member);
    Member getExistentMember(Member member);
    void hireMemberManagerByIPv4(std::string ipv4);
    void hireMemberManagerByIPv6(std::string ipv6);
    void hireMemberManagerByHostname(std::string hostname);

public:
    Manager();

    void killApplication();

    void hireMemberManager(Member member);
    void fireMemberManager();

    void addMemberByMessage(std::string message);
    void addMembersByMessages(std::list<std::string> messages);
    void updateMembersByMessages(std::list<std::string> messages);

    void updateToAwakeByIPv4(std::string ipv4);
    void updateToAwakeByIPv6(std::string ipv6);
    void updateToAwakeByHostname(std::string hostname);

    void updateToSleepingByIPv4(std::string ipv4);
    void updateToSleepingByIPv6(std::string ipv6);
    void updateToSleepingByHostname(std::string hostname);

    Member getByAddress(std::string address);
    Member getByIPv4(std::string ipv4);
    Member getByIPv6(std::string ipv6);
    Member getByHostname(std::string hostname);

    std::list<Member> getMembers();

    std::list<Member> getMembersWhenUpdatedAndLock();
    void unlockMembersUpdates();
};

#endif
