// 用于解析上位机控制链路指令的控制指令类型，解析json指令，分发给不同的指令处理函数执行，并response给上位机
#ifndef CTRL_CMD_DISPATCHER_H
#define CTRL_CMD_DISPATCHER_H

#include "rapidjson/document.h"
#include <string>

#define CTRL_CMD_REPORT_IP "ReportIP"
#define CTRL_CMD_SET_IP "SetIP"
#define CTRL_CMD_NET_RESET "NetReset"
#define CTRL_CMD_SEND_PROJ_OK "SendProjOK"
#define CTRL_CMD_START "Start"
#define CTRL_CMD_STOP "Stop"
#define CTRL_CMD_PAUSE "Pause"
#define CTRL_CMD_RESTART "ReStart"
#define CTRL_CMD_JUMP "Jump"
#define CTRL_CMD_RECOVER_START "RecoverStart"
#define CTRL_CMD_EDIT_STEP "EditStep"
#define CTRL_CMD_ADD_STEP "AddStep"
#define CTRL_CMD_DEL_STEP "DelStep"
#define CTRL_CMD_REAL_PROJ_DATA "RealProjData"
#define CTRL_CMD_PROJ_DEFAULT_STOPINFO "ProjDefaultStopInfo"
#define CTRL_CMD_RESET "Reset"
#define CTRL_CMD_PARALLEL "Parallel"
#define CTRL_CMD_RELIEVE_PARALLEL "RelieveParallel"
#define CTRL_CMD_DEFAULT_LIST "DefaultList"
#define CTRL_CMD_GET_CONFIG "GetConfig"
#define CTRL_CMD_CONFIG_SUCC "ConfigSucc"
#define CTRL_CMD_SET_CONFIG "SetConfig"
#define CTRL_CMD_HEART "Heart"
#define CTRL_CMD_DEFAULT_INFO "DefaultInfo"
#define CTRL_CMD_REGIS_VALUE "RegisValue"
#define CTRL_CMD_CONFIG_OK "ConfigOK"
#define CTRL_CMD_CONFIG_AND_START "ConfigAndStart"
#define CTRL_CMD_VERSIONS "Versions"

class CtrlCmdDispatcher
{
public:
    CtrlCmdDispatcher(const std::string &json_data);

    void DispatchCommand();

private:
    void ProcessReportIPCmd();

    void ProcessSetIPCmd();

    void ProcessNetResetCmd();

    void ProcessSendProjOKCmd();

    void ProcessStartCmd();

    void ProcessStopCmd();

    void ProcessPauseCmd();

    void ProcessReStartCmd();

    void ProcessJumpCmd();

    void ProcessRecoverStartCmd();

    void ProcessEditStepCmd();

    void ProcessAddStepCmd();

    void ProcessDelStepCmd();

    void ProcessRealProjDataCmd();

    void ProcessProjDefaultStopInfoCmd();

    void ProcessResetCmd();

    void ProcessParallelCmd();

    void ProcessRelieveParallelCmd();

    void ProcessDefaultListCmd();

    void ProcessGetConfigCmd();

    void ProcessConfigSuccCmd();

    void ProcessSetConfigCmd();

    void ProcessHeartCmd();

    void ProcessDefaultInfoCmd();

    void ProcessRegisValueCmd();

    void ProcessConfigOKCmd();
    void ProcessConfigAndStartCmd();
    void ProcessVersionsCmd();

    rapidjson::Document m_document;
};

#endif // CTRL_CMD_DISPATCHER_H
