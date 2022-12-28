//
// Created by zerocool on 12/11/22.
//

#ifndef PROJECT_HTTPSERVER_H
#define PROJECT_HTTPSERVER_H

#include "served/multiplexer.hpp"
#include "served/net/server.hpp"
#include "MegaDownloader.h"
#include "json.hpp"
#include "log.h"

const char kAddDownloadEndpoint[] = "/adddownload";
const char kGetStatusEndpoint[] = "/getstatus";
const char kCancelDownloadEndpoint[] = "/canceldownload";
const char kLoginEndpoint[] = "/login";
const char kIPAddress[] = "0.0.0.0";

class HttpServer {
public:
    HttpServer(MegaDownloader* downloader) : downloader(downloader){}

    auto Login(MegaDownloader* downloader)
    {
        return [downloader](served::response & res, const served::request & req) {
            json::JSON reqBody = json::JSON::Load(req.body());
            std::string email = reqBody["email"].ToString();
            std::string password = reqBody["password"].ToString();
            VLOG_F(2, "Parsed email: %s | password: %s", email.c_str(), password.c_str());
            json::JSON obj = json::Object();
            std::ostringstream stream;
            if (email == "" || password == "")
            {
                obj["error_code"] = 401;
                obj["error_string"] = "credentials not properly provided";
                res.set_status(401);
                stream << obj;
                res << stream.str();
            } else {
                auto resp = downloader->Login(email.c_str(), password.c_str());
                obj["error_code"] = resp->errorCode;
                obj["error_string"] = resp->errorString;
                stream << obj;
                res << stream.str();
            }
        };
    }

    auto AddDownload(MegaDownloader* downloader)
    {
        return [downloader](served::response & res, const served::request & req) {
            json::JSON reqBody = json::JSON::Load(req.body());
            std::string link = reqBody["link"].ToString();
            std::string dir = reqBody["dir"].ToString();
            json::JSON obj = json::Object();
            std::ostringstream stream;
            VLOG_F(2, "Parsed link: %s | dir: %s", link.c_str(), dir.c_str());
            if (link == "" || dir == "")
            {
                obj["error_code"] = 401;
                obj["error_string"] = "link or dir not provided.";
                res.set_status(401);
                stream << obj;
                res << stream.str();
            } else {
                auto resp = downloader->AddDownload(link.c_str(), dir.c_str()); 
                obj["gid"] = resp->gid;
                obj["error_code"] = resp->errorCode;
                obj["error_string"] = resp->errorString;
                stream << obj;
                res << stream.str();
            }
            
        };
    }

    auto CancelDownload(MegaDownloader* downloader)
    {
        return [downloader](served::response & res, const served::request & req) {
            json::JSON reqBody = json::JSON::Load(req.body());
            std::string gid = reqBody["gid"].ToString();
            json::JSON obj = json::Object();
            std::ostringstream stream;
            VLOG_F(2, "Parsed gid: %s", gid.c_str());
            if (gid == "")
            { 
                obj["error_code"] = 401;
                obj["error_string"] = "gid not provided.";
                res.set_status(401);
                stream << obj;
                res << stream.str();
            } else {
                auto code = downloader->CancelDownloadByGid(gid);
                obj["gid"] = gid;
                obj["error_code"] = code;
                obj["error_string"] = "";
                stream << obj;
                res << stream.str();
            }
        };
    }

    auto GetStatus(MegaDownloader* downloader)
    {
        return [downloader](served::response & res, const served::request & req) {
            json::JSON reqBody = json::JSON::Load(req.body());
            std::string gid = reqBody["gid"].ToString();
            VLOG_F(2, "Parsed gid: %s", gid.c_str());
            json::JSON obj = json::Object();
            std::ostringstream stream;
            obj["gid"] = gid;
            auto dl = downloader->GetDownloadByGid(gid);
            if (dl == nullptr) {
                obj["error_code"] = 404;
                obj["error_string"] = "gid not found in downloader";
                res.set_status(404);
                stream << obj;
                res << stream.str();
                return;
            }
            auto info = dl->ToDownloadInfo();
            obj["error_code"] = info->errorCode;
            obj["error_string"] = info->errorString;
            obj["name"] = info->name;
            obj["speed"] = info->speed;
            obj["completed_length"] = info->completedLength;
            obj["total_length"] = info->totalLength;
            obj["state"] = info->state;
            obj["is_completed"] = info->isCompleted;
            obj["is_cancelled"] = info->isCancelled;
            obj["is_failed"] = info->isFailed;
            delete info;
            stream << obj;
            res << stream.str();
        };
    }

    void InitEndpoints()
    {
        this->multiplexer.handle(kAddDownloadEndpoint).post(this->AddDownload(this->downloader));
        this->multiplexer.handle(kLoginEndpoint).post(this->Login(this->downloader));
        this->multiplexer.handle(kGetStatusEndpoint).post(this->GetStatus(this->downloader));
        this->multiplexer.handle(kCancelDownloadEndpoint).post(this->CancelDownload(this->downloader));
    }

    void StartServer(const char* port, int threads)
    {
        served::net::server server(kIPAddress, port, this->multiplexer);
        LOG_F(INFO, "Starting web server");
        server.run(threads);
    }

private:
    served::multiplexer multiplexer;
    MegaDownloader* downloader;
};


#endif //PROJECT_HTTPSERVER_H
