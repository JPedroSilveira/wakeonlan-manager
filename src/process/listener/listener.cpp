#include "listener.h"

const int LISTENING_SLEEP_IN_SEC = 5;

void printMembersTable(std::list<Member> members) {
    std::string header = "Hostname \t | Endereço MAC \t | Endereço IPv4 \t | Status \n";
    std::list<std::string> lines {};
    for (Member member : members)
    {
        lines.push_back(member.toTableLine());
    }

    printTable(header, lines);
}

void listenAndPrintUpdates(State* state)
{
    while(true)
    {
        throwExceptionIfNotAlive(state);

        std::list<Member> members = state->getManager()->getMembersWhenUpdatedAndLock();

        printMembersTable(members);

        state->getManager()->unlock();

        std::this_thread::sleep_for(std::chrono::seconds(LISTENING_SLEEP_IN_SEC));
    }
}

void doNothing(State* state)
{
    while(true) 
    {
        throwExceptionIfNotAlive(state);
        std::this_thread::sleep_for(std::chrono::seconds(LISTENING_SLEEP_IN_SEC));
    }
}

void ListenerProcess(State* state)
{
    if (state->self.isManager)
    {
        listenAndPrintUpdates(state);
    }
    else
    {
        doNothing(state);
    }
}