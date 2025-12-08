#include "UpperWorkerThread.h"
#include "AppData.h"
#include <string>
#include <iostream>
#include <unistd.h>
#include "CtrlCmdDispatcher.h"
#include "StatusCmdDispatcher.h"
#include "AppData.h"

using namespace std;

UpperWorkerThread::UpperWorkerThread() : m_running(false) {}

UpperWorkerThread::~UpperWorkerThread()
{
    stop();
}

void UpperWorkerThread::start()
{
    if (!m_running)
    {
        m_running = true;
        m_thread = std::thread(&UpperWorkerThread::threadFunction, this);
    }
}

void UpperWorkerThread::stop()
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

void UpperWorkerThread::join()
{
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

std::vector<std::string> UpperWorkerThread::splitJsonCmd(std::string strData)
{
    std::vector<std::string> jsonDataList;
    size_t pos = strData.find("\n");
    if (pos == std::string::npos)
    {
        // 如果找不到\n，则直接将整个strData字符串push到jsonDataList中
        jsonDataList.push_back(strData);
    }
    else
    {
        while (pos != std::string::npos)
        {
            std::string jsonData = strData.substr(0, pos);
            jsonDataList.push_back(jsonData);
            strData = strData.substr(pos + 1);
            pos = strData.find("\n");
        }
    }
    return jsonDataList;
}

void UpperWorkerThread::threadFunction()
{
    while (m_running)
    {
        // Do work here
        if (AppData::getInstance().getCtrlRecvQueueSize() > 0)
        {
            // cout << "----Worker---getCtrlRecvQueueSize------" << AppData::getInstance().getCtrlRecvQueueSize() << endl;
            UpperportData recv_data = AppData::getInstance().getDataFromCtrlRecvQueue();
            std::string str(recv_data.content);
            if (recv_data.content != nullptr)
            {
                // cout << "---delete---111--" << str << endl;
                delete[] recv_data.content;
                recv_data.content = nullptr;
            }
            cout << "--------" << str << endl;
            // std::vector<std::string> cmdList = splitJsonCmd(str);
            // // cout << "------cmdList------size=" << cmdList.size() << endl;
            // for (std::string &cmd : cmdList)
            // {
            //     CtrlCmdDispatcher patcher(cmd);
            //     patcher.DispatchCommand();
            // }
        }

        if (AppData::getInstance().getDataRecvQueueSize() > 0)
        {
            // cout << "----worker---getDataRecvQueueSize------" << AppData::getInstance().getDataRecvQueueSize() << endl;
            UpperportData recv_data = AppData::getInstance().getDataFromDataRecvQueue();
            std::string str(recv_data.content);
            if (recv_data.content != nullptr)
            {
                // cout << "---delete----111-" << str << endl;
                delete[] recv_data.content;
                recv_data.content = nullptr;
            }
            // cout << "--------" << str << endl;
            std::vector<std::string> cmdList = splitJsonCmd(str);
            // cout << "------statusCmdList------size=" << cmdList.size() << endl;
            for (std::string &cmd : cmdList)
            {
                StatusCmdDispatcher patcher(cmd);
                patcher.DispatchCommand();
            }
        }
        usleep(10000);
        // sleep(1);
    }
}
