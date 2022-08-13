#include "member.h"

const std::string SEPARATOR = ",";

Member::Member()
{
    this->status = 1;
    this->ipv4 = "";
    this->ipv6 = "";
    this->hostname = "";
    this->mac = "";
    this->broadcast = "";
    this->isManager = false;
}

void Member::setMemberAsAwake()
{
    this->status = 1;
}

void Member::setMemberAsSleeping()
{
    this->status = 0;
}

std::string Member::toMessage()
{
    std::string message = "";
    message.append(std::to_string(this->status)).append(SEPARATOR)
        .append(this->ipv4).append(SEPARATOR)
        .append(this->ipv6).append(SEPARATOR)
        .append(this->hostname).append(SEPARATOR)
        .append(this->mac).append(SEPARATOR)
        .append(this->broadcast).append(SEPARATOR)
        .append(booleanToString(this->isManager)).append(SEPARATOR);
    return message;
}

void Member::fromMessage(std::string message) 
{
    std::istringstream stream(message);
    std::string attribute;    

    std::getline(stream, attribute, SEPARATOR.at(0));
    attribute.erase(std::remove(attribute.begin(), attribute.end(), '\n'), attribute.end());
    this->status = std::stoi(attribute);

    std::getline(stream, attribute, SEPARATOR.at(0));
    attribute.erase(std::remove(attribute.begin(), attribute.end(), '\n'), attribute.end());
    this->ipv4 = attribute;

    std::getline(stream, attribute, SEPARATOR.at(0));
    attribute.erase(std::remove(attribute.begin(), attribute.end(), '\n'), attribute.end());
    this->ipv6 = attribute;    

    std::getline(stream, attribute, SEPARATOR.at(0));
    attribute.erase(std::remove(attribute.begin(), attribute.end(), '\n'), attribute.end());
    this->hostname = attribute;  

    std::getline(stream, attribute, SEPARATOR.at(0));
    attribute.erase(std::remove(attribute.begin(), attribute.end(), '\n'), attribute.end());
    this->mac = attribute;  

    std::getline(stream, attribute, SEPARATOR.at(0));
    attribute.erase(std::remove(attribute.begin(), attribute.end(), '\n'), attribute.end());
    this->broadcast = attribute;  

    std::getline(stream, attribute, SEPARATOR.at(0));
    attribute.erase(std::remove(attribute.begin(), attribute.end(), '\n'), attribute.end());
    this->isManager = stringToBoolean(attribute);
}

int Member::getStatus() 
{
    return this->status;
}

void Member::setIsManager(std::vector<std::string> args)
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
        printLine("Manager Machine");
    }
    else
    {
        printLine("Client Machine");
    }
    this->isManager = isManager;
}

std::string Member::toTableLine() 
{
    return this->hostname + " \t | " + this->mac + " \t | " +  this->ipv4 + " \t | " + std::to_string(this->isAwake()) + "\n";
}