#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <vector>

#include "./utils/print-utils/print-utils.h"
#include "./model/member/member.h"
#include "./machine-info/machine-info.h"
#include "./model/state/state.h"
#include "./model/process/process.h"
#include "./process/user-input-monitor/user-input-monitor.h"
#include "./process/discover/discover.h"
#include "./process/monitor/monitor.h"
#include "./process/listener/listener.h"

State state = State();

int main(int argc, char *argv[])
{
    std::vector<std::string> args(argv, argv + argc);

    state.self.setIsManager(args);

    Process userInputMonitorProcess{UserInputMonitorProcess};
    userInputMonitorProcess.start(&state);

    Process monitorProcess{MonitorProcess};
    monitorProcess.start(&state);

    Process discoveryProcess{DiscoverProcess};
    discoveryProcess.start(&state);

    Process listenerProcess{ListenerProcess};
    listenerProcess.start(&state);

    do
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    } while (
        userInputMonitorProcess.getStatus() != std::future_status::ready &&
        discoveryProcess.getStatus() != std::future_status::ready && 
        monitorProcess.getStatus() != std::future_status::ready &&
        listenerProcess.getStatus() != std::future_status::ready);

    state.kill();

    printLine("Program ended!");

    return 0;
}
