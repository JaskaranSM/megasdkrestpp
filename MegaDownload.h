#include <string>
#include "MegaAppTransferListener.h"
#include "structs.h"


class MegaDownload
{
private:
    std::string m_name;
    std::string m_gid;
    MegaAppTransferListener *m_listener;
public:
    MegaDownload(std::string name, std::string gid, MegaAppTransferListener *listener);
    int CancelDownload();
    bool IsCompleted();
    std::string Gid();
    std::string Name();
    int64_t CompletedLength();
    int64_t TotalLength();
    int64_t Speed();
    int GetErrorCode();
    std::string GetErrorString();
    int GetState();
    DownloadInfo* ToDownloadInfo();
};