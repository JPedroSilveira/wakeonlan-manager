#include "manager-life-monitor-listener.h"

void listenManagerLifeMonitorPackets(State* state)
{
    int sockfd, n;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	char buffer[MANAGER_LIFE_MONITOR_PACKET_SIZE];
		
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        printFatalError("Fail to open socket to receive manager life monitor packets");
        throw FatalErrorException();
    }

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(MANAGER_LIFE_MONITOR_PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);    
	 
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0) {
        printFatalError("Fail to bind socket to receive manager life monitor packets");
        close(sockfd);
        throw FatalErrorException();
    }
		
    clilen = sizeof(struct sockaddr_in);
	
    while (true) {
        throwExceptionIfNotAlive(state);

        n = recvfrom(sockfd, buffer, MANAGER_LIFE_MONITOR_PACKET_SIZE, 0, (struct sockaddr *) &cli_addr, &clilen);
        if (n < 0) 
        {
            printWarning("Fail to receive manager life monitor packet");
        }
        else 
        {                 
            std::string message = "I am alive!";
  
            n = sendto(sockfd, message.c_str(), message.length(), 0, (struct sockaddr *) &cli_addr, sizeof(struct sockaddr));
            if (n  < 0) 
            {
                printWarning("Fail to answer manager life monitor packet");
            }      
        }
    }
}

void ManagerLifeMonitorListenerProcess(State* state)
{
    try
    {
        listenManagerLifeMonitorPackets(state);
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