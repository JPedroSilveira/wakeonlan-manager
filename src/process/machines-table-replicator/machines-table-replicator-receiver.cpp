#include "machines-table-replicator-receiver.h"

const std::string LIST_SEPARATOR = ";";

void receiveAndSaveUpdates(State* state)
{
    int sockfd, n;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	char buffer[MACHINE_TABLE_REPLICATOR_PACKAGE_SIZE];
		
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
    {
        printFatalError("Fail to open socket to receive machines table update packets");
        throw FatalErrorException();    
    }

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(MACHINE_TABLE_REPLIER_PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);    
	 
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0) {
        printFatalError("Fail to bind socket to receive machines table update packets");
        close(sockfd);
        throw FatalErrorException();
    }
		
    clilen = sizeof(struct sockaddr_in);
	
    while (true) {
        throwExceptionIfNotAlive(state);

        n = recvfrom(sockfd, buffer, MACHINE_TABLE_REPLICATOR_PACKAGE_SIZE, 0, (struct sockaddr *) &cli_addr, &clilen);
        if (n < 0) 
        {
            printWarning("Fail to receive answer from machines table update packet");
        } 
        else 
        {
            std::string answer = "Got it!";

            n = sendto(sockfd, answer.c_str(), answer.length(), 0,(struct sockaddr *) &cli_addr, sizeof(struct sockaddr));
            if (n  < 0) {
                printWarning("Fail to send confirmation message for machines table update packet");
            }

            std::list<std::string> membersMessages = {};

            std::string receivedMessage = std::string(buffer);

            std::istringstream receivedMessageStream(receivedMessage);

            while (!receivedMessageStream.eof()) {
                std::string member = {};    
                std::getline(receivedMessageStream, member, LIST_SEPARATOR.at(0));
                member.erase(std::remove(member.begin(), member.end(), '\n'), member.end());
                if (!member.empty()) {
                    membersMessages.push_back(member);
                }
            }

            for (std::string message : membersMessages) {
                Member member {};
                member.fromMessage(message);
                // printInfo("Received: " + member.ipv4 + " | " + std::to_string(member.getStatus()) + " | " + (member.isManager ? "true" : "false"));
            }

            state->getManager()->setMembersByMessages(membersMessages); 
        }
    }
}

void MachinesTableReplicatorReceiverProcess(State* state)
{
    try 
    {
        receiveAndSaveUpdates(state);
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