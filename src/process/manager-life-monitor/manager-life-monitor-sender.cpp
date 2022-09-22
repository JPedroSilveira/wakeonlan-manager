#include "manager-life-monitor-sender.h"

const int CONNECTION_TIMEOUT_IN_SEC = 2;
const int MONITORING_SLEEP_IN_SEC = 2;

void sendManagerLifeMonitorPacket(State* state)
{
    int sockfd, n, ret;
    unsigned int length;
    struct sockaddr_in serv_addr, from;
    struct hostent *server;
    char buffer[MANAGER_LIFE_MONITOR_PACKET_SIZE];
    struct timeval tv;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        printFatalError("Fail to open socket to send manager life monitor packets");
        throw FatalErrorException();   
    }

    tv.tv_sec = CONNECTION_TIMEOUT_IN_SEC;
    tv.tv_usec = 0;
    ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if (ret)
    {
        printFatalError("Fail to set socket timeout to send manager life monitor packets");
        close(sockfd);
        return;
    }

    while(true) {
        throwExceptionIfNotAlive(state);

        std::this_thread::sleep_for(std::chrono::seconds(MONITORING_SLEEP_IN_SEC));

        if (state->getSelf().isManager || state->isDoingElection())
        {
            continue;
        }

        try 
        {
            Member managerMember = state->getManagerMember();

            server = gethostbyname(managerMember.ipv4.c_str());
            if (server == NULL) {
                printWarning("Fail to find host for manager " + managerMember.ipv4 + " while sending manager life monitor packet");
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
                printWarning("Fail to send manager life monitor packet for manager " + managerMember.ipv4);
            }
            else
            {
                length = sizeof(struct sockaddr_in);

                n = recvfrom(sockfd, buffer, MANAGER_LIFE_MONITOR_PACKET_SIZE, 0, (struct sockaddr *) &from, &length);
                if (n < 0) {
                    printWarning("Fail to receive manager life monitor packet for manager " + managerMember.ipv4);
                    state->increaseFailToContactManagerCount();
                } else {
                    state->resetFailToContactManagerCount();
                }
            }
        }
        catch (ManagerNotFoundException& e) 
        {
            printWarning("Manager not found while sending manager life monitor packet");
        }
    }
}

void ManagerLifeMonitorSenderProcess(State* state)
{
    try
    {
        sendManagerLifeMonitorPacket(state);
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