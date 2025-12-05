// 单例类，用于全局访问，控制链路和状态链路的收发队列
#ifndef APPDATA_H
#define APPDATA_H

#include <queue>
#include <array>
#include <mutex>

#define DEFAULT_DATA_ITEM_SIZE 300

typedef struct _UpperportData
{
    char *content;
    int len;

    _UpperportData() : content(nullptr), len(0) {}
    // ~_UpperportData()
    // {
    //     if (content != nullptr)
    //     {
    //         delete[] content;
    //     }
    // }
} UpperportData;

class AppData
{

private:
    AppData();                                    // private constructor to prevent instantiation
    AppData(const AppData &) = delete;            // delete the copy constructor
    AppData &operator=(const AppData &) = delete; // delete the assignment operator

    std::queue<UpperportData> data_recv_queue;
    std::queue<UpperportData> data_send_queue;
    std::queue<UpperportData> ctrl_recv_queue;
    std::queue<UpperportData> ctrl_send_queue;

    std::mutex data_recv_queue_mutex;
    std::mutex data_send_queue_mutex;
    std::mutex ctrl_recv_queue_mutex;
    std::mutex ctrl_send_queue_mutex;

public:
    static AppData &getInstance();

    void addDataToDataRecvQueue(UpperportData &data);

    UpperportData getDataFromDataRecvQueue();

    void addDataToDataSendQueue(UpperportData &data);

    UpperportData getDataFromDataSendQueue();

    void addDataToCtrlRecvQueue(UpperportData &data);

    UpperportData getDataFromCtrlRecvQueue();

    void addDataToCtrlSendQueue(UpperportData &data);

    UpperportData getDataFromCtrlSendQueue();

    int getDataRecvQueueSize() { return data_recv_queue.size(); };

    int getDataSendQueueSize() { return data_send_queue.size(); };

    int getCtrlRecvQueueSize() { return ctrl_recv_queue.size(); };

    int getCtrlSendQueueSize() { return ctrl_send_queue.size(); };

    int powerSupplyStatus = 1; // 供电状态， 1：正常 0: 掉电
};

#endif // APPDATA_H
