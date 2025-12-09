#include "ProcessCtrlCmdThread.h"
#include "AppData.h"
#include <string>
#include <iostream>
#include <unistd.h>
#include "CtrlCmdDispatcher.h"
#include "StatusCmdDispatcher.h"
#include "AppData.h"
#include "Base.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

using namespace std;

ProcessCtrlCmdThread::ProcessCtrlCmdThread() : m_running(false), client_socket(0)
{
}

ProcessCtrlCmdThread::~ProcessCtrlCmdThread()
{
    stop();
}

void ProcessCtrlCmdThread::start()
{
    if (!m_running)
    {
        m_running = true;
        m_thread = std::thread(&ProcessCtrlCmdThread::threadFunction, this);
    }
}

void ProcessCtrlCmdThread::stop()
{
    if (m_running)
    {
        m_running = false;
        COUT << "ProcessCtrlCmdThread  " << m_running << endl;
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }
}

void ProcessCtrlCmdThread::join()
{
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void ProcessCtrlCmdThread::threadFunction()
{
    printf("----ProcessCtrlCmdThread---\n");
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    getpeername(client_socket, (struct sockaddr *)&addr, &addr_len);
    char srcIP[INET_ADDRSTRLEN] = {0};
    inet_ntop(AF_INET, &addr.sin_addr, srcIP, INET_ADDRSTRLEN);
    while (m_running)
    {
        /////////////////////////////数据接收//////////////////////////////////
        char buffer[DEFAULT_DATA_ITEM_SIZE] = {0};
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received > 0)
        {
            // std::lock_guard<std::mutex> lock(ctrl_mutex);
            UpperportData recvData;
            recvData.len = bytes_received;
            recvData.content = new char[recvData.len + 1];
            memset(recvData.content, 0, recvData.len + 1);
            std::copy(buffer, buffer + recvData.len, recvData.content);
            // KLOG_D(">>>>>> " << recvData.content);
            AppData::getInstance().addDataToCtrlRecvQueue(recvData);
// printf("------handleCtrlConnection----size-%d-\n", AppData::getInstance().getCtrlRecvQueueSize());
#if DEBUG_LOG_UPPER_CMD
// KLOG_D("From [" << srcIP << "] " << buffer);
#endif
        }
        else if (bytes_received == 0)
        {
            std::cerr << "Client disconnected" << std::endl;
            close(client_socket);
            break;
        }
        else if (bytes_received == -1)
        {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
            {
                // No data available, continue
                // continue;
            }
            else
            {
                std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
                close(client_socket);
                break;
            }
        }
        /////////////////////////////数据发送//////////////////////////////////
        // std::lock_guard<std::mutex> lock(ctrl_mutex);
        // int xxx = AppData::getInstance().getCtrlSendQueueSize();
        // printf("-------check ctrl send queue-------%d----\n", xxx);
        if (AppData::getInstance().getCtrlSendQueueSize() > 0)
        {
            UpperportData send_data = AppData::getInstance().getDataFromCtrlSendQueue();
            // printf("-----ctrl_send_queue----datasize=%zu--\n", send_data.size());
            int bytes_sent = send(client_socket, send_data.content, send_data.len, 0);
            // KLOG_D( "<<<<<< "<< send_data.content);
            if (bytes_sent == -1)
            {
                std::cerr << "Error sending data: " << strerror(errno) << std::endl;
            }
            if (send_data.content != nullptr)
            {
                delete[] send_data.content;
                send_data.content = nullptr;
            }
            // printf("------ctrl_send_queue---send-%d--\n", bytes_sent);
        }
        usleep(10000);
    }
    printf("--------ProcessCtrlCmdThread----END--\n");
}
