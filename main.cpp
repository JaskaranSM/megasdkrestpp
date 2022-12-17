//
// Created by zerocool on 12/11/22.
//
#include <iostream>
#include <algorithm>
#include <cctype>
#include "log.h"
#include "HttpServer.h"

struct Args {
    bool megaDebug;
    const char* appPort;
    const char* megaApiKey;
    int threads;
    int megaThreads;
};

int run(Args& args) 
{
    mega::MegaApi* api;
    MegaDownloader* downloader;
    api = new mega::MegaApi(args.megaApiKey, (const char*)NULL, "MegaSDKRestPP", args.megaThreads);
    downloader = new MegaDownloader(args.megaApiKey, api);
    if (args.megaDebug) 
    {
        api->setLogToConsole(true);
        api->setLogLevel(mega::MegaApi::LOG_LEVEL_DEBUG);
    }
    LOG_F(INFO, "creating server on 0.0.0.0:%s with %d threads", args.appPort, args.threads);
    HttpServer* httpServer = new HttpServer(downloader);
    httpServer->InitEndpoints();
    httpServer->StartServer(args.appPort, args.threads);
    LOG_F(INFO, "cleaning httpServer..");
    delete httpServer;
    return 0;
}

int main(int argc, char* argv[])
{
    Args args;
    const char* megaApiKey = std::getenv("MEGA_API_KEY");
    if (megaApiKey == NULL) 
    {
        std::cerr << "Environment variable MEGA_API_KEY not set, exitting!!!" << std::endl;
        return -1;
    } 

    const char* appPort = std::getenv("APP_PORT");
    if (appPort == NULL) 
    {
        appPort = "8069";
    }

    const char* logFile = std::getenv("LOG_FILE");
    if (logFile == NULL)
    {
        logFile = "log.txt";
    }
    const char* threadCount = std::getenv("APP_THREADS");
    if (threadCount == NULL) {
        args.threads = 1;
    } else {
        std::stringstream ss(threadCount);
        ss >> args.threads;     
    }

    const char* megaThreadCount = std::getenv("MEGA_THREADS");
    if (megaThreadCount == NULL) {
        args.megaThreads = 1;
    } else {
        std::stringstream ss(megaThreadCount);
        ss >> args.megaThreads;     
    }
    
    const char* megaDebugChar = std::getenv("MEGA_DEBUG");
    if (megaDebugChar == NULL) {
        args.megaDebug = false;
    } else {
        std::string megaDebug = megaDebugChar;
        std::transform(megaDebug.begin(), megaDebug.end(), megaDebug.begin(),
        [](unsigned char c){ 
            return std::tolower(c); 
        }
        );
        if (megaDebug == "true") {
            args.megaDebug = true;
        } else {
            args.megaDebug = false;
        }
    }
    
    args.appPort = appPort;
    args.megaApiKey = megaApiKey;
    InitializeLogger(argc, argv, logFile);
    return run(args);
}
