//
// Created by zerocool on 12/11/22.
//

#include "HttpServer.h"

void HttpServer::HttpServer(served::multiplexer mux)
{
    this->multiplexer = mux;
}
