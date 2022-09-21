#include "machines-finder-replier.h"

void receiveMachinesFinderPackets(State* state)
{
    int sockfd, n;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[MACHINE_FINDER_PACKAGE_SIZE];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printFatalError("Fail to open socket to receive machines finder packets");
        throw FatalErrorException();
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(MACHINE_FINDER_PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(serv_addr.sin_zero), 8);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0)
    {
        printFatalError("Fail to bind socket to receive machines finder packets");
        close(sockfd);
        throw FatalErrorException();
    }

    clilen = sizeof(struct sockaddr_in);

    while (true)
    {
        throwExceptionIfNotAlive(state);
        
        n = recvfrom(sockfd, buffer, MACHINE_FINDER_PACKAGE_SIZE, 0, (struct sockaddr *)&cli_addr, &clilen);
        if (n < 0)
        {
            printWarning("Fail to receive machines finder packet");
        }
        else
        {
            std::string response = state->getSelf().toMessage();

            n = sendto(sockfd, response.c_str(), response.length(), 0, (struct sockaddr *)&cli_addr, sizeof(struct sockaddr));
            if (n < 0)
            {
                printWarning("Fail to answer machines finder packet");
            }
        }
    }
}

void MachinesFinderReplierProcess(State* state)
{
    try 
    {
        receiveMachinesFinderPackets(state);
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
