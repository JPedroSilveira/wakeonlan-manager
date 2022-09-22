#include "election-listener.h"

const int BUFFER_SIZE = 256;

void ElectionListenerProcess(State* state) 
{
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    throwExceptionIfNotAlive(state);

    int sockfd, newsockfd, n;
    socklen_t clilen;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serv_addr, cli_addr;
        
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        printLine("ERROR opening socket");
        return;
    }
            
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(ELECTION_PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(serv_addr.sin_zero), 8);     
        
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    {
        printLine("ERROR on binding");
        close(sockfd);
        return;
    }
        
    while(true) 
    {   
        listen(sockfd, 5);
            
        clilen = sizeof(struct sockaddr_in);
        if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1) 
        {
            printLine("ERROR on accept");
            break;
        }
        
        bzero(buffer, BUFFER_SIZE);
        
        n = read(newsockfd, buffer, BUFFER_SIZE);
        if (n < 0) 
        {
            printLine("ERROR reading from socket");
            break;
        }

        std::string receivedMessage = std::string(buffer);
        int candidateProcessPID = std::stoi(receivedMessage);

        if (state->getSelf().pid > candidateProcessPID) 
        {
            std::string answerMessage = "You should not be the manager!";
            n = write(newsockfd, answerMessage.c_str(), BUFFER_SIZE);
            if (n < 0)
            {
                printLine("ERROR writing to socket");
                break;
            }
            if (!state->isDoingElection()) {
                state->increaseFailtToContactManagerCountBy(FAIL_TO_CONTACT_MANAGER_COUNT_THRESHOLD);
            }
        }

        close(newsockfd);
    }
    close(sockfd);
}