#ifndef MEMBER_HEADER
#define MEMBER_HEADER

/**************************************************
 * Class to model a connected machine.
 * ipv4: Member internet protocol version 4.
 * hostname: Member hostname, when available.
 * status: integer representing the Member status.
 * isMananger: boolean defining if the Member is
 * the current manager.
 **************************************************/

#include <sstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include "../../utils/boolean-utils/boolean-utils.h"

class Member
{
private:
    int status;

public:
    std::string ipv4;
    std::string ipv6;
    std::string hostname;
    std::string broadcast;
    std::string mac;
    int pid;
    bool isManager;
    Member();
    int getStatus();
    bool isAwake();
    bool isEqual(Member member);
    void setMemberAsAwake();
    void setMemberAsSleeping();
    std::string toMessage();
    void fromMessage(std::string message);
    std::string toTableLine();
};

#endif
