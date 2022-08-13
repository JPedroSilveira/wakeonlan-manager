#include "print-utils.h"

const std::string SEPARATOR("=================================================================");

void printLine(std::string message)
{
    std::cout << SEPARATOR << std::endl << message << std::endl << std::flush;
}

void printTable(std::string header, std::list<std::string> lines) {
    std::string table = header;
    if (lines.size() > 0) 
    {
        for (std::string line : lines) 
        {	
            table.append(line);
        }
    }
    else
    {
        table.append("No data");
    }

    std::cout << SEPARATOR << std::endl << table << std::endl << std::flush;
}
