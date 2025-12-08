#include "StatusCmdDispatcher.h"
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "Base.h"
#include "AppData.h"
#include "Utility.h"
#include "Base.h"
#include <regex>

int extractNumber(std::string data)
{
    std::regex pattern("Channel(\\d+)_Data");
    std::smatch match;
    if (std::regex_search(data, match, pattern))
    {
        return std::stoi(match[1]);
    }
    else
    {
        return -1; // 如果没有匹配到数字，返回-1或者其他你认为合适的值
    }
}

StatusCmdDispatcher::StatusCmdDispatcher(const std::string &json_data)
{
    COUT << "[Received StatusCmd] :  " << json_data << endl;
    m_document.Parse(json_data.c_str());
    if (m_document.HasParseError())
    {
        cout << "Failed to parse JSON data" << endl
             << json_data << std::endl;
        return;
    }
}

void StatusCmdDispatcher::DispatchCommand()
{
    if (m_document.HasParseError())
    {
        COUT << "Failed to parse JSON data" << endl;
        return;
    }

    if (m_document.HasMember("cmd"))
    {
        int channel = extractNumber(m_document["cmd"].GetString());
        ProcessChannelDataCmd(channel);
        // std::cout << "-------channel--------- " << channel << std::endl;
    }
    else
    {
        std::cerr << "Command not found in JSON" << endl;
    }
}

void StatusCmdDispatcher::ProcessChannelDataCmd(int channel)
{
    // COUT << "Processing ChannelData command " << endl;
    if (channel < 1 || channel > 16)
    {
        COUT << "Invalid channel" << endl;
        return;
    }
    // 回复通道状态数据
    UpperportData data;
    // DataFormater::FormatStatusData(channel, data);
    // 打印data的数据
    for (int i = 0; i < data.len; ++i)
    {
        // std::cout << data[i];
        // std::cout << std::hex << static_cast<int>(data[i]);
        // CLOG_D(channel,std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data.content[i]) << " ");
    }
    AppData::getInstance().addDataToDataSendQueue(data);
}
