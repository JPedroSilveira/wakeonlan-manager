#ifndef WAKE_ON_LAN_UTILS_HEADER
#define WAKE_ON_LAN_UTILS_HEADER

/**************************************************
 * Provides methods to wake up machines using
 * their MAC address
 **************************************************/

#include <iostream>

void sendMagicPacket(std::string mac);

#endif