#include "output.h"

const std::string SEPARATOR("=================================================================");

void printLine(std::string message)
{
    std::cout << SEPARATOR << std::endl
        << message << std::endl
        << std::flush;
}

void printMembersTable(MembersManager manager) {
    std::string table = "Hostname \t | Endereço MAC \t | Endereço IPv4 \t | Status \n";
    for (Member member : manager.getMembers()) 
    {	
	printLine(member.hostname);
        table.append(member.hostname + " \t | " + member.mac + " \t | " +  member.ipv4 + " \t | " + std::to_string(member.getStatus()) + "\n");
    }
    std::cout << SEPARATOR << std::endl
        << table << std::endl
        << std::flush;
}
