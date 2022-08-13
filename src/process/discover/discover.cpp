#include "discover.h"

const int PORT = 4000;
const int BUFFER_SIZE = 512;
const int BROADCAST_AWAITING_TIME_IN_SEC = 10;
const int BROADCAST_SLEEP_TIME_IN_SEC = 10;
const int BROADCAST_TIMEOUT_IN_SEC = 10;

void sendBroadcastPacket(State* state)
{
    while(true) {
        throwExceptionIfNotAlive(state);

        char buffer[BUFFER_SIZE];
        int n{0};
        socklen_t clilen;
        struct sockaddr_in cli_addr;

        // Open a socket
        int sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sockfd <= 0)
        {
            printLine("Error: Could not open socket");
            return;
        }

        // Set socket broadcast
        int broadcastEnable = 1;
        int ret = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
        if (ret)
        {
            printLine("Error: Could not open set socket to broadcast mode");
            close(sockfd);
            return;
        }

        // Set socket timeout
        struct timeval tv;
        tv.tv_sec = BROADCAST_TIMEOUT_IN_SEC;
        tv.tv_usec = 0;
        ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
        if (ret)
        {
            printLine("Error: Could not set socket timeout");
            close(sockfd);
            return;
        }

        struct sockaddr_in broadcastAddr; // Make an endpoint
        memset(&broadcastAddr, 0, sizeof broadcastAddr);
        broadcastAddr.sin_family = AF_INET;

        // Set the broadcast IP address
        inet_pton(AF_INET, "143.54.55.63", &broadcastAddr.sin_addr); 
        broadcastAddr.sin_port = htons(PORT);                                   

        // Send the broadcast request
        std::string message = state->self.toMessage();
        ret = sendto(sockfd, message.c_str(), message.length(), 0, (struct sockaddr *)&broadcastAddr, sizeof broadcastAddr);
        if (ret < 0)
        {
            printLine("Error: Could not open send broadcast");
            close(sockfd);
            return;
        }

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        long timeAwaiting = 0;
        std::list<std::string> memberMessages {};

        while (timeAwaiting < BROADCAST_AWAITING_TIME_IN_SEC)
        {
            /* receive from socket */
            n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cli_addr, &clilen);
            if (n > 0)
            {
                memberMessages.push_back(std::string(buffer));
            }

            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

            timeAwaiting = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
        }

        close(sockfd);

        state->getMembersManager()->addMembersByMessages(memberMessages);
   
        std::this_thread::sleep_for(std::chrono::seconds(BROADCAST_SLEEP_TIME_IN_SEC));
    }
}

void sendBroadcastPacketMock(State* state) {
    while(true) 
    {
        throwExceptionIfNotAlive(state);

        std::list<std::string> memberMessages {};

        memberMessages.push_back("1,192.168.0.50,e3:23:12:12:02:ee,jpedrohost,aa:bb:cc:dd:ee:ff,192.168.0.255,0");
        state->getMembersManager()->addMembersByMessages(memberMessages);
   
        std::this_thread::sleep_for(std::chrono::seconds(BROADCAST_SLEEP_TIME_IN_SEC));
    }
}

void receiveBroadcastPacket(State* state)
{
    int sockfd, n;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[BUFFER_SIZE];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printLine("ERROR opening socket");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(serv_addr.sin_zero), 8);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0)
    {
        printLine("ERROR on binding");
        return;
    }

    clilen = sizeof(struct sockaddr_in);

    while (true)
    {
        throwExceptionIfNotAlive(state);
        
        n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cli_addr, &clilen);
        if (n < 0)
        {
            printLine("ERROR on recvfrom");
        }

        std::string response = state->self.toMessage();

        n = sendto(sockfd, response.c_str(), response.length(), 0, (struct sockaddr *)&cli_addr, sizeof(struct sockaddr));
        if (n < 0)
        {
            printLine("ERROR on sendto");
        }
    }

    close(sockfd);
}

void DiscoverProcess(State* state)
{
    if (state->self.isManager)
    {
        sendBroadcastPacket(state);
    }
    else
    {
        receiveBroadcastPacket(state);
    }
}
