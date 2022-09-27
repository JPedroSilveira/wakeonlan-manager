#include "election-vote-request-listener.h"

void answerVoteRequests(State* state)
{
    int sockfd, newsockfd, n;
    socklen_t clilen;
    char buffer[ELECTION_VOTE_PACKET_SIZE];
    struct sockaddr_in serv_addr, cli_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        printFatalError("Fail to open socket to receive vote request packets");
        throw FatalErrorException();
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(ELECTION_VOTE_PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(serv_addr.sin_zero), 8);     

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    {
        printFatalError("Fail to bind socket to receive vote request packets");
        close(sockfd);
        throw FatalErrorException();
    }

    if (listen(sockfd, 5) < 0)
    {
        printFatalError("Fail to set the amount of pending connections to receive vote request packets");
        close(sockfd);
        throw FatalErrorException();
    }

    clilen = sizeof(struct sockaddr_in);

    while(true)
    {
        if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1) 
        {
            printError("Fail to accept connection to receive vote request packet");
            continue;
        }

        bzero(buffer, ELECTION_VOTE_PACKET_SIZE);

        n = read(newsockfd, buffer, ELECTION_VOTE_PACKET_SIZE);
        if (n <= 0) 
        {
            printError("Fail to read vote request packet");
        }
        else
        {
            std::string receivedMessage = std::string(buffer);
            int candidateProcessPID = std::stoi(receivedMessage);
            if (state->getSelf().pid > candidateProcessPID) 
            {
                std::string answerMessage = "You should not be the manager!";
                n = write(newsockfd, answerMessage.c_str(), ELECTION_VOTE_PACKET_SIZE);
                if (n < 0)
                {
                    printError("Fail to answer vote request packet");
                }
            }
        }
        close(newsockfd);
    }
}

void ElectionVoteRequestListenerProcess(State* state) 
{
    try 
    {
        answerVoteRequests(state);
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
