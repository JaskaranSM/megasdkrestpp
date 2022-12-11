//
// Created by zerocool on 12/11/22.
//
#include "loguru.cpp"
#include "log.h"

void InitializeLogger(int argc, char* argv[], const char* logFilePath)
{
    loguru::init(argc, argv);
    loguru::add_file(logFilePath, loguru::Append, loguru::Verbosity_MAX);
}