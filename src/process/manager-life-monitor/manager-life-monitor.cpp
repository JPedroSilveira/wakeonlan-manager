#include "manager-life-monitor.h"

const int BUFFER_SIZE = 256;
const int CONNECTION_TIMEOUT_IN_SEC = 5;
const int MONITORING_SLEEP_IN_SEC = 10;

void sendManagerIsAlivePacket(State* state)
{
    while(true) {
        throwExceptionIfNotAlive(state);

        try 
        {
            Member member = state->getManagerMember();
            int sockfd, n, ret;
            unsigned int length;
            struct sockaddr_in serv_addr, from;
            struct hostent *server;
            char buffer[BUFFER_SIZE];
            struct timeval tv;
            
            if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
                printLine("ERROR opening socket");
            }

            tv.tv_sec = CONNECTION_TIMEOUT_IN_SEC;
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
                return;
            }	
                
            serv_addr.sin_family = AF_INET;     
            serv_addr.sin_port = htons(MACHINE_LIFE_MONITOR_PORT);    
            serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
            bzero(&(serv_addr.sin_zero), 8);  

            std::string message = "Are you alive?";

            n = sendto(sockfd, message.c_str(), message.length(), 0, (const struct sockaddr *) &serv_addr, sizeof(struct sockaddr_in));
            if (n < 0) 
            {
                printLine("ERROR sending sleep status request");
            }
            else
            {
                length = sizeof(struct sockaddr_in);

                n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &from, &length);
                if (n < 0) {
                    printLine("ERROR receiving sleep status request");
                    state->increaseFailToContactManagerCount();
                } else {
                    state->resetFailToContactManagerCount();
                }
            }
            
            close(sockfd);
        }
        catch (ManagerNotFoundException& e) 
        {     
            printLine("Manager not found");
        }
 
        std::this_thread::sleep_for(std::chrono::seconds(MONITORING_SLEEP_IN_SEC));
    }
}

void listenManagerIsAlivePacket(State* state)
{
    int sockfd, n;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	char buffer[BUFFER_SIZE];
		
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        printLine("ERROR opening socket");
    }

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(MANAGER_LIFE_MONITOR_PORT);
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
            printLine("ERROR on receiving manager is alive request");
        } else {                    
            std::string message = "I am alive!";
  
            n = sendto(sockfd, message.c_str(), message.length(), 0, (struct sockaddr *) &cli_addr, sizeof(struct sockaddr));
            if (n  < 0) {
                printLine("ERROR on answering manager is alive request");
            }      
        }
    }
	
    close(sockfd);
}

void ManagerLifeMonitorProcess(State* state)
{
    if (state->self.isManager)
    {
        listenManagerIsAlivePacket(state);
    }
    else
    {
        sendManagerIsAlivePacket(state);
    }
}