/**************************************************
 * A distributed system that monitors the state of 
 * machines on a LAN and allows users to wake up 
 * other machines when necessary.
 * Requirements:
 * - At least two machines
 * - C++ 11
 * - wakeonlan command line tool that accepts the command: 
 * `wakeonlan <COMPUTER_MAC>` to send the magic packet
 * 
 * Developed by:
 *  - João Pedro Silveira e Silva
 *  - Rafael Trevisan
 *  - Victoria Duarte
 *  - Alice Carra
 **************************************************/

#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <vector>

#include "./utils/print-utils/print-utils.h"
#include "./entity/member/member.h"
#include "./utils/machine-info-utils/machine-info-utils.h"
#include "./entity/state/state.h"
#include "./entity/process/process.h"
#include "./process/user-input-monitor/user-input-monitor.h"
#include "./process/members-finder/members-finder-sender.h"
#include "./process/members-finder/members-finder-replier.h"
#include "./process/members-life-monitor/members-life-monitor.h"
#include "./process/members-life-monitor/members-life-monitor-replier.h"
#include "./process/members-update-printer/members-update-printer.h"
#include "./process/members-table-replicator/members-table-replicator-sender.h"
#include "./process/members-table-replicator/members-table-replicator-receiver.h"
#include "./process/manager-life-monitor/manager-life-monitor-sender.h"
#include "./process/manager-life-monitor/manager-life-monitor-listener.h"
#include "./process/election/election-new-manager-sender.h"
#include "./process/election/election-new-manager-listener.h"
#include "./process/election/election-vote-request-listener.h"
#include "./process/election/election-maker.h"
#include "./process/election/election-starter.h"

State state = State();

int main(int argc, char *argv[])
{
    std::vector<std::string> args(argv, argv + argc);

    state.setIsManagerByArgs(args);

    Process userInputMonitorProcess{UserInputMonitorProcess};
    userInputMonitorProcess.start(&state);

    Process membersLifeMonitorReplierProcess{MembersLifeMonitorReplierProcess};
    membersLifeMonitorReplierProcess.start(&state);
    Process membersLifeMonitorProcess{MembersLifeMonitorProcess};
    membersLifeMonitorProcess.start(&state);

    Process membersFinderReplierProcess{MembersFinderReplierProcess};
    membersFinderReplierProcess.start(&state);
    Process membersFinderSenderProcess{MembersFinderSenderProcess};
    membersFinderSenderProcess.start(&state);

    Process membersUpdatePrinterProcess{MembersUpdatePrinter};
    membersUpdatePrinterProcess.start(&state);

    Process membersTableReplicatorReceiverProcess{MembersTableReplicatorReceiverProcess};
    membersTableReplicatorReceiverProcess.start(&state);
    Process membersTableReplicatorSenderProcess{MembersTableReplicatorSenderProcess};
    membersTableReplicatorSenderProcess.start(&state); 

    Process managerLifeMonitorListenerProcess{ManagerLifeMonitorListenerProcess};
    managerLifeMonitorListenerProcess.start(&state);
    Process managerLifeMonitorSenderProcess{ManagerLifeMonitorSenderProcess};
    managerLifeMonitorSenderProcess.start(&state);

    Process electionNewManagerSenderProcess{ElectionNewManagerSenderProcess};
    electionNewManagerSenderProcess.start(&state);
    Process electionNewManagerListenerProcess{ElectionNewManagerListenerProcess};
    electionNewManagerListenerProcess.start(&state);
    Process electionVoteRequestListenerProcess{ElectionVoteRequestListenerProcess};
    electionVoteRequestListenerProcess.start(&state);
    Process electionMakerProcess{ElectionMakerProcess};
    electionMakerProcess.start(&state);
    Process electionStarterProcess{ElectionStarterProcess};
    electionStarterProcess.start(&state);

    do
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    } 
    while (
        userInputMonitorProcess.getStatus() != std::future_status::ready
        && membersLifeMonitorReplierProcess.getStatus() != std::future_status::ready
        && membersLifeMonitorProcess.getStatus() != std::future_status::ready
        && membersFinderReplierProcess.getStatus() != std::future_status::ready
        && membersFinderSenderProcess.getStatus() != std::future_status::ready
        && membersUpdatePrinterProcess.getStatus() != std::future_status::ready 
        && membersTableReplicatorReceiverProcess.getStatus() != std::future_status::ready
        && membersTableReplicatorSenderProcess.getStatus() != std::future_status::ready 
        && managerLifeMonitorListenerProcess.getStatus() != std::future_status::ready
        && managerLifeMonitorSenderProcess.getStatus() != std::future_status::ready 
        && electionNewManagerSenderProcess.getStatus() != std::future_status::ready
        && electionNewManagerListenerProcess.getStatus() != std::future_status::ready
        && electionVoteRequestListenerProcess.getStatus() != std::future_status::ready
        && electionMakerProcess.getStatus() != std::future_status::ready
        && electionStarterProcess.getStatus() != std::future_status::ready
    );

    state.kill();

    printWarning("Program ending!");

    return 0;
}
