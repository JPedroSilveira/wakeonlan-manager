#include "elector.h"

const int PORT = 4003;
const int BUFFER_SIZE = 65000;
const int SOCKET_TIMEOUT_IN_SEC = 2;
const std::string BROADCASTADDRESS "";

void AnnounceElection() {
    //TODO mudar pra TCP 
    // Broadcasts to the network that the election is going to happen
    int sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd <= 0) {
        printLine("Error: Could not open socket");
        
        return;
    }

    int broadcastEnable = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    if (ret) {
        printLine("Error: Could not open set socket to broadcast mode");
        close(sockfd);
        
        return;
    }

    struct sockaddr_in broadcastAddr; // Make an endpoint
    memset(&broadcastAddr, 0, sizeof broadcastAddr);
    broadcastAddr.sin_family = AF_INET;

        // Set the broadcast IP address
    inet_pton(AF_INET, BROADCASTADDRESS, &broadcastAddr.sin_addr); 
    broadcastAddr.sin_port = htons(PORT);                                   

        // Send the broadcast request
    std::string message = "Election Request";
    ret = sendto(sockfd, message.c_str(), message.length(), 0, (struct sockaddr *)&broadcastAddr, sizeof broadcastAddr);
    if (ret < 0) {
        printLine("Error: Could not open send broadcast");
        close(sockfd);
        return;
    }
}

void ElectionResponse () {
     // TODO Responds to the election message using reliable communication TCP

}

void AnnounceWinner () {
    // TODO  Announces to the network that this is the new Coordinator
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