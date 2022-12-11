#include <vector>
#include "MegaDownload.h"
#include "megaapi.h"
#include "structs.h"

class MegaDownloader
{
private:
    std::vector<MegaDownload*> m_downloads;
    mega::MegaApi *m_api;
    const char* m_apiKey;

private:
    void registerDownload(MegaDownload *dl);

public:
    MegaDownloader(const char* apiKey, mega::MegaApi *api);
    bool IsLoggedIn();
    struct LoginResp* Login(const char* email, const char* password);
    MegaDownload* GetDownloadByGid(std::string gid);
    int CancelDownloadByGid(std::string gid);
    AddDownloadResp* AddDownload(const char* link, const char* dir);
};