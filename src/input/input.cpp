#include "input.h"

void InputProcess(State* state)
{
    while (true)
    {
        printLine("You can enter commands while application is running");
        std::string command;
        std::getline(std::cin, command);

        if (state->self.isManager && command.rfind("WAKEUP", 0) == 0)
        {
            std::string address = command.substr(7, command.length());
	    printLine("Waking up..." + address);
            Member member = state->membersManager.getByHostname(address);
	    wakeUp(member.mac);
	
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
