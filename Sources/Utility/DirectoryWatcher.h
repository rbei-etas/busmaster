#pragma once
#include<string>
#include<map>
#include<list>
#include"Utility_Thread.h"
#include"IDirectoryWatcher.h"

#define READ_DIR_CHANGE_BUFFER_SIZE 4096

struct DIR_INFO
{
    std::string strDirName;
    HANDLE hDirectory;
    CHAR* pBuffer;
    DIR_INFO();
};

class CDirectoryWatcher : public IDirectoryWatcher
{
private:
	bool m_bThreadIsAlive;
    static CDirectoryWatcher* m_psDirWatcher;
    static int m_nReferenceCount;
    HANDLE m_hIOCPort;
    UINT m_unActionCode;
    CRITICAL_SECTION m_CriticalSection;
    ULONG_PTR m_CKey;
    std::map<std::string,std::list<IDirectoryUpdate*>> m_mapDirAndlstDirHdlr;
    std::map<ULONG_PTR,DIR_INFO> m_mapCKeyAndDirInfo;
    CPARAM_THREADPROC m_ouMonitorDirectoryThread;

    static DWORD WINAPI MonitorDirectoryThreadProc(LPVOID pVoid);
    void Initialize();
    HRESULT WatchDirectory(std::string strDirName);
    HRESULT UnWatchDirectory(std::string strDirName);
    HRESULT NotifyDirectoryHandler(ULONG_PTR CKey);
    HRESULT ReIssueWatchCommand(ULONG_PTR CKey);

    CDirectoryWatcher();
    ~CDirectoryWatcher();
    CDirectoryWatcher(const CDirectoryWatcher&);
    CDirectoryWatcher& operator=(const CDirectoryWatcher&);
public:
    static CDirectoryWatcher* GetInstance();
    static void ReleaseInstance();
    HRESULT AddDirectoryWatch(std::string strDirName,IDirectoryUpdate* pIDirHdlr);
    HRESULT RemoveDirectoryWatch(std::string strDirName,IDirectoryUpdate* pIDirHdlr);
};