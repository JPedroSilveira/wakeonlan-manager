#include "machine-info-utils.h"

const int MAX_HOSTNAME_LENGTH = 255;
const int MAX_MAC_ADDRESS_LENGTH = 255;

#if __APPLE__
const int MAC_ADDRESS_FAMILY = AF_LINK;
#elif __linux__
const int MAC_ADDRESS_FAMILY = AF_PACKET;
#else
const int MAC_ADDRESS_FAMILY = -1;
#endif

std::string getBroadcast(std::string ipv4)
{
    int pos = ipv4.find_last_of('.');
    return ipv4.substr(0, pos).append(".255");
}

std::vector<std::string> getAddresses()
{
    std::string ipv4{""};
    std::string ipv6{""};
    std::string mac("");
    struct ifaddrs *ptr_ifaddrs = nullptr;
    int result = getifaddrs(&ptr_ifaddrs);
    if (result)
    {
        throw std::runtime_error("Error getting machine IP addresses");
    }

    for (
        struct ifaddrs *ptr_entry = ptr_ifaddrs;
        ptr_entry != nullptr;
        ptr_entry = ptr_entry->ifa_next)
    {
        std::string interfaceName = std::string(ptr_entry->ifa_name);

        // TODO[LAB]: REMOVE TO RUN ON LABS
        // bool isWiredInterface = interfaceName.rfind("eth", 0) == 0;
        // if (!isWiredInterface) {
        //     continue;
        // }

        std::string ipaddress_human_readable_form;
        std::string netmask_human_readable_form;

        std::string interface_name = std::string(ptr_entry->ifa_name);
        sa_family_t address_family = ptr_entry->ifa_addr->sa_family;

        bool isIPV4 = address_family == AF_INET;
        bool isIPV6 = address_family == AF_INET6;
        bool isMAC = address_family == MAC_ADDRESS_FAMILY;

        if (isIPV4)
        {
            if (ptr_entry->ifa_addr != nullptr)
            {
                char buffer[INET_ADDRSTRLEN] = {
                    0,
                };
                inet_ntop(
                    address_family,
                    &((struct sockaddr_in *)(ptr_entry->ifa_addr))->sin_addr,
                    buffer,
                    INET_ADDRSTRLEN);

                ipv4 = std::string(buffer);
            }
        }
        else if (isIPV6)
        {
            if (ptr_entry->ifa_addr != nullptr)
            {
                char buffer[INET6_ADDRSTRLEN] = {
                    0,
                };
                inet_ntop(
                    address_family,
                    &((struct sockaddr_in6 *)(ptr_entry->ifa_addr))->sin6_addr,
                    buffer,
                    INET6_ADDRSTRLEN);

                ipv6 = std::string(buffer);
            }
        }
        else if (isMAC)
        {
            if (ptr_entry->ifa_addr != nullptr && std::string(ptr_entry->ifa_name).size() > 0)
            {
                char macAdress[MAX_MAC_ADDRESS_LENGTH];
                std::string command = "cat /sys/class/net/" + std::string(ptr_entry->ifa_name) + "/address";
                FILE *fp;
                fp = popen(command.c_str(), "r");
                fgets(macAdress, sizeof(macAdress), fp);
                pclose(fp);
		        mac = macAdress;
            }
        }
    }

    std::vector<std::string> ipAddresses{ipv4, ipv6, mac};
    return ipAddresses;
}

Member getMachineInfo()
{
    Member member{};

    member.isManager = false;

    char hostname[MAX_HOSTNAME_LENGTH];
    int result = gethostname(hostname, MAX_HOSTNAME_LENGTH);
    if (!result)
    {
        member.hostname = hostname;
    }

    std::vector<std::string> addresses = getAddresses();
    member.ipv4 = addresses.at(0);
    member.ipv6 = addresses.at(1);
    member.mac = addresses.at(2);

    member.broadcast = getBroadcast(member.ipv4);

    member.pid = getpid();

    return member;
}
