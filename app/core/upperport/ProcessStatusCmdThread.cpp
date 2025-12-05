#include "ProcessStatusCmdThread.h"
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
#include <csignal>

using namespace std;

ProcessStatusCmdThread::ProcessStatusCmdThread() : m_running(false), client_socket(0)
{
}

ProcessStatusCmdThread::~ProcessStatusCmdThread()
{
    stop();
}

void ProcessStatusCmdThread::start()
{
    if (!m_running)
    {
        m_running = true;
        m_thread = std::thread(&ProcessStatusCmdThread::threadFunction, this);
    }
}

void ProcessStatusCmdThread::stop()
{
    if (m_running)
    {
        m_running = false;
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }
}

void ProcessStatusCmdThread::join()
{
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void ProcessStatusCmdThread::threadFunction()
{
    printf("----ProcessStatusCmdThread---\n");
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    getpeername(client_socket, (struct sockaddr *)&addr, &addr_len);
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGPIPE);
    sigprocmask(SIG_BLOCK, &set, NULL);
    char srcIP[INET_ADDRSTRLEN] = {0};
    inet_ntop(AF_INET, &addr.sin_addr, srcIP, INET_ADDRSTRLEN);
    while (m_running)
    {
        // sleep(1);
        /////////////////////////////数据接收//////////////////////////////////
        char buffer[DEFAULT_DATA_ITEM_SIZE] = {0};
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received > 0)
        {
            // std::lock_guard<std::mutex> lock(data_mutex);
            UpperportData recvData;
            recvData.len = bytes_received;
            recvData.content = new char[recvData.len + 1];
            memset(recvData.content, 0, recvData.len + 1);
            std::copy(buffer, buffer + recvData.len, recvData.content);
            AppData::getInstance().addDataToDataRecvQueue(recvData);
            // printf("------handleDataConnection------size--%d-\n", AppData::getInstance().getDataRecvQueueSize());
            // KLOG_D("From [" << srcIP << "] " << buffer);
        }
        else if (bytes_received == 0)
        {
#ifdef BUILD_CONFIG_ARCH_ARM //
// KLOG_E("Client disconnected" << std::endl);
#endif
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
                // KLOG_E( "Error receiving data: " << strerror(errno) << std::endl);
                close(client_socket);
                break;
            }
        }
        /////////////////////////////数据发送//////////////////////////////////
        // std::lock_guard<std::mutex> lock(data_mutex);
        if (AppData::getInstance().getDataSendQueueSize() > 0)
        {
            UpperportData send_data = AppData::getInstance().getDataFromDataSendQueue();
            // printf("-----data_send_queue----size=%zu--\n", AppData::getInstance().getDataSendQueueSize());
            try
            {
                int bytes_sent = send(client_socket, send_data.content, send_data.len, 0);
                if (bytes_sent == -1)
                {
                    // KLOG_E("Error sending data: " << strerror(errno) << std::endl);
                }
            }
            catch (const std::exception &e)
            {
                // KLOG_E( e.what() << '\n');
            }

            if (send_data.content != nullptr)
            {
                delete[] send_data.content;
                send_data.content = nullptr;
            }
            // printf("------data_send_queue---send-%d--\n", bytes_sent);
        }
        usleep(10000);
    }
#ifdef BUILD_CONFIG_ARCH_ARM //
    printf("--------ProcessStatusCmdThread----END--\n");
#endif
}
