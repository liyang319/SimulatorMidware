// 上位机指令工作线程，检查接收队列中是否有数据，如果有，从队列中取出数据，并调用控制链路和状态链路的dispatcher，进行指令处理
#ifndef UPPERWORKERTHREAD_H
#define UPPERWORKERTHREAD_H

#include <thread>
#include <vector>

class UpperWorkerThread
{
public:
    UpperWorkerThread();

    ~UpperWorkerThread();

    void start();

    void stop();

    void join(); // 添加join方法

private:
    std::thread m_thread;
    bool m_running;

    void threadFunction();
    std::vector<std::string> splitJsonCmd(std::string strCmd);
};

#endif // UPPERWORKERTHREAD_H
