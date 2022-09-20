#include "election.h"

const int BUFFER_SIZE = 256;
const int MONITORING_SLEEP_IN_SEC = 2;
const int MONITORING_TIMEOUT_IN_SEC = 2;
const int NEW_MANAGER_MESSAGE_TIMEOUT_IN_SEC = 2;
const int TIME_TO_AWAIT_FOR_NEW_MANAGER_IN_SEC = 60;

void sendNewManagerMessage(State* state)
{
    for (Member member : state->getManager()->getMembers()) 
    {
        int sockfd, n, ret;
        struct sockaddr_in serv_addr;
        struct hostent *server;
        char buffer[BUFFER_SIZE];
        struct timeval tv;

	    server = gethostbyname(member.ipv4.c_str());
        if (server == NULL) 
        {
            printLine("ERROR, no such host");
            throw ElectionException();
        }
        
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
        {
            printLine("ERROR opening socket");
            throw ElectionException();
        }

        tv.tv_sec = NEW_MANAGER_MESSAGE_TIMEOUT_IN_SEC;
        tv.tv_usec = 0;
        ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
        if (ret)
        {
            printLine("Error: Could not set socket timeout");
            close(sockfd);
            throw ElectionException();
        }
    
        serv_addr.sin_family = AF_INET;     
        serv_addr.sin_port = htons(NEW_MANAGER_ANNOUNCEMENT_PORT);    
        serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
        bzero(&(serv_addr.sin_zero), 8);     
	
	    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        {
            printLine("ERROR connecting with host " + member.ipv4);
            close(sockfd);
            break;
        }
            
        std::string message = "I'm the new manager!";
    
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) 
        {
            printLine("ERROR writing to socket");
            close(sockfd);
            throw ElectionException();
        }

        bzero(buffer, BUFFER_SIZE);
	
        n = read(sockfd, buffer, BUFFER_SIZE);
        if (n < 0) 
        {
            printLine("No message received from " + member.ipv4);
        }

        close(sockfd);
    }
}

// TODO: 
// Escutar pelo pacote "new-manager-packet" durante TIME_TO_AWAIT_FOR_NEW_MANAGER_IN_SEC
// Se receber uma requisição: atualizar sua tabela de membros, colocando para true a flag isManager do novo membro manager e retornar true
// Se não: retornar false
bool verifyIfNewManagerWasDefined(State* state)
{
    int sockfd, newsockfd, n;
    socklen_t clilen;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serv_addr, cli_addr;
        
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        printLine("ERROR opening socket");
        throw ElectionException();
    }
            
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(NEW_MANAGER_ANNOUNCEMENT_PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(serv_addr.sin_zero), 8);     
        
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    {
        printLine("ERROR on binding");
        close(sockfd);
        throw ElectionException();
    }
        
    listen(sockfd, 5);
            
    clilen = sizeof(struct sockaddr_in);
    if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1) 
    {
        printLine("ERROR on accept");
        close(sockfd);
        throw ElectionException();
    }
        
    bzero(buffer, BUFFER_SIZE);
        
    n = read(newsockfd, buffer, BUFFER_SIZE);
    if (n < 0) 
    {
        close(newsockfd);
        close(sockfd);
        return false;
    }

    std::string answerMessage = "Got it!";
    n = write(newsockfd, answerMessage.c_str(), BUFFER_SIZE);
    if (n < 0)
    {
        printLine("ERROR writing to socket");
    }

    close(newsockfd);
    close(sockfd);
    return true;
}

bool shouldBeNewManager(State* state)
{
    bool answer = true;
    for (Member member : state->getManager()->getMembers()) 
    {
        int sockfd, n, ret;
        struct sockaddr_in serv_addr;
        struct hostent *server;
        char buffer[BUFFER_SIZE];
        struct timeval tv;

	    server = gethostbyname(member.ipv4.c_str());
        if (server == NULL) 
        {
            printLine("ERROR, no such host");
            throw ElectionException();
        }
        
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
        {
            printLine("ERROR opening socket");
            throw ElectionException();
        }

        tv.tv_sec = MONITORING_TIMEOUT_IN_SEC;
        tv.tv_usec = 0;
        ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
        if (ret)
        {
            printLine("Error: Could not set socket timeout");
            close(sockfd);
            throw ElectionException();
        }
    
        serv_addr.sin_family = AF_INET;     
        serv_addr.sin_port = htons(ELECTION_PORT);    
        serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
        bzero(&(serv_addr.sin_zero), 8);     
	
	    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        {
            printLine("ERROR connecting with host " + member.ipv4);
            close(sockfd);
            break;
        }
            
        std::string message = std::to_string(state->self.pid);
    
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) 
        {
            printLine("ERROR writing to socket");
            close(sockfd);
            throw ElectionException();
        }

        bzero(buffer, BUFFER_SIZE);
	
        n = read(sockfd, buffer, BUFFER_SIZE);
        if (n < 0) 
        {
            printLine("No message received from " + member.ipv4);
            close(sockfd);
        }
        else
        {
            close(sockfd);
            answer = false;
        }
    }
    return answer;
}

bool tryDoElection(State* state)
{
    try 
    {
        state->isDoingElection = true;
        state->getManager()->fireMemberManager();
            
        if (shouldBeNewManager(state))
        {
            sendNewManagerMessage(state);
            state->isDoingElection = false;
            return true;
        } 
        else 
        {
            bool electionDone = verifyIfNewManagerWasDefined(state);
            state->isDoingElection = false;
            return electionDone;
        }
    }
    catch (ElectionException& e) 
    {  
        state->isDoingElection = false;
        return false;
    }
}

void ElectionProcess(State* state) 
{
    while(true) 
    {
        throwExceptionIfNotAlive(state);

        int failToContactManagerCount = state->getFailToContactManagerCountWhenUpdated();

        if (failToContactManagerCount >= FAIL_TO_CONTACT_MANAGER_COUNT_THRESHOLD) 
        {
            bool electionDone = false;
            while(!electionDone)
            {
                electionDone = tryDoElection(state);
            }
            state->resetAndUnlockFailToContactManagerCountLock();
        }
        else 
        {
            state->unlockFailToContactManagerCountLock();
        }

        std::this_thread::sleep_for(std::chrono::seconds(MONITORING_SLEEP_IN_SEC));
    }
}