#include "machines-table-replicator-sender.h"

const int IS_NOT_MANAGER_SLEEP_IN_SEC = 2;
const int SOCKET_TIMEOUT_IN_SEC = 1;
const std::string LIST_SEPARATOR = ";";

void listenAndSendUpdates(State* state)
{
    int sockfd, n, ret;
    unsigned int length;
    struct sockaddr_in serv_addr, from;
    struct hostent *server;
    char buffer[MACHINE_TABLE_REPLICATOR_PACKAGE_SIZE];
    struct timeval tv;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
    {
        printFatalError("Fail to open socket to send machines table update packet");
        throw FatalErrorException();        
    }

    tv.tv_sec = SOCKET_TIMEOUT_IN_SEC;
    tv.tv_usec = 0;
    ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if (ret)
    {
        printFatalError("Fail to set socket timeout to send machines table update packet");
        close(sockfd);
        throw FatalErrorException();
    }

    while(true) {
        if (!state->getSelf().isManager)
        {
            std::this_thread::sleep_for(std::chrono::seconds(IS_NOT_MANAGER_SLEEP_IN_SEC));
            continue;
        }

        throwExceptionIfNotAlive(state);

        std::list<Member> members = state->getManager()->getMembersWhenUpdatedAndLock();

        std::string message = {};

        for (Member member : members) {
            message += member.toMessage();
            message += LIST_SEPARATOR;
        }

        for (Member member : state->getManager()->getMembers()) 
        {
            if (member.isManager) {
                continue;
            }

            server = gethostbyname(member.ipv4.c_str());
            if (server == NULL) {
                printWarning("Fail to find host for " + member.ipv4 + " while sending machines table update packet");
                continue;
            }	
                
            serv_addr.sin_family = AF_INET;     
            serv_addr.sin_port = htons(MACHINE_TABLE_REPLIER_PORT);    
            serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
            bzero(&(serv_addr.sin_zero), 8);  

            n = sendto(sockfd, message.c_str(), message.length(), 0, (const struct sockaddr *) &serv_addr, sizeof(struct sockaddr_in));
            if (n < 0) 
            {
                printWarning("Fail to send machines table update packet for " + member.ipv4);
            }
            else
            {
                length = sizeof(struct sockaddr_in);

                n = recvfrom(sockfd, buffer, MACHINE_TABLE_REPLICATOR_PACKAGE_SIZE, 0, (struct sockaddr *) &from, &length);
                if (n < 0) 
                {
                    printWarning("Fail to receive machines table update packet for " + member.ipv4);
                }
            }
        }

        state->getManager()->unlock();
    }  
}

void MachinesTableReplicatorSenderProcess(State* state)
{
    try 
    {
        listenAndSendUpdates(state);
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