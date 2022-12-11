//
// Created by zerocool on 12/11/22.
//
#include <iostream>
#include "MegaDownloader.h"
#include "log.h"
#include <thread>

int main(int argc, char* argv[])
{
    InitializeLogger(argc, argv, "everything.log");
    LOG_F(INFO, "Hello log file!");
//    mega::MegaApi* api = new mega::MegaApi(apiKey, NULL, "MegaSDK Client", 5);
//    MegaDownloader downloader(apiKey, api);
//    LoginResp* resp = downloader.Login(email, password);
//    LOG_F(INFO, "login resp %d", resp->errorCode);
//    AddDownloadResp* addResp =  downloader.AddDownload("https://mega.nz/folder/ymYC3boa#Ws_RQUXmVq96eDD8bBVKoQ", "dls");
//    if (addResp->errorCode != 0) {
//        LOG_F(INFO, "adddownload resp %d", addResp->errorCode);
//    } else {
//        MegaDownload* dl = downloader.GetDownloadByGid(addResp->gid);
//        if (dl == nullptr) {
//            LOG_F(INFO, "dl is null somehow");
//        } else {
//            while (!dl->IsCompleted()) {
//                LOG_F(INFO, "[%s] %ld/%ld @ %ld",dl->Name(), dl->CompletedLength(), dl->TotalLength(), dl->Speed());
//                std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 1));
//            }
//        }
//    }
}
