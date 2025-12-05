#include "Utility.h"
#include <iostream>
#include <string>
#include <cstring> // for strncpy
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
// #include <netpacket/packet.h>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <netdb.h>
#include <ifaddrs.h>

std::string Utility::getSystemIP(const std::string &interfaceName)
{
    struct ifaddrs *interfaces = nullptr;
    struct ifaddrs *addr = nullptr;
    int result = getifaddrs(&interfaces);
    std::string ip;

    if (result == 0)
    {
        for (addr = interfaces; addr != nullptr; addr = addr->ifa_next)
        {
            if (addr->ifa_addr && addr->ifa_addr->sa_family == AF_INET)
            {
                if ((addr->ifa_flags & IFF_LOOPBACK) == 0 && (addr->ifa_flags & IFF_RUNNING) != 0)
                {
                    struct sockaddr_in *ipaddr = reinterpret_cast<struct sockaddr_in *>(addr->ifa_addr);
                    ip = std::string(inet_ntoa(ipaddr->sin_addr));
                    break;
                }
            }
        }

        freeifaddrs(interfaces);
    }
    else
    {
        std::cerr << "getifaddrs failed with error: " << result << std::endl;
    }

    return ip;
}

std::string Utility::getSystemMac(const std::string &interfaceName)
{
    // int fd = socket(AF_INET, SOCK_DGRAM, 0);
    // if (fd < 0)
    // {
    //     perror("socket failed");
    //     return "";
    // }

    // struct ifreq ifr;
    // memset(&ifr, 0, sizeof(ifr));
    // strncpy(ifr.ifr_name, interfaceName.c_str(), IFNAMSIZ);

    // struct sockaddr_ll addr;
    // if (ioctl(fd, SIOCGIFHWADDR, &ifr) == 0)
    // {
    //     close(fd);
    //     char mac[18];
    //     snprintf(mac, sizeof(mac), "%02x:%02x:%02x:%02x:%02x:%02x",
    //              (unsigned char)ifr.ifr_hwaddr.sa_data[0],
    //              (unsigned char)ifr.ifr_hwaddr.sa_data[1],
    //              (unsigned char)ifr.ifr_hwaddr.sa_data[2],
    //              (unsigned char)ifr.ifr_hwaddr.sa_data[3],
    //              (unsigned char)ifr.ifr_hwaddr.sa_data[4],
    //              (unsigned char)ifr.ifr_hwaddr.sa_data[5]);
    //     return std::string(mac);
    // }

    // close(fd);
    return "";
}

void Utility::setNetworkConfig(std::string iface, std::string mask, std::string gateway, std::string dns, std::string ip,
                               std::string mac)
{
    std::string command = "ifconfig " + iface + " " + ip + " netmask " + mask + " hw ether " + mac;

    // Set IP address, netmask and MAC address
    system(command.c_str());

    // Set default gateway
    command = "route add default gw " + gateway;
    system(command.c_str());

    // Set DNS server
    command = "echo \"nameserver " + dns + "\" > /etc/resolv.conf";
    system(command.c_str());
}

bool Utility::restartNetwork()
{
    // 执行重启网络的命令
    int result = system("sudo systemctl restart network");

    // 检查命令执行结果
    if (result == 0)
    {
        std::cout << "Network has been restarted successfully." << std::endl;
        return true;
    }
    else
    {
        std::cout << "Failed to restart network." << std::endl;
        return false;
    }
}

void Utility::sftpUploadWithCmd(std::string localFilePath, std::string remoteFilePath, std::string userName, std::string remoteHost)
{
    // std::string localFilePath = "/path/to/local/file.txt";
    // std::string remoteFilePath = "/path/on/remote/server/file.txt";
    // std::string username = "your_username";
    // std::string remoteHost = "remote_host";
    ///////////////////////////
    // 1.在本地计算机上生成SSH密钥对。您可以使用以下命令生成密钥对:
    //     bash ssh-keygen -t rsa
    // 2.将公钥上传到服务器。您可以使用以下命令将公钥上传到服务器：
    //     bash ssh-copy-id username@remote_host
    // 其中,username 是您在服务器上的用户名,remote_host 是服务器的主机名或IP地址。执行这个命令后,系统会要求输入服务器密码,
    // 并将您的公钥添加到服务器的~/.ssh/authorized_keys文件中。
    // 3.确保~/.ssh/authorized_keys文件的权限正确设置。您可以使用以下命令设置权限:
    //     chmod 600 ~/.ssh/authorized_keys
    // 这样,当您使用SSH密钥对连接到服务器时,
    // 服务器将会检查您的公钥是否存在于~/.ssh/authorized_keys文件中,从而完成身份验证。
    ////////////////////////////////
    // 构建SFTP命令，使用SSH密钥对进行身份验证
    // std::string sftpCommand = "sftp -oIdentityFile=/path/to/private/key " + userName + "@" + remoteHost + " <<EOF\n";
    std::string sftpCommand = "sftp " + userName + "@" + remoteHost + " <<EOF\n";
    sftpCommand += "put " + localFilePath + " " + remoteFilePath + "\n";
    sftpCommand += "bye\n";
    sftpCommand += "EOF";

    // 执行SFTP命令
    int returnValue = std::system(sftpCommand.c_str());
    if (returnValue == 0)
    {
        std::cout << "文件上传成功" << std::endl;
    }
    else
    {
        std::cout << "文件上传失败" << std::endl;
    }
}

unsigned short Utility::calculateCrc16(char *data, int length)
{
    unsigned short crc = 0xFFFF;

    for (int i = 0; i < length; i++)
    {
        crc ^= static_cast<unsigned char>(data[i]);
        for (int j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
            {
                crc = (crc >> 1) ^ 0xA001;
            }
            else
            {
                crc = crc >> 1;
            }
        }
    }

    return crc;
};