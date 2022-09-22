#include "machines-finder.h"

const int BROADCAST_AWAITING_TIME_IN_SEC = 10;
const int BROADCAST_SLEEP_TIME_IN_SEC = 2;
const int BROADCAST_TIMEOUT_IN_SEC = 10;

void sendBroadcastPacket(State* state)
{
    char buffer[MACHINE_FINDER_PACKET_SIZE];
    int n{0};
    socklen_t clilen;
    struct sockaddr_in cli_addr;

    // Open a socket
    int sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd <= 0)
    {
        printFatalError("Fail to open socket to send machines finder packet");
        throw FatalErrorException();
    }

    // Set socket broadcast
    int broadcastEnable = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    if (ret)
    {
        printFatalError("Fail to set socket broadcast to send machines finder packet");
        close(sockfd);
        throw FatalErrorException();
    }

    // Set socket timeout
    struct timeval tv;
    tv.tv_sec = BROADCAST_TIMEOUT_IN_SEC;
    tv.tv_usec = 0;
    ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if (ret)
    {
        printFatalError("Fail to set socket timeout to send machines finder packet");
        close(sockfd);
        throw FatalErrorException();
    }

    struct sockaddr_in broadcastAddr; // Make an endpoint
    memset(&broadcastAddr, 0, sizeof broadcastAddr);
    broadcastAddr.sin_family = AF_INET;

    // Set the broadcast IP address
    // TODO[LAB]: IF NECESSARY SET BROADCAST MANUALLY
    inet_pton(AF_INET, state->getSelf().broadcast.c_str(), &broadcastAddr.sin_addr); 
    broadcastAddr.sin_port = htons(MACHINE_FINDER_PORT);                                   

    // Send the broadcast request
    std::string message = state->getSelf().toMessage();
    ret = sendto(sockfd, message.c_str(), message.length(), 0, (struct sockaddr *)&broadcastAddr, sizeof broadcastAddr);
    if (ret < 0)
    {
        printFatalError("Fail to send machines finder broadcast packet");
        close(sockfd);
        throw FatalErrorException();
    }

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    long timeAwaiting = 0;
    std::list<std::string> memberMessages {};

    while (timeAwaiting < BROADCAST_AWAITING_TIME_IN_SEC)
    {
        /* receive from socket */
        n = recvfrom(sockfd, buffer, MACHINE_FINDER_PACKET_SIZE, 0, (struct sockaddr *)&cli_addr, &clilen);
        if (n > 0)
        {
	    printDebug("New member received");
            memberMessages.push_back(std::string(buffer));
        }

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        timeAwaiting = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
    }

    close(sockfd);

    state->getManager()->addMembersByMessages(memberMessages);
}

void sendBroadcastPacketMock(State* state) 
{
    std::list<std::string> memberMessages {};

    memberMessages.push_back("1,192.168.0.50,e3:23:12:12:02:ee,mockhost1,aa:bb:cc:dd:ee:ff,192.168.0.255,0,1");
    memberMessages.push_back("1,192.168.0.49,e2:23:12:12:02:ee,mockhost2,bb:cc:dd:ee:ff::gg,192.168.0.255,0,2");
    state->getManager()->addMembersByMessages(memberMessages);
}

void MachinesFinderProcess(State* state)
{
    try 
    {
        while(true) 
        {
            throwExceptionIfNotAlive(state);
            if (state->getSelf().isManager)
            {
                sendBroadcastPacket(state);
            }
            std::this_thread::sleep_for(std::chrono::seconds(BROADCAST_SLEEP_TIME_IN_SEC));
        } 
    }
    catch (FatalErrorException& e) 
    {
        return;
    }
    catch (NotAliveException& e)
    {
        return;
    }
}
