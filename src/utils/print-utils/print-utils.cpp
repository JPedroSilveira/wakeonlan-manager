#include "print-utils.h"

const bool ENABLE_DEBUGGING = true;

const std::string SEPARATOR("=================================================================");

void printLine(std::string message)
{
    std::cout << SEPARATOR << std::endl << message << std::endl << std::flush;
}

void printInfo(std::string message)
{
    std::string finalMessage = "INFO: " + message;
    printLine(finalMessage);
}

void printTip(std::string message)
{
    std::string finalMessage = "TIP: " + message;
    printLine(finalMessage);
}

void printWarning(std::string message)
{
    std::string finalMessage = "WARNING: " + message;
    printLine(finalMessage);
}

void printError(std::string message)
{
    std::string finalMessage = "ERROR: " + message;
    printLine(finalMessage);
}

void printFatalError(std::string message)
{
    std::string finalMessage = "FATAL ERROR (!): " + message;
    printLine(finalMessage);
}

void printDebug(std::string message)
{
    if (ENABLE_DEBUGGING) {
        std::string finalMessage = "DEBUG (X): " + message;
        printLine(finalMessage);
    }
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
