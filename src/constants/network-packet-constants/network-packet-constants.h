#ifndef NETWORK_PACKAGES_CONSTANTS_HEADER
#define NETWORK_PACKAGES_CONSTANTS_HEADER

/**************************************************
 * Define packages max size for all network packages
 **************************************************/

const int MACHINE_FINDER_PACKET_SIZE = 512;
const int MACHINE_LIFE_MONITOR_PACKET_SIZE = 512;
const int MACHINE_TABLE_REPLICATOR_PACKET_SIZE = 65000;
const int MANAGER_LIFE_MONITOR_PACKET_SIZE = 256;
const int ELECTION_VOTE_PACKET_SIZE = 256;

#endif
