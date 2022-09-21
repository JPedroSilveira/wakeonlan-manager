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
#include "./process/machines-finder/machines-finder.h"
#include "./process/machines-finder/machines-finder-replier.h"
#include "./process/machines-life-monitor/machines-life-monitor.h"
#include "./process/machines-life-monitor/machines-life-monitor-replier.h"
#include "./process/machines-update-printer/machines-update-printer.h"
#include "./process/machines-table-replicator/machines-table-replicator-sender.h"
#include "./process/machines-table-replicator/machines-table-replicator-receiver.h"
#include "./process/manager-life-monitor/manager-life-monitor.h"
#include "./process/election/election-listener.h"

State state = State();

int main(int argc, char *argv[])
{
    std::vector<std::string> args(argv, argv + argc);

    state.setIsManagerByArgs(args);

    Process userInputMonitorProcess{UserInputMonitorProcess};
    userInputMonitorProcess.start(&state);

    Process machinesLifeMonitorReplierProcess{MachinesLifeMonitorReplierProcess};
    machinesLifeMonitorReplierProcess.start(&state);
    Process machinesLifeMonitorProcess{MachinesLifeMonitorProcess};
    machinesLifeMonitorProcess.start(&state);

    Process machinesFinderReplierProcess{MachinesFinderReplierProcess};
    machinesFinderReplierProcess.start(&state);
    Process machinesFinderProcess{MachinesFinderProcess};
    machinesFinderProcess.start(&state);

    Process machinesUpdatePrinterProcess{MachinesUpdatePrinter};
    machinesUpdatePrinterProcess.start(&state);

    Process machinesTableReplicatorReceiverProcess{MachinesTableReplicatorReceiverProcess};
    machinesTableReplicatorReceiverProcess.start(&state);
    Process machinesTableReplicatorSenderProcess{MachinesTableReplicatorSenderProcess};
    machinesTableReplicatorSenderProcess.start(&state); 

    // Process managerLifeMonitorProcess{ManagerLifeMonitorProcess};
    // managerLifeMonitorProcess.start(&state);

    // Process electionListener{ElectionListenerProcess};
    // electionListener.start(&state);

    do
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    } 
    while (
        userInputMonitorProcess.getStatus() != std::future_status::ready
        && machinesLifeMonitorReplierProcess.getStatus() != std::future_status::ready
        && machinesLifeMonitorProcess.getStatus() != std::future_status::ready
        && machinesFinderReplierProcess.getStatus() != std::future_status::ready
        && machinesFinderProcess.getStatus() != std::future_status::ready
        && machinesUpdatePrinterProcess.getStatus() != std::future_status::ready 
        && machinesTableReplicatorReceiverProcess.getStatus() != std::future_status::ready
        && machinesTableReplicatorSenderProcess.getStatus() != std::future_status::ready 
        // && managerLifeMonitorProcess.getStatus() != std::future_status::ready 
        // && electionListener.getStatus() != std::future_status::ready
    );

    state.kill();

    printWarning("Program ending!");

    return 0;
}
