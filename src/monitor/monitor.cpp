#include "monitor.h"

const int PORT = 4001;
const int BUFFER_SIZE = 256;
const int MONITORING_SLEEP_IN_SEC = 10;
const int MONITORING_TIMEOUT_IN_SEC = 1;

void sendMonitoringPackages(State* state)
{
    while(true) {
        for (Member member : state->membersManager.getMembers()) 
        {
            int sockfd, n;
            unsigned int length;
            struct sockaddr_in serv_addr, from;
            struct hostent *server;
            
            char buffer[BUFFER_SIZE];

            server = gethostbyname(member.ipv4.c_str());
            if (server == NULL) {
                printLine("ERROR, no such host " + member.hostname);
                exit(0);
            }	
            
            if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
                printLine("ERROR opening socket");
            }

            // Set socket timeout
            struct timeval tv;
            tv.tv_sec = MONITORING_TIMEOUT_IN_SEC;
            tv.tv_usec = 0;
            int ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
            if (ret)
            {
                printf("Error: Could not set socket timeout");
                close(sockfd);
                return;
            }
                
            serv_addr.sin_family = AF_INET;     
            serv_addr.sin_port = htons(PORT);    
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
                state->membersManager.updateToSleepingByIPv4(member.ipv4);
            } else {
                state->membersManager.updateToAwakeByIPv4(member.ipv4);
            }
            
            close(sockfd);
        }
        
        printMembersTable(state->membersManager);
        
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
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);    
	 
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0) {
        printLine("ERROR on binding");
    }
		
    clilen = sizeof(struct sockaddr_in);
	
    while (true) {
	n = recvfrom(sockfd, buffer, 256, 0, (struct sockaddr *) &cli_addr, &clilen);
	if (n < 0) {
		printLine("ERROR on receiving sleep status request");
        } else {
            printLine("Sleep status request received");
                    
            std::string message = "I am alive!";
  
            n = sendto(sockfd, message.c_str(), message.length(), 0,(struct sockaddr *) &cli_addr, sizeof(struct sockaddr));
            if (n  < 0) {
                printf("ERROR on answering sleep status request");
            }      
        }
    }
	
    close(sockfd);
}

void MonitorProcess(State* state)
{
    if (state->self.isManager)
    {
	printLine("Sou Manager");
        sendMonitoringPackages(state);
    }
    else
    {
	printLine("Sou Client");
        listenMonitoringPackages(state);
    }   
}
