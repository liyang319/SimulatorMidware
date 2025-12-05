#include "UpperTcpServer.h"
#include <cstring>

UpperTcpServer::UpperTcpServer(int data_port, int ctrl_port)
{
    this->data_port = data_port;
    this->ctrl_port = ctrl_port;
    this->data_socket = createSocket(data_port);
    this->ctrl_socket = createSocket(ctrl_port);
    this->data_conn_socket = -1;
    this->ctrl_conn_socket = -1;
    this->is_running = false;
}

void UpperTcpServer::start()
{
    this->is_running = true;
    std::thread t(&UpperTcpServer::run, this);
    t.detach();
}

void UpperTcpServer::stop()
{
    this->is_running = false;
    if (this->data_socket >= 0)
    {
        close(this->data_socket);
    }
    if (this->ctrl_socket >= 0)
    {
        close(this->ctrl_socket);
    }
    if (this->data_conn_socket >= 0)
    {
        close(this->data_conn_socket);
    }
    if (this->ctrl_conn_socket >= 0)
    {
        close(this->ctrl_conn_socket);
    }

    // KLOG_D("socket closed");
}

int UpperTcpServer::createSocket(int port)
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        // KLOG_E("Error creating socket" << std::endl);
        return -1;
    }

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt(SO_REUSEADDR) failed");
        close(server_socket);
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        // KLOG_E("Error binding socket to port " << port << std::endl);
        return -1;
    }

    if (listen(server_socket, 5) < 0)
    {
        // KLOG_E("Error listening on port " << port << std::endl);
        return -1;
    }

    // fcntl(server_socket, F_SETFL, O_NONBLOCK);
    // 设置套接字为非阻塞模式
    int flags = fcntl(server_socket, F_GETFL, 0);
    if (flags == -1)
    {
        // KLOG_E("Error getting socket flags" << std::endl);
        return -1;
    }
    flags |= O_NONBLOCK;
    if (fcntl(server_socket, F_SETFL, flags) == -1)
    {
        // KLOG_E("Error setting socket to non-blocking mode" << std::endl);
        return -1;
    }
    return server_socket;
}

