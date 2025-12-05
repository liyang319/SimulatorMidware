// 处理控制链路指令线程，用于接收上位机控制链路进来的数据，并放入控制链路接收队列中
#ifndef PROCESSCTRLCMDTHREAD_H
#define PROCESSCTRLCMDTHREAD_H

#include <thread>

class ProcessCtrlCmdThread
{
public:
    ProcessCtrlCmdThread();

    ~ProcessCtrlCmdThread();

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

#endif // PROCESSCTRLCMDTHREAD_H
