#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include "UpperTcpServer.h"
#include "UpperWorkerThread.h"

#include "Base.h"

int main(int argc, char *argv[])
{
    COUT << "Simulator Midware!" << endl;
    UpperTcpServer *_server;
    UpperWorkerThread *_workerThread;

    UpperTcpServer server(9000, 9001);
    UpperWorkerThread workerThread;
    server.start();
    workerThread.start();
    _server = &server;
    _workerThread = &workerThread;

    // 保持运行
    while (true)
    {
        sleep(10);
    }

    return 0;
}