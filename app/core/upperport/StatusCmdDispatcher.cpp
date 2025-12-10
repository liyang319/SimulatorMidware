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
        std::string cmd = m_document["cmd"].GetString();
        if (cmd == "QueryVars")
        {
            COUT << "Processing QueryVars command " << endl;
            ProcessQueryVarsCmd();
        }
        else
        {
            COUT << "Unknown command: " << cmd << endl;
        }
    }
    else
    {
        std::cerr << "Command not found in JSON" << endl;
    }
}

void StatusCmdDispatcher::ProcessQueryVarsCmd()
{
    COUT << "Processing QueryVars command " << endl;

    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Value cmd;
    cmd.SetString("QueryVars_ack");
    doc.AddMember("cmd", cmd, doc.GetAllocator());
    doc.AddMember("count", 3, doc.GetAllocator());

    // 构建 vars 对象
    rapidjson::Value vars(rapidjson::kObjectType);
    vars.AddMember("var1", "123.33", doc.GetAllocator());
    vars.AddMember("var2", "11.22", doc.GetAllocator());
    vars.AddMember("var3", "333.50", doc.GetAllocator());
    doc.AddMember("vars", vars, doc.GetAllocator());

    doc.AddMember("ack", "OK", doc.GetAllocator());

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    COUT << buffer.GetString() << endl;

    UpperportData data;
    data.len = strlen(buffer.GetString());
    data.content = new char[data.len + 1];
    memset(data.content, 0, data.len + 1);
    std::memcpy(data.content, buffer.GetString(), data.len);
    AppData::getInstance().addDataToDataSendQueue(data);
}
