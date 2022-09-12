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

void Manager::addMembersByMessages(std::list<std::string> messages)
{
    pthread_mutex_lock(&(this->changeMembersLock));

    for (std::string message : messages) {
        Member member {};
        member.fromMessage(message);

        int removedItems = this->removeByHostname(member.hostname);
        if (removedItems < 0) {
            this->removeByIPv4(member.hostname);
        }
        if (removedItems < 0) {
            this->removeByIPv6(member.hostname);
        }

        this->members.push_back(member);
    }

    this->postMembersUpdate();

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
        try {
            return this->getByIPv4(address);
        } catch (ItemNotFoundException& e) {
            return this->getByIPv6(address);
        }
    }
}

Member Manager::getByIPv4(std::string ipv4) 
{
    for (Member member : this->members)
    {
        if (member.ipv4 == ipv4)
        {
            return member;
        }
    }
    throw ItemNotFoundException();
}

Member Manager::getByIPv6(std::string ipv6)
{
    for (Member member : this->members)
    {
        if (member.ipv6 == ipv6)
        {
            return member;
        }
    }
    throw ItemNotFoundException();
 }

Member Manager::getByHostname(std::string hostname)
{
    for (Member member : this->members)
    {
        if (member.hostname == hostname)
        {
            return member;
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