#include "megaapi.h"
#include <condition_variable>
#include <mutex>


class MegaAppTransferListener : public mega::MegaTransferListener
{
private:
    std::condition_variable m_cv;
    std::mutex m_mu;
    bool m_notified;
    int64_t m_completedLength;
    int64_t m_totalLength;
    int64_t m_speed;
    int m_state;
    int m_errCode;
    std::string m_errString;
    bool m_isComplete;
    bool m_isCancelled;
    bool m_isFailed;
    bool m_isFolder;
    int m_transferTag;
    std::string m_gid;

    mega::MegaTransfer *m_currentTransfer;
    mega::MegaApi *m_api;

private:
    void onTransferStart(mega::MegaApi *api, mega::MegaTransfer *transfer);
    void onTransferFinish(mega::MegaApi *api, mega::MegaTransfer *transfer, mega::MegaError* e);
    void onTransferTemporaryError(mega::MegaApi *api, mega::MegaTransfer *transfer, mega::MegaError e);
    void onTransferUpdate(mega::MegaApi *api, mega::MegaTransfer *transfer);
    void onFolderTransferUpdate(mega::MegaApi *api, mega::MegaTransfer *transfer, int stage, uint32_t foldercount, uint32_t createdfoldercount, uint32_t filecount, const char *currentFolder, const char *currentFileLeafname);
    void lockAndNotify();

public:
    MegaAppTransferListener(std::string gid, bool isFolder, mega::MegaApi *api);
    void CancelTransfer();
    int64_t CompletedLength();
    int64_t TotalLength();
    int64_t Speed();
    void SetTotalLength(int64_t length);
    int GetState();
    int GetErrorCode();
    std::string GetErrorString();
    bool IsCompleted();
    bool IsCancelled();
    bool IsFailed();
    bool IsFolderTransfer();
    int GetTransferTag();
    void Reset();
    void Wait();
};
