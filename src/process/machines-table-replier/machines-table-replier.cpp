#include "machines-table-replier.h"

const int BUFFER_SIZE = 65000;
const int SLEEP_IN_SEC = 5;
const int SOCKET_TIMEOUT_IN_SEC = 2;
const std::string LIST_SEPARATOR = ";";

void listenAndSendUpdates(State* state)
{
    while(true)
    {
        throwExceptionIfNotAlive(state);

        std::list<Member> members = state->getManager()->getMembersWhenUpdatedAndLock();

        std::string message = {};

        for (Member member : members) {
            message += member.toMessage();
            message += LIST_SEPARATOR;
        }

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

            tv.tv_sec = SOCKET_TIMEOUT_IN_SEC;
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
            serv_addr.sin_port = htons(MACHINE_TABLE_REPLIER_PORT);    
            serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
            bzero(&(serv_addr.sin_zero), 8);  

            n = sendto(sockfd, message.c_str(), message.length(), 0, (const struct sockaddr *) &serv_addr, sizeof(struct sockaddr_in));
            if (n < 0) {
                printLine("ERROR sending reply state request");
            }
                
            length = sizeof(struct sockaddr_in);

            n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &from, &length);
            if (n < 0) {
                printLine("ERROR receiving reply state request");
            }
            
            close(sockfd);
        }

        state->getManager()->unlock();

        std::this_thread::sleep_for(std::chrono::seconds(SLEEP_IN_SEC));
    }
}

void receiveAndSaveUpdates(State* state)
{
    int sockfd, n;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	char buffer[BUFFER_SIZE];
		
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        printLine("ERROR opening socket");
    }

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(MACHINE_TABLE_REPLIER_PORT);
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
            printLine("ERROR on receiving reply state request");
        } else {
            printLine("Reply state received from manager");

            std::list<std::string> membersMessages = {};

            std::string receivedMessage = std::string(buffer);

            std::istringstream receivedMessageStream(receivedMessage);

            while (!receivedMessageStream.eof()) {
                std::string member = {};    
                std::getline(receivedMessageStream, member, LIST_SEPARATOR.at(0));
                member.erase(std::remove(member.begin(), member.end(), '\n'), member.end());
                membersMessages.push_back(member);
            }

            state->getManager()->setMembersByMessages(membersMessages);
                    
            std::string message = "Got it!";
  
            n = sendto(sockfd, message.c_str(), message.length(), 0,(struct sockaddr *) &cli_addr, sizeof(struct sockaddr));
            if (n  < 0) {
                printLine("ERROR on answering reply state request");
            }      
        }
    }
	
    close(sockfd);
}

void MachinesTableReplierProcess(State* state)
{
    if (state->self.isManager)
    {
        listenAndSendUpdates(state);
    }
    else
    {
        receiveAndSaveUpdates(state);
    }
}