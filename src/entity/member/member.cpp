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
    this->pid = 0;
}

bool Member::isAwake()
{
    return this->status == 1;
}

bool Member::isEqual(Member member)
{
    return member.toMessage() == this->toMessage();
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
        .append(booleanToString(this->isManager)).append(SEPARATOR)
        .append(std::to_string(this->pid)).append(SEPARATOR);
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

    std::getline(stream, attribute, SEPARATOR.at(0));
    attribute.erase(std::remove(attribute.begin(), attribute.end(), '\n'), attribute.end());
    this->pid = std::stoi(attribute);
}

int Member::getStatus() 
{
    return this->status;
}

std::string Member::toTableLine() 
{
    return this->hostname + " \t | " + this->mac + " \t | " +  this->ipv4 + " \t | " + std::to_string(this->isAwake()) + + " \t | " + (this->isManager ? "true" : "false") + "\t \n";
}