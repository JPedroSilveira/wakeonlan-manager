#include "manager.h"

Manager::Manager() {
    #ifdef __APPLE__
        this->updateMembersSemaphore = dispatch_semaphore_create(0);
    #else
        sem_init(&(this->updateMembersSemaphore), 0, 0);
    #endif

    int failure = pthread_mutex_init(&(this->changeMembersLock), NULL);
    if (failure != 0) {
        throw MutexInitFailureException();
    }
}

void Manager::hireMemberManagerByIPv4(std::string ipv4)
{
    pthread_mutex_lock(&(this->changeMembersLock));
    bool memberExists = false;
    std::list<Member>::iterator it;
    for (it = this->members.begin(); it != this->members.end(); it++)
    {
        if (it->ipv4 == ipv4)
        {
            memberExists = true;
            it->isManager = true;
            this->postMembersUpdate();
            break;
        }
    }
    pthread_mutex_unlock(&(this->changeMembersLock));
    if (!memberExists) {
        throw ItemNotFoundException();
    }
}

void Manager::hireMemberManagerByIPv6(std::string ipv6)
{
    pthread_mutex_lock(&(this->changeMembersLock));
    bool memberExists = false;
    std::list<Member>::iterator it;
    for (it = this->members.begin(); it != this->members.end(); it++)
    {
        if (it->ipv6 == ipv6)
        {
            memberExists = true;
            it->isManager = true;
            this->postMembersUpdate();
            break;
        }
    }
    pthread_mutex_unlock(&(this->changeMembersLock));
    if (!memberExists) {
        throw ItemNotFoundException();
    }
}

void Manager::hireMemberManagerByHostname(std::string hostname)
{
    pthread_mutex_lock(&(this->changeMembersLock));
    bool memberExists = false;
    std::list<Member>::iterator it;
    for (it = this->members.begin(); it != this->members.end(); it++)
    {
        if (it->hostname == hostname)
        {
            memberExists = true;
            it->isManager = true;
            this->postMembersUpdate();
            break;
        }
    }
    pthread_mutex_unlock(&(this->changeMembersLock));
    if (!memberExists) {
        throw ItemNotFoundException();
    }
}

void Manager::fireMemberManager()
{
    pthread_mutex_lock(&(this->changeMembersLock));
    std::list<Member>::iterator it;
    for (it = this->members.begin(); it != this->members.end(); it++)
    {
        if (it->isManager)
        {
            it->isManager = false;
            it->setMemberAsSleeping();
            this->postMembersUpdate();
            break;
        }
    }
    pthread_mutex_unlock(&(this->changeMembersLock));
}

void Manager::updateToAwakeByHostname(std::string hostname)
{
    pthread_mutex_lock(&(this->changeMembersLock));
    std::list<Member>::iterator it;
    for (it = this->members.begin(); it != this->members.end(); it++)
    {
        if (it->hostname == hostname)
        {
            if (!it->isAwake()) {
                it->setMemberAsAwake();
                this->postMembersUpdate();
            }
            break;
        }
    }
    pthread_mutex_unlock(&(this->changeMembersLock));
}

void Manager::updateToAwakeByIPv4(std::string ipv4)
{
    pthread_mutex_lock(&(this->changeMembersLock));
    std::list<Member>::iterator it;
    for (it = this->members.begin(); it != this->members.end(); it++)
    {
        if (it->ipv4 == ipv4)
        {
            if (!it->isAwake()) {
                it->setMemberAsAwake();
                this->postMembersUpdate();
            }
            break;
        }
    }
    pthread_mutex_unlock(&(this->changeMembersLock));
}

void Manager::updateToAwakeByIPv6(std::string ipv6)
{
    pthread_mutex_lock(&(this->changeMembersLock));
    std::list<Member>::iterator it;
    for (it = this->members.begin(); it != this->members.end(); it++)
    {
        if (it->ipv6 == ipv6)
        {
            if (!it->isAwake()) {
                it->setMemberAsAwake();
                this->postMembersUpdate();
            }
            break;
        }
    }
    pthread_mutex_unlock(&(this->changeMembersLock));
}

void Manager::updateToSleepingByHostname(std::string hostname)
{
    pthread_mutex_lock(&(this->changeMembersLock));
    std::list<Member>::iterator it;
    for (it = this->members.begin(); it != this->members.end(); it++)
    {
        if (it->hostname == hostname)
        {
            if (it->isAwake()) {
                it->setMemberAsSleeping();
                this->postMembersUpdate();
            }
            break;
        }
    }
    pthread_mutex_unlock(&(this->changeMembersLock));
}

void Manager::updateToSleepingByIPv4(std::string ipv4)
{
    pthread_mutex_lock(&(this->changeMembersLock));
    std::list<Member>::iterator it;
    for (it = this->members.begin(); it != this->members.end(); it++)
    {
        if (it->ipv4 == ipv4)
        {
            if (it->isAwake()) {
                it->setMemberAsSleeping();
                this->postMembersUpdate();
            }
            break;
        }
    }
    pthread_mutex_unlock(&(this->changeMembersLock));
}

