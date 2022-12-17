#include "MegaDownloader.h"
#include "MegaAppRequestListener.h"
#include "Utils.h"

#include <string.h>
#include "log.h"

MegaDownloader::MegaDownloader(const char* apiKey, mega::MegaApi *api)
{
    this->m_apiKey = apiKey;
    this->m_api = api;
}

bool MegaDownloader::IsLoggedIn()
{
    return this->m_api->isLoggedIn();
}

struct LoginResp* MegaDownloader::Login(const char* email, const char* password)
{
    struct LoginResp* resp = new LoginResp();
    resp->errorCode = 0;

    MegaAppRequestListener* reqListener = new MegaAppRequestListener(this->m_api);
    this->m_api->login(email,password, reqListener);
    reqListener->Wait();
    reqListener->Reset();

    if (reqListener->GetError() != NULL)
    {
        mega::MegaError* err = reqListener->GetError();
        resp->errorCode = err->getErrorCode();
        resp->errorString = err->getErrorString();
    }

    return resp;
}

MegaDownload* MegaDownloader::GetDownloadByGid(std::string gid)
{
    for (auto const& dl : this->m_downloads)
    {
        if (dl->Gid() == gid) {
            return dl;
        }
    }
    return nullptr;
}

int MegaDownloader::CancelDownloadByGid(std::string gid)
{
    MegaDownload* dl = GetDownloadByGid(gid);
    if(dl == nullptr)
    {
        return 404;
    }
    return dl->CancelDownload();
}

AddDownloadResp* MegaDownloader::AddDownload(const char* link, const char* dir)
{
    Utils utils;
    struct AddDownloadResp* resp = new AddDownloadResp();
    std::string gid = utils.GenerateRandomID(16);
    resp->gid = gid;
    LOG_F(INFO, "AddDownload: gid: %s", resp->gid.c_str());
    MegaAppRequestListener* reqListener = new MegaAppRequestListener(this->m_api);
    MegaAppTransferListener* transferListener = new MegaAppTransferListener(gid, this->m_api);
    LOG_F(INFO, "AddDownload: listeners created");
    if(utils.IsMegaFolder(link))
    {
        LOG_F(INFO, "AddDownload: starting loginToFolder");
        mega::MegaApi* folderApi = new mega::MegaApi(this->m_apiKey, (const char*)NULL, "Mega FolderApi temp Client");
        folderApi->loginToFolder(link, reqListener);
        reqListener->Wait();
        reqListener->Reset();
        LOG_F(INFO, "AddDownload: done loginToFolder");
        if(reqListener->GetError() != NULL && reqListener->GetError()->getErrorCode() != mega::MegaError::API_OK)
        {
            LOG_F(INFO, "AddDownload: loginToFolder errored");
            resp->errorCode = reqListener->GetError()->getErrorCode();
            resp->errorString = reqListener->GetError()->getErrorString();
            return resp;
        }
        LOG_F(INFO, "AddDownload: setting publicNode on reqListener");
        reqListener->SetPublicNode(folderApi->authorizeNode(reqListener->GetPublicNode()));
    } else {
        LOG_F(INFO, "AddDownload: getting publicNode");
        this->m_api->getPublicNode(link, reqListener);
        reqListener->Wait();
        reqListener->Reset();
    }

    if(reqListener->GetError() != NULL && reqListener->GetError()->getErrorCode() != mega::MegaError::API_OK)
    {
        LOG_F(INFO, "AddDownload: checkin if error on reqListener");
        resp->errorCode = reqListener->GetError()->getErrorCode();
        resp->errorString = reqListener->GetError()->getErrorString();
        return resp;
    }

    const char* fname = reqListener->GetPublicNode()->getName();
    std::string fpath = dir;
    fpath += "/";
    fpath += fname;
    transferListener->SetTotalLength(this->m_api->getSize(reqListener->GetPublicNode()));
    LOG_F(INFO, "Starting Download: %s : %s", fname, fpath.c_str());
    MegaDownload* dl = new MegaDownload(std::string(fname), gid, transferListener);
    this->registerDownload(dl);
    this->m_api->startDownload(reqListener->GetPublicNode(), fpath.c_str(), "", NULL, false, NULL, transferListener);
    return resp;
}

void MegaDownloader::registerDownload(MegaDownload *dl)
{
    this->m_downloads.push_back(dl);
}
