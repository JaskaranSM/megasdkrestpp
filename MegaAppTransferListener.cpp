#include "megaapi.h"
#include "MegaAppTransferListener.h"

#include <iostream>
#include <log.h>

MegaAppTransferListener::MegaAppTransferListener(std::string gid, bool isFolder, mega::MegaApi *api)
{
    this->m_isFolder = isFolder;
    this->m_notified = false;
    this->m_gid = gid;
    this->m_api = api;
    this->m_state = mega::MegaTransfer::STATE_QUEUED;
    this->m_errCode = 0;
    this->m_isCancelled = false;
    this->m_isComplete = false;
    this->m_isFailed = false;
    this->m_completedLength = 0;
    this->m_speed = 0;
    this->m_totalLength = 0;
    this->m_transferTag = 0;
}

void MegaAppTransferListener::lockAndNotify()
{
    {
        std::unique_lock<std::mutex> lock(this->m_mu);
        m_notified = true;
    }
    m_cv.notify_all();
}

void MegaAppTransferListener::onTransferStart(mega::MegaApi *api, mega::MegaTransfer *transfer)
{
    this->m_transferTag = transfer->getTag();
    this->m_state = transfer->getState();
    this->m_currentTransfer = transfer->copy();
}

void MegaAppTransferListener::onTransferFinish(mega::MegaApi *api, mega::MegaTransfer *transfer, mega::MegaError* e)
{
    if (e->getErrorCode() != mega::MegaError::API_OK)
    {
        this->m_isFailed = true;
    }
    this->m_errCode = e->getErrorCode();
    this->m_errString = e->getErrorString();
    this->m_state = transfer->getState();
    this->lockAndNotify();

    if(transfer->isFolderTransfer() && transfer->isFinished() && e->getErrorCode() == mega::MegaError::API_OK)
    {
        this->m_isComplete = true;
        this->m_cv.notify_all();
    } else if(transfer->isFinished() && e->getErrorCode() == mega::MegaError::API_OK) {
        this->m_isComplete = true;
        this->m_cv.notify_all();
    }
}

void MegaAppTransferListener::onTransferTemporaryError(mega::MegaApi *api, mega::MegaTransfer *transfer, mega::MegaError e)
{
    VLOG_F(2, "onTransferTemporaryError: %s", e.toString());
}

void MegaAppTransferListener::onTransferUpdate(mega::MegaApi *api, mega::MegaTransfer *transfer)
{
    this->m_speed = transfer->getSpeed();
    this->m_completedLength = transfer->getTransferredBytes();
    this->m_state = transfer->getState();
    VLOG_F(3, "onTransferUpdate: %s ", transfer->getFileName());
}

void MegaAppTransferListener::onFolderTransferUpdate(mega::MegaApi *api, mega::MegaTransfer *transfer, int stage, uint32_t foldercount, uint32_t createdfoldercount, uint32_t filecount, const char *currentFolder, const char *currentFileLeafname)
{
    this->m_speed = transfer->getSpeed();
    this->m_completedLength = transfer->getTransferredBytes();
    this->m_state = transfer->getState();
    if (stage != 3) 
    {
        this->m_state = mega::MegaTransfer::STATE_QUEUED; 
    }
    VLOG_F(3, "onFolderTransferUpdate: %s ", transfer->getFileName());
}

void MegaAppTransferListener::CancelTransfer()
{
    this->m_isCancelled = true;
    this->m_api->cancelTransfer(this->m_currentTransfer);
}

int64_t MegaAppTransferListener::CompletedLength()
{
    return this->m_completedLength;
}

int64_t MegaAppTransferListener::TotalLength()
{
    return this->m_totalLength;
}

int64_t MegaAppTransferListener::Speed()
{
    return this->m_speed;
}

void MegaAppTransferListener::SetTotalLength(int64_t length)
{
    this->m_totalLength = length;
}

int MegaAppTransferListener::GetState()
{
    if (this->GetTransferTag() == 0) //transfer isnt started yet
    {
        return mega::MegaTransfer::STATE_QUEUED;
    }
    mega::MegaTransfer* transfer = this->m_api->getTransferByTag(this->GetTransferTag());
    if (transfer == NULL)
    {
        return mega::MegaTransfer::STATE_FAILED;
    }
    return transfer->getState();
}

int MegaAppTransferListener::GetErrorCode()
{
    return this->m_errCode;
}

std::string MegaAppTransferListener::GetErrorString()
{
    return this->m_errString;
}

bool MegaAppTransferListener::IsCompleted()
{
    return this->m_isComplete;
}

bool MegaAppTransferListener::IsCancelled()
{
    return this->m_isCancelled;
}

bool MegaAppTransferListener::IsFailed()
{
    return this->m_isFailed;
}

bool MegaAppTransferListener::IsFolderTransfer()
{
    return this->m_isFolder;
}

int MegaAppTransferListener::GetTransferTag()
{
    return this->m_transferTag;
}

void MegaAppTransferListener::Reset()
{
    this->m_notified = false;
}

void MegaAppTransferListener::Wait()
{
    std::unique_lock<std::mutex> lock(this->m_mu);
    this->m_cv.wait(lock, [this]{return this->m_notified;});
}