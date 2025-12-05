#ifndef UTILITY_H
#define UTILITY_H

#include <string>

class Utility
{
public:
    static std::string getSystemIP(const std::string &interfaceName = "eth0");

    static std::string getSystemMac(const std::string &interfaceName = "eth0");

    static void
    setNetworkConfig(std::string iface, std::string mask, std::string gateway, std::string dns, std::string ip,
                     std::string mac);

    static bool restartNetwork();

    static void sftpUploadWithCmd(std::string localFilePath, std::string remoteFilePath, std::string userName, std::string remoteHost);

    static unsigned short calculateCrc16(char *data, int length);
};

#endif // UTILITY_H