#if 0
void UpperTcpServer::handleDataConnection(int client_socket)
{
    // Implementation for handling data connection
    printf("----handleDataConnection---\n");
    while (true)
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

            // KLOG_D("From [" << data_conn_ip << "] " << buffer);
            // cout << "------add data send queue--------" << str << endl;
            // AppData::getInstance().addDataToDataSendQueue(recv_data);
        }
        else if (bytes_received == 0)
        {
            // KLOG_E("Client disconnected" << std::endl);
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
                // KLOG_E("Error receiving data: " << strerror(errno) << std::endl);
                close(client_socket);
                break;
            }
        }
        /////////////////////////////数据发送//////////////////////////////////
        // std::lock_guard<std::mutex> lock(data_mutex);
        if (AppData::getInstance().getDataSendQueueSize() > 0)
        {
            UpperportData send_data = AppData::getInstance().getDataFromDataSendQueue();
            // printf("-----data_send_queue----datasize=%zu--\n", send_data.size());
            int bytes_sent = send(client_socket, send_data.content, send_data.len, 0);
            if (bytes_sent == -1)
            {
                // KLOG_E("Error sending data: " << strerror(errno) << std::endl);
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
    printf("--------handleDataConnection----END---\n");
}

void UpperTcpServer::handleCtrlConnection(int client_socket)
{
    // Implementation for handling control connection
    printf("----handleCtrlConnection---\n");
    while (true)
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
            AppData::getInstance().addDataToCtrlRecvQueue(recvData);
            // printf("------handleCtrlConnection----size-%d-\n", AppData::getInstance().getCtrlRecvQueueSize());
            // KLOG_D("From [" << ctrl_conn_ip << "] " << buffer);
        }
        else if (bytes_received == 0)
        {
            // KLOG_E("Client disconnected" << std::endl);
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
                // KLOG_E("Error receiving data: " << strerror(errno) << std::endl);
                close(client_socket);
                break;
            }
        }
        /////////////////////////////数据发送//////////////////////////////////
        // std::lock_guard<std::mutex> lock(ctrl_mutex);
        int xxx = AppData::getInstance().getCtrlSendQueueSize();
        // printf("-------check ctrl send queue-------%d----\n", xxx);
        if (AppData::getInstance().getCtrlSendQueueSize() > 0)
        {
            UpperportData send_data = AppData::getInstance().getDataFromCtrlSendQueue();
            // printf("-----ctrl_send_queue----datasize=%zu--\n", send_data.size());
            int bytes_sent = send(client_socket, send_data.content, send_data.len, 0);
            if (bytes_sent == -1)
            {
                // KLOG_E("Error sending data: " << strerror(errno) << std::endl);
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
    printf("--------handleCtrlConnection----END---\n");
}
#endif

void UpperTcpServer::run()
{
    // Implementation for running the server
    while (this->is_running)
    {
        int maxfd = std::max(data_socket, ctrl_socket) + 1;
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(data_socket, &read_fds);
        FD_SET(ctrl_socket, &read_fds);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000;

        int ret = select(maxfd, &read_fds, NULL, NULL, &timeout);
        if (ret == -1)
        {
            // KLOG_E("Error in select()" << std::endl);
            break;
        }

        if (FD_ISSET(data_socket, &read_fds))
        {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            data_conn_socket = accept(data_socket, (struct sockaddr *)&client_addr, &client_len);
            if (data_conn_socket < 0)
            {
                ; // KLOG_E("Error accepting data connection" << std::endl);
            }
            else
            {
                if (processStatusCmdThread.isRunning())
                {
#ifdef BUILD_CONFIG_ARCH_ARM //
                    printf("------ProcessStatusCmdThread---STOP---\n");
#endif
                    processStatusCmdThread.stop();
                }
                // 设置客户端套接字为非阻塞模式
                int client_flags = fcntl(data_conn_socket, F_GETFL, 0);
                if (client_flags == -1)
                {
                    ; // KLOG_E("Error getting client socket flags" << std::endl);
                    // return -1;
                }
                client_flags |= O_NONBLOCK;
                if (fcntl(data_conn_socket, F_SETFL, client_flags) == -1)
                {
                    ; // KLOG_E("Error setting client socket to non-blocking mode" << std::endl);
                    // return -1;
                }
                /////////////获取数据链路连接IP//////////
                struct sockaddr_in addr;
                socklen_t addr_len = sizeof(addr);
                getpeername(data_conn_socket, (struct sockaddr *)&addr, &addr_len);
                char srcIP[INET_ADDRSTRLEN] = {0};
                inet_ntop(AF_INET, &addr.sin_addr, srcIP, INET_ADDRSTRLEN);
                data_conn_ip = srcIP;
                ///////////////////启动数据链路处理线程///////////////////
                // std::thread t(&UpperTcpServer::handleDataConnection, this, data_conn_socket);
                // t.detach();
                processStatusCmdThread.setClientSocket(data_conn_socket);
                processStatusCmdThread.start();
            }
        }

        if (FD_ISSET(ctrl_socket, &read_fds))
        {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            ctrl_conn_socket = accept(ctrl_socket, (struct sockaddr *)&client_addr, &client_len);
            if (ctrl_conn_socket < 0)
            {
                ; // KLOG_E("Error accepting ctrl connection" << std::endl);
            }
            else
            {
                if (processCtrlCmdThread.isRunning())
                {
                    printf("------ProcessCtrlCmdThread---STOP---\n");
                    processCtrlCmdThread.stop();
                }
                // 设置客户端套接字为非阻塞模式
                int client_flags = fcntl(ctrl_conn_socket, F_GETFL, 0);
                if (client_flags == -1)
                {
                    ; // KLOG_E("Error getting client socket flags" << std::endl);
                    // return -1;
                }
                client_flags |= O_NONBLOCK;
                if (fcntl(ctrl_conn_socket, F_SETFL, client_flags) == -1)
                {
                    ; // KLOG_E("Error setting client socket to non-blocking mode" << std::endl);
                    // return -1;
                }
                /////////////获取控制链路连接IP//////////
                struct sockaddr_in addr;
                socklen_t addr_len = sizeof(addr);
                getpeername(ctrl_conn_socket, (struct sockaddr *)&addr, &addr_len);
                char srcIP[INET_ADDRSTRLEN] = {0};
                inet_ntop(AF_INET, &addr.sin_addr, srcIP, INET_ADDRSTRLEN);
                ctrl_conn_ip = srcIP;
                ////////////////////启动控制链路处理线程//////////////////
                // std::thread t(&UpperTcpServer::handleCtrlConnection, this, ctrl_conn_socket);
                // t.detach();
                processCtrlCmdThread.setClientSocket(ctrl_conn_socket);
                processCtrlCmdThread.start();
            }
        }
    }
}

void UpperTcpServer::join()
{
    // Implementation for waiting for the server thread to finish
    while (this->is_running)
    {
        // Wait for the server thread to finish
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
