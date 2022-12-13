//
// Created by zerocool on 12/11/22.
//

#ifndef PROJECT_STRUCTS_H
#define PROJECT_STRUCTS_H
#include <stdbool.h>
#include <stdint.h>
#include <string>

struct DownloadInfo
{
    std::string name;
    int64_t completedLength;
    int64_t totalLength;
    int64_t speed;
    int state;
    std::string gid;
    int errorCode;
    std::string errorString;
    bool isFailed;
    bool isCancelled;
    bool isCompleted;
};

struct AddDownloadResp
{
    std::string gid;
    std::string errorString;
    int errorCode;
};

struct LoginResp
{
    int errorCode;
    std::string errorString;

};

#endif //PROJECT_STRUCTS_H
