#include "CtrlCmdDispatcher.h"
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "Base.h"
#include "AppData.h"
#include "Utility.h"
#include "Base.h"
#include <regex>

// using namespace rapidjson;
CtrlCmdDispatcher::CtrlCmdDispatcher(const std::string &json_data)
{
    // COUT << "[Received CtrlCmd] :  " << json_data << endl;
    m_document.Parse(json_data.c_str());
    if (m_document.HasParseError())
    {
        ; // KLOG_W("Failed to parse JSON data"<<std::endl<<json_data<<std::endl);
    }
}

void CtrlCmdDispatcher::DispatchCommand()
{
    if (m_document.HasParseError())
    {
        ; // KLOG_W("Failed to parse JSON data");
        return;
    }
    if (m_document.HasMember("cmd"))
    {
        std::string cmd = m_document["cmd"].GetString();
        // KLOG_D("====== " << cmd);
        if (cmd == CTRL_CMD_REPORT_IP)
        {
            ProcessReportIPCmd();
        }
        else if (cmd == CTRL_CMD_SET_IP)
        {
            ProcessSetIPCmd();
        }
        else if (cmd == CTRL_CMD_NET_RESET)
        {
            ProcessNetResetCmd();
        }
        else if (cmd == CTRL_CMD_SEND_PROJ_OK)
        {
            ProcessSendProjOKCmd();
        }
        else if (cmd == CTRL_CMD_START)
        {
            ProcessStartCmd();
        }
        else if (cmd == CTRL_CMD_STOP)
        {
            ProcessStopCmd();
        }
        else if (cmd == CTRL_CMD_PAUSE)
        {
            ProcessPauseCmd();
        }
        else if (cmd == CTRL_CMD_RESTART)
        {
            ProcessReStartCmd();
        }
        else if (cmd == CTRL_CMD_JUMP)
        {
            ProcessJumpCmd();
        }
        else if (cmd == CTRL_CMD_RECOVER_START)
        {
            ProcessRecoverStartCmd();
        }
        else if (cmd == CTRL_CMD_EDIT_STEP)
        {
            ProcessEditStepCmd();
        }
        else if (cmd == CTRL_CMD_ADD_STEP)
        {
            ProcessAddStepCmd();
        }
        else if (cmd == CTRL_CMD_DEL_STEP)
        {
            ProcessDelStepCmd();
        }
        else if (cmd == CTRL_CMD_REAL_PROJ_DATA)
        {
            ProcessRealProjDataCmd();
        }
        else if (cmd == CTRL_CMD_PROJ_DEFAULT_STOPINFO)
        {
            ProcessProjDefaultStopInfoCmd();
        }
        else if (cmd == CTRL_CMD_RESET)
        {
            ProcessResetCmd();
        }
        else if (cmd == CTRL_CMD_PARALLEL)
        {
            ProcessParallelCmd();
        }
        else if (cmd == CTRL_CMD_RELIEVE_PARALLEL)
        {
            ProcessRelieveParallelCmd();
        }
        else if (cmd == CTRL_CMD_DEFAULT_LIST)
        {
            ProcessDefaultListCmd();
        }
        else if (cmd == CTRL_CMD_GET_CONFIG)
        {
            ProcessGetConfigCmd();
        }
        else if (cmd == CTRL_CMD_CONFIG_SUCC)
        {
            ProcessConfigSuccCmd();
        }
        else if (cmd == CTRL_CMD_SET_CONFIG)
        {
            ProcessSetConfigCmd();
        }
        else if (cmd == CTRL_CMD_HEART)
        {
            ProcessHeartCmd();
        }
        else if (cmd == CTRL_CMD_DEFAULT_INFO)
        {
            ProcessDefaultInfoCmd();
        }
        else if (cmd == CTRL_CMD_REGIS_VALUE)
        {
            ProcessRegisValueCmd();
        }
        else if (cmd == CTRL_CMD_CONFIG_OK)
        {
            ProcessConfigOKCmd();
        }
        else if (cmd == CTRL_CMD_CONFIG_AND_START)
        {
            ProcessConfigAndStartCmd();
        }
        else if (cmd == CTRL_CMD_VERSIONS)
        {
            ProcessVersionsCmd();
        }
        else
        {
            std::cerr << "Unknown command: " << cmd << endl;
        }
    }
    else
    {
        std::cerr << "Command not found in JSON" << endl;
    }
}

