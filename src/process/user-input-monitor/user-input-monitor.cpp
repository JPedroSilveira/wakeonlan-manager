#include "user-input-monitor.h"

void listenUserInputs(State* state)
{
    while (true)
    {
        throwExceptionIfNotAlive(state);

        printTip("You can enter commands while application is running");
        std::string command;
        std::getline(std::cin, command);

        if (state->getSelf().isManager && command.rfind("WAKEUP", 0) == 0)
        {
            std::string address = command.substr(7, command.length());
	        printInfo("Waking up " + address);
            try 
            {
                Member member = state->getManager()->getByAddress(address);
                sendMagicPacket(member.mac);
            } 
            catch (ItemNotFoundException& e) 
            {         
                printError("Address not found!");
            }
        }
        else if (!state->getSelf().isManager && command == "EXIT")
        {
            printInfo("Existing from application...");
            throw NotAliveException();
        }
        else
        {
            printError("Invalid command!");
        }
    }
}

void UserInputMonitorProcess(State* state)
{
    try
    {
        listenUserInputs(state);
    }
    catch (NotAliveException& e)
    {
        return;
    }
}