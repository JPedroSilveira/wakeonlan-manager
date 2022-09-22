#include "election-new-manager-sender.h"

const int NEW_MANAGER_PACKET_TIMEOUT_IN_SEC = 2;

bool sendNewManagerPacket(State* state)
{
    int sockfd, n, ret;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[ELECTION_NEW_MANAGER_PACKET_SIZE];
    struct timeval tv;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        printError("Fail to open socket to send new manager packets");
        return false;
    }

    tv.tv_sec = NEW_MANAGER_PACKET_TIMEOUT_IN_SEC;
    tv.tv_usec = 0;
    ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if (ret)
    {
        printError("Fail to set socket timeout to send new manager packets");
        close(sockfd);
        return false;
    }

    Member self = state->getSelf();
    
    for (Member member : state->getManager()->getMembers()) 
    {
        if (member.isEqual(self)) {
            continue;
        }

        printDebug("Sending new manager packet for " + member.ipv4);

	    server = gethostbyname(member.ipv4.c_str());
        if (server == NULL) 
        {
            printWarning("Fail to get hostname of " + member.ipv4 + " while sending new manager packet, so it will be ignored");
            continue;
        }
        
        serv_addr.sin_family = AF_INET;     
        serv_addr.sin_port = htons(ELECTION_NEW_MANAGER_ANNOUNCEMENT_PORT);    
        serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
        bzero(&(serv_addr.sin_zero), 8);     
	
	    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        {
            printWarning("Fail to connect with " + member.ipv4 + " while sending new manager packet, so it will be ignored");
            continue;
        }
            
        std::string message = state->getSelf().toMessage();
    
        n = write(sockfd, message.c_str(), message.length());
        if (n < 0) 
        {
            printWarning("Fail to write message to " + member.ipv4 + " while sending new manager packet, so it will be ignored");
            continue;
        }

        bzero(buffer, ELECTION_NEW_MANAGER_PACKET_SIZE);
	
        n = read(sockfd, buffer, ELECTION_NEW_MANAGER_PACKET_SIZE);
        if (n < 0) 
        {
            printWarning("No response received from " + member.ipv4 + " while sending new manager packet");
        }
    }

    return true;
}

void ElectionNewManagerSenderProcess(State* state)
{
    try 
    {
        while(true)
        {
            state->awaitForSendNewManagerMessageTrigger();
            bool success = sendNewManagerPacket(state);
            if (success) {
                state->getManager()->hireMemberManager(state->getSelf());
                state->finishElection();
            }
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