void CtrlCmdDispatcher::ProcessReportIPCmd()
{
    COUT << "Processing ReportIP command" << endl;
}

void CtrlCmdDispatcher::ProcessSetIPCmd()
{
    COUT << "Processing SetIP command" << endl;
}

void CtrlCmdDispatcher::ProcessNetResetCmd()
{
    COUT << "Processing NetReset command" << endl;
}

void CtrlCmdDispatcher::ProcessSendProjOKCmd()
{
    COUT << "Processing SendProjOK command" << endl;
}

void CtrlCmdDispatcher::ProcessStartCmd()
{
    COUT << "Processing Start command" << endl;
}

void CtrlCmdDispatcher::ProcessStopCmd()
{
    COUT << "Processing Stop command" << endl;
}

void CtrlCmdDispatcher::ProcessPauseCmd()
{
    COUT << "Processing Pause command" << endl;
}

void CtrlCmdDispatcher::ProcessReStartCmd()
{
    COUT << "Processing ReStart command" << endl;
}

void CtrlCmdDispatcher::ProcessJumpCmd()
{
    COUT << "Processing Jump command" << endl;
}

void CtrlCmdDispatcher::ProcessRecoverStartCmd()
{
    COUT << "Processing Recover command" << endl;
}

void CtrlCmdDispatcher::ProcessEditStepCmd()
{
    COUT << "Processing EditStep command" << endl;
}

void CtrlCmdDispatcher::ProcessAddStepCmd()
{
    COUT << "Processing AddStep command" << endl;
}

void CtrlCmdDispatcher::ProcessDelStepCmd()
{
    COUT << "Processing DelStep command" << endl;
}

void CtrlCmdDispatcher::ProcessRealProjDataCmd()
{
    COUT << "Processing RealProjData command" << endl;
}

void CtrlCmdDispatcher::ProcessProjDefaultStopInfoCmd()
{
    COUT << "Processing ProjDefaultStopInfo command" << endl;
}

void CtrlCmdDispatcher::ProcessResetCmd()
{
    COUT << "Processing Reset command" << endl;
}

void CtrlCmdDispatcher::ProcessParallelCmd()
{
    COUT << "Processing Parallel command" << endl;
}

void CtrlCmdDispatcher::ProcessRelieveParallelCmd()
{
    COUT << "Processing RelieveParallel command" << endl;
}

void CtrlCmdDispatcher::ProcessDefaultListCmd()
{
    COUT << "Processing DefaultList command" << endl;
}

void CtrlCmdDispatcher::ProcessGetConfigCmd()
{
    COUT << "Processing GetConfig command" << endl;
}

void CtrlCmdDispatcher::ProcessConfigSuccCmd()
{
    COUT << "Processing ConfigSucc command" << endl;
}

void CtrlCmdDispatcher::ProcessSetConfigCmd()
{
    COUT << "Processing SetConfig command" << endl;
}

void CtrlCmdDispatcher::ProcessHeartCmd()
{
    COUT << "Processing Heart command" << endl;
}

void CtrlCmdDispatcher::ProcessDefaultInfoCmd()
{
    COUT << "Processing DefaultInfo command" << endl;
}

void CtrlCmdDispatcher::ProcessRegisValueCmd()
{
    COUT << "Processing RegisValue command" << endl;
}

void CtrlCmdDispatcher::ProcessConfigOKCmd()
{
    COUT << "Processing ConfigOK command" << endl;
}

void CtrlCmdDispatcher::ProcessVersionsCmd()
{
}
void CtrlCmdDispatcher::ProcessConfigAndStartCmd()
{
    COUT << "Processing ConfigAndStart command" << endl;
}