#ifndef MEMBERS_MANAGER_HEADER
#define MEMBERS_MANAGER_HEADER

/**************************************************
 * Manages the list of members
 **************************************************/

#include <iostream>
#include <list>
#include "../model/member/member.h"

class MembersManager
{
private:
    std::list<Member> members;

public:
    MembersManager();

    void addMember(std::string message);

    void updateToAwakeByIPv4(std::string ipv4);
    void updateToAwakeByIPv6(std::string ipv6);
    void updateToAwakeByHostname(std::string hostname);

    void updateToSleepingByIPv4(std::string ipv4);
    void updateToSleepingByIPv6(std::string ipv6);
    void updateToSleepingByHostname(std::string hostname);

    void removeByIPv4(std::string ipv4);
    void removeByIPv6(std::string ipv6);
    void removeByHostname(std::string hostname);

    Member getByIPv4(std::string ipv4);
    Member getByIPv6(std::string ipv6);
    Member getByHostname(std::string hostname);

    std::list<Member> getMembers();
};

#endif
