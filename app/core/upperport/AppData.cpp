// appdata.cpp
#include "AppData.h"

AppData &AppData::getInstance()
{
    static AppData instance;
    return instance;
}

void AppData::addDataToDataRecvQueue(UpperportData &data)
{
    std::lock_guard<std::mutex> lock(data_recv_queue_mutex);
    data_recv_queue.push(data);
}

UpperportData AppData::getDataFromDataRecvQueue()
{
    std::lock_guard<std::mutex> lock(data_recv_queue_mutex);
    if (!data_recv_queue.empty())
    {
        UpperportData data = data_recv_queue.front();
        data_recv_queue.pop();
        return data;
    }
    return {};
}

void AppData::addDataToDataSendQueue(UpperportData &data)
{
    std::lock_guard<std::mutex> lock(data_send_queue_mutex);
    data_send_queue.push(data);
}

UpperportData AppData::getDataFromDataSendQueue()
{
    std::lock_guard<std::mutex> lock(data_send_queue_mutex);
    if (!data_send_queue.empty())
    {
        UpperportData data = data_send_queue.front();
        data_send_queue.pop();
        return data;
    }
    return {};
}

void AppData::addDataToCtrlRecvQueue(UpperportData &data)
{
    std::lock_guard<std::mutex> lock(ctrl_recv_queue_mutex);
    ctrl_recv_queue.push(data);
}

UpperportData AppData::getDataFromCtrlRecvQueue()
{
    std::lock_guard<std::mutex> lock(ctrl_recv_queue_mutex);
    if (!ctrl_recv_queue.empty())
    {
        UpperportData data = ctrl_recv_queue.front();
        ctrl_recv_queue.pop();
        return data;
    }
    return {};
}

void AppData::addDataToCtrlSendQueue(UpperportData &data)
{
    std::lock_guard<std::mutex> lock(ctrl_send_queue_mutex);
    ctrl_send_queue.push(data);
}

UpperportData AppData::getDataFromCtrlSendQueue()
{
    std::lock_guard<std::mutex> lock(ctrl_send_queue_mutex);
    if (!ctrl_send_queue.empty())
    {
        UpperportData data = ctrl_send_queue.front();
        ctrl_send_queue.pop();
        return data;
    }
    return {};
}

AppData::AppData()
{
    // constructor
}
