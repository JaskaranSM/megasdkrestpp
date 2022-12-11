//
// Created by zerocool on 12/11/22.
//

#ifndef PROJECT_HTTPSERVER_H
#define PROJECT_HTTPSERVER_H

#include "served/multiplexer.hpp"
#include "served/net/server.hpp"

const char kAddDownloadEndpoint[] = "/adddownload";
const char kGetStatusEndpoint[] = "/getstatus";
const char kIPAddress[] = "0.0.0.0";
const char kPort[] = "5000";


class HttpServer {
public:
    HttpServer(served::multiplexer) : multiplexer(multiplexer) {}

    void InitEndpoints();
private:
    served::multiplexer multiplexer;
};


#endif //PROJECT_HTTPSERVER_H
