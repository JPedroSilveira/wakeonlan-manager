#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <vector>

#include "./output/output.h"
#include "./model/member/member.h"
#include "./machine-manager/machine-manager.h"
#include "./model/state/state.h"
#include "./model/process/process.h"
#include "./input/input.h"
#include "./discover/discover.h"
#include "./monitor/monitor.h"
#include "./listener/listener.h"

MembersManager membersManager = MembersManager{};
Member self = getMachineInfo();
State state = State(self, membersManager);

int main(int argc, char *argv[])
{
    std::vector<std::string> args(argv, argv + argc);

    state.self.setIsManager(args);

    Process inputProcess{InputProcess};
    inputProcess.start(&state);

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
        inputProcess.getStatus() != std::future_status::ready &&
        discoveryProcess.getStatus() != std::future_status::ready && 
        monitorProcess.getStatus() != std::future_status::ready);

    printLine("Program ended!");

    return 0;
}
