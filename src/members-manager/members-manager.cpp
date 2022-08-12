#include "members-manager.h"

MembersManager::MembersManager() {}

void MembersManager::updateToAwakeByHostname(std::string hostname)
{
    for (Member member : this->members)
    {
        if (member.hostname == hostname)
        {
            member.setMemberAsAwake();
            return;
        }
    }
}

void MembersManager::updateToAwakeByIPv4(std::string ipv4)
{
    for (Member member : this->members)
    {
        if (member.ipv4 == ipv4)
        {
            member.setMemberAsAwake();
            return;
        }
    }
}

void MembersManager::updateToAwakeByIPv6(std::string ipv6)
{
    for (Member member : this->members)
    {
        if (member.ipv6 == ipv6)
        {
            member.setMemberAsAwake();
            return;
        }
    }
}

void MembersManager::updateToSleepingByHostname(std::string hostname)
{
    for (Member member : this->members)
    {
        if (member.hostname == hostname)
        {
            member.setMemberAsSleeping();
            return;
        }
    }
}

void MembersManager::updateToSleepingByIPv4(std::string ipv4)
{
    for (Member member : this->members)
    {
        if (member.ipv4 == ipv4)
        {
	printf("vai dormir");
            member.setMemberAsSleeping();
            return;
        }
    }
}

void MembersManager::updateToSleepingByIPv6(std::string ipv6)
{
    for (Member member : this->members)
    {
        if (member.ipv6 == ipv6)
        {
            member.setMemberAsSleeping();
            return;
        }
    }
}

void MembersManager::removeByIPv4(std::string ipv4)
{
    this->members.remove_if([ipv4](const Member member)
                            { return member.ipv4 == ipv4; });
}
void MembersManager::removeByIPv6(std::string ipv6)
{
    this->members.remove_if([ipv6](const Member member)
                            { return member.ipv6 == ipv6; });
}

void MembersManager::removeByHostname(std::string hostname)
{
    this->members.remove_if([hostname](const Member member)
                            { return member.hostname == hostname; });
}

void MembersManager::addMember(std::string message)
{
    Member member {};
    member.fromMessage(message);

    this->removeByHostname(member.hostname);
    this->removeByIPv4(member.hostname);
    this->removeByIPv6(member.hostname);

    this->members.push_back(member);
}

Member MembersManager::getByIPv4(std::string ipv4) 
{
	Member member {};
    return member; //TODO
}

Member MembersManager::getByIPv6(std::string ipv6)
{
	Member member {};
    return member; //TODO
}

Member MembersManager::getByHostname(std::string hostname)
{
    for (Member member : this->members)
    {
        if (member.hostname == hostname)
        {
            return member;
        }
    }
	Member member {};
    return member; //TODO
}

std::list<Member> MembersManager::getMembers()
{
    return this->members;
}
