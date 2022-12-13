//
// Created by zerocool on 12/11/22.
//
#include <iostream>
#include "log.h"
#include "HttpServer.h"


int main(int argc, char* argv[])
{
    mega::MegaApi* api;
    MegaDownloader* downloader;
    api = new mega::MegaApi("Q2pyBIbB", (const char*)NULL, "MegaSDK", 1);
    downloader = new MegaDownloader("Q2pyBIbB", api);
//    api->setLogToConsole(true);
//    api->setLogLevel(mega::MegaApi::LOG_LEVEL_DEBUG);
    InitializeLogger(argc, argv, "everything.log");
    LOG_F(INFO, "creating server");
    HttpServer* httpServer = new HttpServer(downloader);
    httpServer->InitEndpoints();
    httpServer->StartServer();
    delete httpServer;
}
