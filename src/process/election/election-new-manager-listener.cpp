#include "election-new-manager-listener.h"

void receiveNewManagerRequest(State* state)
{
    int sockfd, newsockfd, n;
    socklen_t clilen;
    char buffer[ELECTION_NEW_MANAGER_PACKET_SIZE];
    struct sockaddr_in serv_addr, cli_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        printFatalError("Fail to open socket to receive new manager packets");
        throw FatalErrorException();
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(ELECTION_NEW_MANAGER_ANNOUNCEMENT_PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(serv_addr.sin_zero), 8);     

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    {
        printFatalError("Fail to bind socket to receive new manager packets");
        close(sockfd);
        throw FatalErrorException();
    }

    if (listen(sockfd, 5) < 0)
    {
        printFatalError("Fail to set the amount of pending connections to receive new manager packets");
        close(sockfd);
        throw FatalErrorException();
    }

    clilen = sizeof(struct sockaddr_in);

    while(true)
    {
        if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1) 
        {
            printError("Fail to accept connection to receive new manager packet");
            continue;
        }

        bzero(buffer, ELECTION_NEW_MANAGER_PACKET_SIZE);

        n = read(newsockfd, buffer, ELECTION_NEW_MANAGER_PACKET_SIZE);
        if (n < 0) 
        {
            printError("Fail to read new manager packet");
        }
        else
        {
            std::string answerMessage = "Awesome";
            n = write(newsockfd, answerMessage.c_str(), ELECTION_NEW_MANAGER_PACKET_SIZE);
            if (n < 0)
            {
                printError("Fail to answer new manager packet");
            }

            std::string receivedMessage = std::string(buffer);
            Member member {};
            member.fromMessage(receivedMessage);

            printDebug("New manager message received from " + member.ipv4);

            state->getManager()->hireMemberManager(member);

            state->finishElection();
        }
        close(newsockfd);
    }
}

void ElectionNewManagerListenerProcess(State* state)
{
    try 
    {
        receiveNewManagerRequest(state);
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