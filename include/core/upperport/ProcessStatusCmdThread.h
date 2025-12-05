// 处理状态链路指令线程，用于接收上位机状态链路进来的数据，并放入状态链路接收队列中
#ifndef PROCESSSTATUSCMDTHREAD_H
#define PROCESSSTATUSCMDTHREAD_H

#include <thread>

class ProcessStatusCmdThread
{
public:
    ProcessStatusCmdThread();

    ~ProcessStatusCmdThread();

    void start();

    void stop();

    void join();

    void setClientSocket(int socketfd) { this->client_socket = socketfd; }

    bool isRunning() { return m_running; }

private:
    std::thread m_thread;
    bool m_running;
    int client_socket;
    void threadFunction();
};

#endif // PROCESSSTATUSCMDTHREAD_H
