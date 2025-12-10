// 用于解析上位机状态链路指令的控制指令类型，解析json指令，分发给不同的指令处理函数执行，并response给上位机
#ifndef STATUS_CMD_DISPATCHER_H
#define STATUS_CMD_DISPATCHER_H

#include "rapidjson/document.h"
#include <string>

#define STATUS_CMD_CHANNEL_1_DATA "Channel1_Data"
#define STATUS_CMD_CHANNEL_2_DATA "Channel2_Data"
#define STATUS_CMD_CHANNEL_3_DATA "Channel3_Data"
#define STATUS_CMD_CHANNEL_4_DATA "Channel4_Data"
#define STATUS_CMD_CHANNEL_5_DATA "Channel5_Data"
#define STATUS_CMD_CHANNEL_6_DATA "Channel6_Data"
#define STATUS_CMD_CHANNEL_7_DATA "Channel7_Data"
#define STATUS_CMD_CHANNEL_8_DATA "Channel8_Data"
#define STATUS_CMD_CHANNEL_9_DATA "Channel9_Data"
#define STATUS_CMD_CHANNEL_10_DATA "Channel10_Data"
#define STATUS_CMD_CHANNEL_11_DATA "Channel11_Data"
#define STATUS_CMD_CHANNEL_12_DATA "Channel12_Data"
#define STATUS_CMD_CHANNEL_13_DATA "Channel13_Data"
#define STATUS_CMD_CHANNEL_14_DATA "Channel14_Data"

class StatusCmdDispatcher
{
public:
    StatusCmdDispatcher(const std::string &json_data);

    void DispatchCommand();

private:
    void ProcessQueryVarsCmd();

    rapidjson::Document m_document;
};

#endif // STATUS_CMD_DISPATCHER_H
