#include "machines-update-printer.h"

void printMembersTable(std::list<Member> members) {
    std::string header = "Hostname \t | Endereco MAC \t | Endereco IPv4 \t | Status \n";
    std::list<std::string> lines {};
    for (Member member : members)
    {
        lines.push_back(member.toTableLine());
    }

    printTable(header, lines);
}

void listenAndPrintMembersUpdates(State* state)
{
    while(true)
    {
        throwExceptionIfNotAlive(state);

        std::list<Member> members = state->getManager()->getMembersWhenUpdatedAndLock();

        printMembersTable(members);

        state->getManager()->unlock();
    }
}

void MachinesUpdatePrinter(State* state)
{
    try
    {
       listenAndPrintMembersUpdates(state);
    }
    catch (NotAliveException& e)
    {
        return;
    }
}