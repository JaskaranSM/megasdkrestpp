#include "MegaDownload.h"

MegaDownload::MegaDownload(std::string name, std::string gid, MegaAppTransferListener *listener)
{
    this->m_name = name;
    this->m_gid = gid;
    this->m_listener = listener;
}

int MegaDownload::CancelDownload()
{
    this->m_listener->CancelTransfer();
    return 0;
}

bool MegaDownload::IsCompleted()
{
    return this->m_listener->IsCompleted();
}

std::string MegaDownload::Gid()
{
    return this->m_gid;
}

std::string MegaDownload::Name()
{
    return this->m_name;
}

int64_t MegaDownload::CompletedLength()
{
    return this->m_listener->CompletedLength();
}

int64_t MegaDownload::TotalLength()
{
    return this->m_listener->TotalLength();
}

int64_t MegaDownload::Speed()
{
    return this->m_listener->Speed();
}

int MegaDownload::GetErrorCode()
{
    return this->m_listener->GetErrorCode();
}

std::string MegaDownload::GetErrorString()
{
    return this->m_listener->GetErrorString();
}

int MegaDownload::GetState()
{
    return this->m_listener->GetState();
}

DownloadInfo* MegaDownload::ToDownloadInfo()
{
    struct DownloadInfo* info = new DownloadInfo();
    info->gid = this->Gid();
    info->name = this->Name();
    info->errorCode = this->GetErrorCode();
    info->errorString = this->GetErrorString();
    info->completedLength = this->CompletedLength();
    info->totalLength = this->TotalLength();
    info->speed = this->Speed();
    info->state = this->GetState();
    info->isCancelled = this->m_listener->IsCancelled();
    info->isFailed = this->m_listener->IsFailed();
    info->isCompleted = this->m_listener->IsCompleted();
    return info;
}