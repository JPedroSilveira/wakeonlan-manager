#include "machines-life-monitor.h"

const int BUFFER_SIZE = 256;
const int MONITORING_SLEEP_IN_SEC = 5;
const int MONITORING_TIMEOUT_IN_SEC = 2;

void sendMonitoringPackages(State* state)
{
    while(true) {
        throwExceptionIfNotAlive(state);

        for (Member member : state->getManager()->getMembers()) 
        {
            int sockfd, n, ret;
            unsigned int length;
            struct sockaddr_in serv_addr, from;
            struct hostent *server;
            char buffer[BUFFER_SIZE];
            struct timeval tv;
            
            if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
                printLine("ERROR opening socket");
            }

            // Set socket timeout
            tv.tv_sec = MONITORING_TIMEOUT_IN_SEC;
            tv.tv_usec = 0;
            ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
            if (ret)
            {
                printLine("Error: Could not set socket timeout");
                close(sockfd);
                return;
            }

            server = gethostbyname(member.ipv4.c_str());
            if (server == NULL) {
                printLine("ERROR, no such host " + member.hostname);
                exit(0);
            }	
                
            serv_addr.sin_family = AF_INET;     
            serv_addr.sin_port = htons(MACHINE_LIFE_MONITOR_PORT);    
            serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
            bzero(&(serv_addr.sin_zero), 8);  

            std::string message = "Are you alive?";

            n = sendto(sockfd, message.c_str(), message.length(), 0, (const struct sockaddr *) &serv_addr, sizeof(struct sockaddr_in));
            if (n < 0) {
                printLine("ERROR sending sleep status request");
            }
                
            length = sizeof(struct sockaddr_in);

            n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &from, &length);
            if (n < 0) {
                printLine("ERROR receiving sleep status request");
                state->getManager()->updateToSleepingByIPv4(member.ipv4);
            } else {
                state->getManager()->updateToAwakeByIPv4(member.ipv4);
            }
            
            close(sockfd);
        }
                
        std::this_thread::sleep_for(std::chrono::seconds(MONITORING_SLEEP_IN_SEC));
    }
}

void sendMonitoringPackagesMock(State* state) {
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

void listenMonitoringPackages(State* state)
{
    int sockfd, n;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	char buffer[BUFFER_SIZE];
		
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        printLine("ERROR opening socket");
    }

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(MACHINE_LIFE_MONITOR_PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);    
	 
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0) {
        printLine("ERROR on binding");
    }
		
    clilen = sizeof(struct sockaddr_in);
	
    while (true) {
        throwExceptionIfNotAlive(state);

        n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &cli_addr, &clilen);
        if (n < 0) {
            printLine("ERROR on receiving sleep status request");
        } else {
            printLine("Sleep status request received");
                    
            std::string message = "I am alive!";
  
            n = sendto(sockfd, message.c_str(), message.length(), 0,(struct sockaddr *) &cli_addr, sizeof(struct sockaddr));
            if (n  < 0) {
                printLine("ERROR on answering sleep status request");
            }      
        }
    }
	
    close(sockfd);
}

void MachinesLifeMonitorProcess(State* state)
{
    if (state->self.isManager)
    {
        sendMonitoringPackages(state);
    }
    else
    {
        listenMonitoringPackages(state);
    }   
}