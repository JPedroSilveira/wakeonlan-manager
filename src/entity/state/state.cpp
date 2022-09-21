#include "state.h"

State::State()
{
    Member self = getMachineInfo();
    
    this->ipv4 = self.ipv4;
    this->ipv6 = self.ipv6;
    this->hostname = self.hostname;

    this->alive = true;
    this->isDoingElection = false;
    this->failToContactManagerCount = 0;

    this->manager = Manager{};
    this->manager.addMemberByMessage(self.toMessage());
    
    #ifdef __APPLE__
        this->updateFailToContactManagerCountSemaphore = dispatch_semaphore_create(0);
    #else
        sem_init(&(this->updateFailToContactManagerCountSemaphore), 0, 0);
    #endif

    int failure = pthread_mutex_init(&(this->changeFailToContactManagerCountLock), NULL);
    if (failure != 0) {
        throw MutexInitFailureException();
    }
    
    failure = pthread_mutex_init(&(this->isDoingElectionLock), NULL);
    if (failure != 0) {
        throw MutexInitFailureException();
    }
}

Member State::getManagerMember()
{
    for (Member member : this->getManager()->getMembers()) 
    {
        if (member.isManager) {
            return member;
        }
    }
    throw ManagerNotFoundException();
}

Member State::getSelf()
{
    try 
    {
        return this->manager.getByIPv4(this->ipv4);
    } 
    catch (ItemNotFoundException& e) 
    {        
        try 
        {
            return this->manager.getByIPv6(this->ipv6);
        } 
        catch (ItemNotFoundException& e) 
        {
            return this->manager.getByHostname(this->hostname);
        }
    }
}

Manager* State::getManager() 
{
    return &(this->manager);
}

void State::setIsManagerByArgs(std::vector<std::string> args)
{
    bool isManager = false;

    for (std::string arg : args)
    {
        if (arg == "manager")
        {
            isManager = true;
            break;
        }
    }

    if (isManager) 
    {
        printInfo("Manager Machine");
        try 
        {
            return this->manager.hireMemberManagerByIPv4(this->ipv4);
        } 
        catch (ItemNotFoundException& e) 
        {        
            try 
            {
                return this->manager.hireMemberManagerByIPv6(this->ipv6);
            } 
            catch (ItemNotFoundException& e) 
            {
                return this->manager.hireMemberManagerByHostname(this->hostname);
            }
        }
    }
    else
    {
        printInfo("Client Machine");
    }
}

bool State::isAlive()
{
    return this->alive;
}

void State::kill()
{
    this->alive = false;
    this->manager.killApplication();
    this->postFailToContactManagerCountUpdate();
}

int State::getFailToContactManagerCountWhenUpdated()
{
    #ifdef __APPLE__
        dispatch_semaphore_wait(this->updateFailToContactManagerCountSemaphore, DISPATCH_TIME_FOREVER);
    #else
        sem_wait(&(this->updateFailToContactManagerCountSemaphore));
    #endif

    pthread_mutex_lock(&(this->changeFailToContactManagerCountLock));

    return this->failToContactManagerCount;
}

void State::increaseFailToContactManagerCount()
{
    pthread_mutex_lock(&(this->changeFailToContactManagerCountLock));
    this->failToContactManagerCount++;
    this->postFailToContactManagerCountUpdate();
    pthread_mutex_unlock(&(this->changeFailToContactManagerCountLock));
}

void State::increaseFailtToContactManagerCountBy(int quantity)
{
    pthread_mutex_lock(&(this->changeFailToContactManagerCountLock));
    this->failToContactManagerCount = this->failToContactManagerCount + quantity;
    this->postFailToContactManagerCountUpdate();
    pthread_mutex_unlock(&(this->changeFailToContactManagerCountLock));
}

void State::resetFailToContactManagerCount()
{
    pthread_mutex_lock(&(this->changeFailToContactManagerCountLock));
    this->failToContactManagerCount = 0;
    this->postFailToContactManagerCountUpdate();
    pthread_mutex_unlock(&(this->changeFailToContactManagerCountLock));
}

void State::unlockFailToContactManagerCountLock()
{
    pthread_mutex_unlock(&(this->changeFailToContactManagerCountLock));
}

void State::resetAndUnlockFailToContactManagerCountLock()
{
    this->failToContactManagerCount = 0;
    pthread_mutex_unlock(&(this->changeFailToContactManagerCountLock));
}

void State::postFailToContactManagerCountUpdate()
{
    #ifdef __APPLE__
        dispatch_semaphore_signal(this->updateFailToContactManagerCountSemaphore);
    #else
        sem_post(&(this->updateFailToContactManagerCountSemaphore));
    #endif
}