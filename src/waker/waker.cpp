#include "waker.h"

const std::string WAKE_ON_LAN_COMMAND = "wakeonlan";

void wakeUp(std::string mac)
{
    std::string command = WAKE_ON_LAN_COMMAND + " " + mac;
    system(command.c_str());
}