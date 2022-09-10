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
#include "./process/machines-life-monitor/machines-life-monitor.h"
#include "./process/machines-update-printer/machines-update-printer.h"
#include "./process/machines-table-replier/machines-table-replier.h"

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

State state = State();

int main(int argc, char *argv[])
{
    std::vector<std::string> args(argv, argv + argc);

    state.self.setIsManager(args);

    Process userInputMonitorProcess{UserInputMonitorProcess};
    userInputMonitorProcess.start(&state);

    Process machinesLifeMonitorProcess{MachinesLifeMonitorProcess};
    machinesLifeMonitorProcess.start(&state);

    Process machinesFinderProcess{MachinesFinderProcess};
    machinesFinderProcess.start(&state);

    Process machinesUpdatePrinter{MachinesUpdatePrinter};
    machinesUpdatePrinter.start(&state);

    Process machinesTableReplierProcess{MachinesTableReplierProcess};
    machinesTableReplierProcess.start(&state);

    do
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    } while (
        userInputMonitorProcess.getStatus() != std::future_status::ready &&
        machinesLifeMonitorProcess.getStatus() != std::future_status::ready &&
        machinesFinderProcess.getStatus() != std::future_status::ready && 
        machinesUpdatePrinter.getStatus() != std::future_status::ready && 
        machinesTableReplierProcess.getStatus() != std::future_status::ready
    );

    state.kill();

    printLine("Program ended!");

    return 0;
}
