#include "machines-life-monitor.h"

const int MONITORING_SLEEP_IN_SEC = 2;
const int MONITORING_TIMEOUT_IN_SEC = 1;

void sendMonitoringPackets(State* state)
{
    int sockfd, n, ret;
    unsigned int length;
    struct sockaddr_in serv_addr, from;
    struct hostent *server;
    char buffer[MACHINE_LIFE_MONITOR_PACKET_SIZE];
    struct timeval tv;
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        printFatalError("Fail to open socket to send life monitor packet");
        throw FatalErrorException();
    }

    tv.tv_sec = MONITORING_TIMEOUT_IN_SEC;
    tv.tv_usec = 0;
    ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if (ret)
    {
        printFatalError("Fail to set socket timeout to send life monitor packet");
        close(sockfd);
        throw FatalErrorException();
    }

    while(true)
    {
        throwExceptionIfNotAlive(state);

        if (state->getSelf().isManager)
        {
            for (Member member : state->getManager()->getMembers()) 
            {
                server = gethostbyname(member.ipv4.c_str());
                if (server == NULL) {
                    printWarning("Fail to find host for " + member.ipv4 + " while sending life monitor packet");
                    continue;
                }	
                    
                serv_addr.sin_family = AF_INET;     
                serv_addr.sin_port = htons(MACHINE_LIFE_MONITOR_PORT);    
                serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
                bzero(&(serv_addr.sin_zero), 8);  

                std::string message = "Are you alive?";

                n = sendto(sockfd, message.c_str(), message.length(), 0, (const struct sockaddr *) &serv_addr, sizeof(struct sockaddr_in));
                if (n < 0) 
                {
                    printWarning("Fail to send life monitor packet for " + member.ipv4);
                }
                else
                {
                    length = sizeof(struct sockaddr_in);

                    n = recvfrom(sockfd, buffer, MACHINE_LIFE_MONITOR_PACKET_SIZE, 0, (struct sockaddr *) &from, &length);
                    if (n < 0) {
                        state->getManager()->updateToSleepingByIPv4(member.ipv4);
                    } else {
                        state->getManager()->updateToAwakeByIPv4(member.ipv4);
                    }
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(MONITORING_SLEEP_IN_SEC));
    }
}

void sendMonitoringPacketsMock(State* state) 
{
    while(true)
    {
        throwExceptionIfNotAlive(state);
        for (Member member : state->getManager()->getMembers()) 
        {
            if (member.getStatus() == 1) 
            {
                state->getManager()->updateToSleepingByIPv4(member.ipv4);
            }
            else
            {
                state->getManager()->updateToAwakeByIPv4(member.ipv4);
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(MONITORING_SLEEP_IN_SEC));
    }
}

void MachinesLifeMonitorProcess(State* state)
{
    try 
    {
        sendMonitoringPackets(state);
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