void Manager::updateToSleepingByIPv6(std::string ipv6)
{
    pthread_mutex_lock(&(this->changeMembersLock));
    std::list<Member>::iterator it;
    for (it = this->members.begin(); it != this->members.end(); it++)
    {
        if (it->ipv6 == ipv6)
        {
            if (it->isAwake()) {
                it->setMemberAsSleeping();
                this->postMembersUpdate();
            }
            break;
        }
    }
    pthread_mutex_unlock(&(this->changeMembersLock));
}

int Manager::removeByIPv4(std::string ipv4)
{
    int oldSize = this->members.size();
    this->members.remove_if([ipv4](const Member member) { return member.ipv4 == ipv4; });
    return oldSize - this->members.size();
}

int Manager::removeByIPv6(std::string ipv6)
{
    int oldSize = this->members.size();
    this->members.remove_if([ipv6](const Member member) { return member.ipv6 == ipv6; });
    return oldSize - this->members.size();
}

int Manager::removeByHostname(std::string hostname)
{
    int oldSize = this->members.size();
    this->members.remove_if([hostname](const Member member) { return member.hostname == hostname; });
    return oldSize - this->members.size();
}

void Manager::addMemberByMessage(std::string message)
{
    pthread_mutex_lock(&(this->changeMembersLock));
    Member member {};
    member.fromMessage(message);

    if (!this->exists(member))
    {
        this->members.push_back(member);
        this->postMembersUpdate();
    }
    
    pthread_mutex_unlock(&(this->changeMembersLock));
}

void Manager::addMembersByMessages(std::list<std::string> messages)
{
    pthread_mutex_lock(&(this->changeMembersLock));

    bool memberListUpdated = false;

    for (std::string message : messages) {
        Member member {};
        member.fromMessage(message);

        if (!exists(member))
        {
            this->members.push_back(member);
            memberListUpdated = true;
        }
    }

    if (memberListUpdated)
    {
        this->postMembersUpdate();
    }

    pthread_mutex_unlock(&(this->changeMembersLock));
}

void Manager::setMembersByMessages(std::list<std::string> messages)
{
    pthread_mutex_lock(&(this->changeMembersLock));

    this->members.clear();

    for (std::string message : messages) {
        Member member {};
        member.fromMessage(message);
        this->members.push_back(member);
    }

    this->postMembersUpdate();

    pthread_mutex_unlock(&(this->changeMembersLock));
}

Member Manager::getByAddress(std::string address)
{
    try 
    {
        return this->getByHostname(address);
    } 
    catch (ItemNotFoundException& e) 
    {        
        try 
        {
            return this->getByIPv4(address);
        } 
        catch (ItemNotFoundException& e) 
        {
            return this->getByIPv6(address);
        }
    }
}

Member Manager::getByIPv4(std::string ipv4) 
{
    if (!ipv4.empty()) {
        for (Member member : this->members)
        {
            if (member.ipv4 == ipv4)
            {
                return member;
            }
        }
    }
    
    throw ItemNotFoundException();
}

Member Manager::getByIPv6(std::string ipv6)
{
    if (!ipv6.empty()) {
        for (Member member : this->members)
        {
            if (member.ipv6 == ipv6)
            {
                return member;
            }
        }
    }
    throw ItemNotFoundException();
 }

Member Manager::getByHostname(std::string hostname)
{
    if (!hostname.empty()) {
        for (Member member : this->members)
        {
            if (member.hostname == hostname)
            {
                return member;
            }
        }
    }
    throw ItemNotFoundException();
}

std::list<Member> Manager::getMembers()
{
    return this->members;
}

std::list<Member> Manager::getMembersWhenUpdatedAndLock()
{    
    #ifdef __APPLE__
        dispatch_semaphore_wait(this->updateMembersSemaphore, DISPATCH_TIME_FOREVER);
    #else
        sem_wait(&(this->updateMembersSemaphore));
    #endif

    pthread_mutex_lock(&(this->changeMembersLock));

    return this->members;
}

void Manager::unlock()
{
    pthread_mutex_unlock(&(this->changeMembersLock));
}

void Manager::postMembersUpdate()
{
    #ifdef __APPLE__
        dispatch_semaphore_signal(this->updateMembersSemaphore);
    #else
        sem_post(&(this->updateMembersSemaphore));
    #endif
}

bool Manager::exists(Member member)
{
    for (Member existentMember : this->members)
    {
        if (!member.ipv4.empty() && existentMember.ipv4 == member.ipv4)
        {
            return true;
        }
        else if (!member.ipv6.empty() && existentMember.ipv6 == member.ipv6)
        {
            return true;
        }
        else if (!member.hostname.empty() && existentMember.hostname == member.hostname)
        {
            return true;
        }
    }

    return false;
}

void Manager::killApplication()
{
    this->postMembersUpdate();
}