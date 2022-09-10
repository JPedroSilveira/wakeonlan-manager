#include "user-input-monitor.h"

void UserInputMonitorProcess(State* state)
{
    while (true)
    {
        throwExceptionIfNotAlive(state);

        printLine("You can enter commands while application is running");
        std::string command;
        std::getline(std::cin, command);

        if (state->self.isManager && command.rfind("WAKEUP", 0) == 0)
        {
            std::string address = command.substr(7, command.length());
	        printLine("Waking up " + address);
            try {
                Member member = state->getManager()->getByAddress(address);
                sendMagicPacket(member.mac);
            } catch (ItemNotFoundException& e) {         
                printLine("Address not found!");
            }
        }
        else if (!state->self.isManager && command == "EXIT")
        {
            printLine("Exiting...");
            break;
        }
        else
        {
            printLine("Invalid command!");
        }
    }
}