#include "election-maker.h"

const int ELECTION_VOTE_TIMEOUT_IN_SEC = 2;
const int AWAITING_TIME_FOR_ELECTION_TO_BE_COMPLETED_IN_SEC = 20;

bool doElectionByPid(State* state)
{
    int sockfd, n, ret;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[ELECTION_VOTE_PACKET_SIZE];
    struct timeval tv;

    bool wasElected = true;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        printError("Fail to open socket to send election packets");
        throw ElectionException();
    }

    tv.tv_sec = ELECTION_VOTE_TIMEOUT_IN_SEC;
    tv.tv_usec = 0;
    ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if (ret)
    {
        printError("Fail to set socket timeout to send election packets");
        close(sockfd);
        throw ElectionException();
    }

    Member self = state->getSelf();
    
    for (Member member : state->getManager()->getMembers()) 
    {
        if (member.isEqual(self)) {
            continue;
        }

        printDebug("Sending election packet for " + member.ipv4);

	    server = gethostbyname(member.ipv4.c_str());
        if (server == NULL) 
        {
            printWarning("Fail to get hostname for " + member.ipv4 + " during election, so it will be ignored");
            continue;
        }
        
        serv_addr.sin_family = AF_INET;     
        serv_addr.sin_port = htons(ELECTION_VOTE_PORT);    
        serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
        bzero(&(serv_addr.sin_zero), 8);     
	
	    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        {
            printWarning("Fail to connect with " + member.ipv4 + " during election, so it will be ignored");
            continue;
        }
            
        std::string message = std::to_string(state->getSelf().pid);
    
        n = write(sockfd, message.c_str(), message.length());
        if (n < 0) 
        {
            printWarning("Fail to write message to " + member.ipv4 + " during election, so it will be ignored");
            continue;
        }

        bzero(buffer, ELECTION_VOTE_PACKET_SIZE);
	
        n = read(sockfd, buffer, ELECTION_VOTE_PACKET_SIZE);
        if (n <= 0) 
        {
            printDebug("No response received from " + member.ipv4 + " during election");
        }
        else
        {
            printDebug("Response received from " + member.ipv4 + ", this member will not be the new manager");
            wasElected = false;
            break;
        }
    }
    return wasElected;
}

bool tryToBeElectedForManager(State* state)
{
    while(true) 
    {
        try 
        {       
            return doElectionByPid(state);
        }
        catch (ElectionException& e) 
        {  
            printWarning("Attempt to be elected failed with unexpected error, trying again...");
        }
    }
}

void ElectionMakerProcess(State* state) 
{
    try
    {
        while(true) {
            state->awaitForStartElectionTrigger();
            while(state->isDoingElection()) {
                printDebug("Election attempt started");
                state->getManager()->fireMemberManager();
                printDebug("Current manager fired, trying to be elected");
                bool wasElected = tryToBeElectedForManager(state);
                if (wasElected)
                {
                    printDebug("Member was elected, should send victory messages now");
                    state->triggerSendNewManagerMessage();
                }
                else
                {
                    printDebug("Member was NOT elected, should await for the winner now");
                }
                std::this_thread::sleep_for(std::chrono::seconds(AWAITING_TIME_FOR_ELECTION_TO_BE_COMPLETED_IN_SEC));
            }
            printDebug("Election done!");
        }
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
