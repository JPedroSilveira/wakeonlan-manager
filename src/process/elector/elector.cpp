#include "elector.h"

const int PORT = 4003;
const int BUFFER_SIZE = 65000;
const int SOCKET_TIMEOUT_IN_SEC = 2;

void AnnounceElection() {
    // Broadcasts to the network that the election is going to happen
    int sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd <= 0) {
        printf("Error: Could not open socket");
        
        return;
    }

    int broadcastEnable = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    if (ret) {
        printLine("Error: Could not open set socket to broadcast mode");
        close(sockfd);
        
        return;
    }

}

void ElectionResponse () {
     // Responds to the 

}

void AnnounceWinner () {
    // Announces to the network that this is the new manager
    int sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd <= 0) {
        printf("Error: Could not open socket");
        
        return;
    }

    int broadcastEnable = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    if (ret) {
        printLine("Error: Could not open set socket to broadcast mode");
        close(sockfd);
        
        return;
    }

}