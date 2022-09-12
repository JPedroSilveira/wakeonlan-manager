#include "state.h"

State::State()
{
    this->self = getMachineInfo();
    this->isDoingElection = false;
    this->manager = Manager{};
    this->alive = true;
    this->failToContactManagerCount = 0;

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

Manager* State::getManager() 
{
    return &(this->manager);
}

bool State::isAlive()
{
    return this->alive;
}

void State::kill()
{
    this->alive = false;
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

void State::postFailToContactManagerCountUpdate()
{
    #ifdef __APPLE__
        dispatch_semaphore_signal(this->updateFailToContactManagerCountSemaphore);
    #else
        sem_post(&(this->updateFailToContactManagerCountSemaphore));
    #endif
}