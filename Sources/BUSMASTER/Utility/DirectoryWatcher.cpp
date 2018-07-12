#pragma once
#include"Utils_stdafx.h"
#include<iostream>
#include<algorithm>
#include"DirectoryWatcher.h"
#include "Shlwapi.h"

DIR_INFO::DIR_INFO()
{
    pBuffer=nullptr;
}

CDirectoryWatcher* CDirectoryWatcher::m_psDirWatcher = nullptr;
int CDirectoryWatcher::m_nReferenceCount = 0;
/**************************************************************************************
    Function Name    : GetInstance
    Input(s)         : void
    Output           : CDirectoryWatcher*
    Functionality    : Gets single instance of CDirectoryWatcher
    Member of        : CDirectoryWatcher
    Author(s)        : Robin G.K.
    Date Created     : 26.06.2015
***************************************************************************************/
CDirectoryWatcher* CDirectoryWatcher::GetInstance()
{
    if (m_psDirWatcher == nullptr)
    {
        m_psDirWatcher = new CDirectoryWatcher;
    }
    m_nReferenceCount++;
    return m_psDirWatcher;
}
/**************************************************************************************
    Function Name    :  ReleaseInstance
    Input(s)         :  void
    Output           :  void
    Functionality    :  Releases the instance of CDirectoryWatcher
    Member of        :  CDirectoryWatcher
    Author(s)        :  Robin G.K.
    Date Created     :  26.06.2015
***************************************************************************************/
void CDirectoryWatcher::ReleaseInstance()
{
    m_nReferenceCount--;
    if ((0 == m_nReferenceCount) && (nullptr != m_psDirWatcher))
    {
        delete m_psDirWatcher;
        m_psDirWatcher = nullptr;
    }
}
/**************************************************************************************
    Function Name    :  CDirectoryWatcher
    Input(s)         :  -
    Output           :  -
    Functionality    :  Constructor
    Member of        :  CDirectoryWatcher
    Author(s)        :  Robin G.K.
    Date Created     :  26.06.2015
***************************************************************************************/
CDirectoryWatcher::CDirectoryWatcher()
{
    InitializeCriticalSection(&m_CriticalSection);
    Initialize();
}

/**************************************************************************************
    Function Name    :  ~CDirectoryWatcher
    Input(s)         :  -
    Output           :  -
    Functionality    :  Destructor
    Member of        :  CDirectoryWatcher
    Author(s)        :  Robin G.K.
    Date Created     :  26.06.2015
***************************************************************************************/
CDirectoryWatcher::~CDirectoryWatcher()
{
    for(std::map<std::string,std::list<IDirectoryUpdate*>>::iterator itr = m_mapDirAndlstDirHdlr.begin();
            itr!=m_mapDirAndlstDirHdlr.end(); itr++)
    {
        UnWatchDirectory(itr->first);
    }

    m_ouMonitorDirectoryThread.bTerminateThread();
    CloseHandle( m_hIOCPort );
    m_hIOCPort = nullptr;
    DeleteCriticalSection(&m_CriticalSection);
}

/**************************************************************************************
    Function Name    :  Initialize
    Input(s)         :  void
    Output           :  void
    Functionality    :  Initialize variables
    Member of        :  CDirectoryWatcher
    Author(s)        :  Robin G.K.
    Date Created     :  26.06.2015
***************************************************************************************/
void CDirectoryWatcher::Initialize()
{
    m_CKey=0;
    m_hIOCPort = nullptr;
    m_unActionCode = INACTION;
    m_ouMonitorDirectoryThread.m_unActionCode = INACTION;
    m_ouMonitorDirectoryThread.m_pBuffer = this;
    m_ouMonitorDirectoryThread.m_hActionEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

/**************************************************************************************
    Function Name    :  MonitorDirectoryThreadProc
    Input(s)         :  LPVOID pVoid
    Output           :  DWORD
    Functionality    :  Directory Monitor for Changes Thread
    Member of        :  CDirectoryWatcher
    Author(s)        :  Robin G.K.
    Date Created     :  26.06.2015
***************************************************************************************/
DWORD WINAPI CDirectoryWatcher::MonitorDirectoryThreadProc(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    if (pThreadParam == nullptr)
    {
        return ((DWORD)-1);
    }
    CDirectoryWatcher* pDirectoryWatcher = (CDirectoryWatcher*)(pThreadParam->m_pBuffer);
    if(pDirectoryWatcher == nullptr)
    {
        return ((DWORD)-1);
    }

    bool bLoopON = true;
    ULONG_PTR CKey=0;
    DWORD hResult = S_FALSE;
    while (bLoopON)
    {
        switch (pThreadParam->m_unActionCode)
        {
            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;
            case SUSPEND:
            {
                WaitForSingleObject(pThreadParam->m_hActionEvent,INFINITE);
                ResetEvent(pThreadParam->m_hActionEvent);
            }
            break;
            case INACTION:
            {
                //dwWait = 0;
                DWORD numBytes;
                LPOVERLAPPED lpOverlapped;
                CKey = 0;
                hResult = GetQueuedCompletionStatus( pDirectoryWatcher->m_hIOCPort,
                                                     &numBytes,
                                                     &CKey,//<-- completion Key
                                                     &lpOverlapped,
                                                     INFINITE);
                if(hResult ==  0) //Failure
                {
                    hResult = GetLastError();
                }
                else if(hResult!=0) //Successful
                {
                    OVERLAPPED Overlapped;
                    memset(&Overlapped, 0, sizeof(Overlapped));
                    //1. If port is exited by calling PostQueuedCompletionStatus
                    //TODO: Check if invoked by PostQueuedCompletionStatus
                    if(numBytes == 0)
                    {
                        if(pDirectoryWatcher->m_mapCKeyAndDirInfo.find(CKey) != pDirectoryWatcher->m_mapCKeyAndDirInfo.end())
                        {
                            if(pDirectoryWatcher->m_mapCKeyAndDirInfo.at(CKey).pBuffer != nullptr)
                            {
                                delete[] pDirectoryWatcher->m_mapCKeyAndDirInfo.at(CKey).pBuffer;
                                pDirectoryWatcher->m_mapCKeyAndDirInfo.at(CKey).pBuffer = nullptr;
                            }
                            CloseHandle(pDirectoryWatcher->m_mapCKeyAndDirInfo.at(CKey).hDirectory);
                            pDirectoryWatcher->m_mapCKeyAndDirInfo.erase(CKey);
                        }
                        if(pDirectoryWatcher->m_mapCKeyAndDirInfo.size()==0)
                        {
                            pThreadParam->m_unActionCode = pDirectoryWatcher->m_unActionCode;
                        }
                    }
                    //2. If Change in directory detected
                    else
                    {
                        if(pDirectoryWatcher->m_mapCKeyAndDirInfo.find(CKey) != pDirectoryWatcher->m_mapCKeyAndDirInfo.end())
                        {
                            pDirectoryWatcher->NotifyDirectoryHandler(CKey);
                            pDirectoryWatcher->ReIssueWatchCommand(CKey);
                        }
                    }
                }
            }
            break;
        }
    }
    SetEvent(pThreadParam->hGetExitNotifyEvent());
    return 0;
}

/**************************************************************************************
    Function Name    :  WatchDirectory
    Input(s)         :  std::string strDirName
    Output           :  HRESULT
    Functionality    :  Add directory to watch in MonitorDirectoryThreadProc
    Member of        :  CDirectoryWatcher
    Author(s)        :  Robin G.K.
    Date Created     :  26.06.2015
***************************************************************************************/
HRESULT CDirectoryWatcher::WatchDirectory(std::string strDirName)
{
    DWORD hResult = S_OK;
    DIR_INFO DirInfo;
    //Overlapped is just used to specify it is in Async mode - It has no significance
    OVERLAPPED  Overlapped;
    memset(&Overlapped, 0, sizeof(Overlapped));
    DWORD BytesReturned=0;

    DirInfo.strDirName = strDirName;

    DirInfo.hDirectory = CreateFile(DirInfo.strDirName.c_str(),
                                    FILE_LIST_DIRECTORY,
                                    FILE_SHARE_READ | FILE_SHARE_WRITE ,//| FILE_SHARE_DELETE, <-- removing FILE_SHARE_DELETE prevents the user or someone else from renaming or deleting the watched directory. This is a good thing to prevent.
                                    NULL,
                                    OPEN_EXISTING,
                                    FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OVERLAPPED,
                                    NULL);

    if( DirInfo.hDirectory == INVALID_HANDLE_VALUE )
    {
        hResult = GetLastError();
    }
    if(hResult == S_OK)
    {
        m_CKey++;
        m_hIOCPort = CreateIoCompletionPort(DirInfo.hDirectory,
                                            m_hIOCPort, //if m_hCompPort is NULL, hDir is associated with a NEW completion port,
                                            //if m_hCompPort is NON-NULL, hDir is associated with the existing completion port that the handle m_hCompPort references
                                            m_CKey, //the completion 'key'... this ptr is returned from GetQueuedCompletionStatus() when one of the events in the dwChangesToWatchFor filter takes place
                                            0);

        if(m_hIOCPort != nullptr)
        {
            CHAR*       Buffer = new CHAR[ READ_DIR_CHANGE_BUFFER_SIZE ];
            DirInfo.pBuffer = Buffer;
            memset(Buffer, '\0', READ_DIR_CHANGE_BUFFER_SIZE*sizeof(CHAR));
            hResult = ReadDirectoryChangesW( DirInfo.hDirectory,
                                             Buffer,//<--FILE_NOTIFY_INFORMATION records are put into this buffer
                                             READ_DIR_CHANGE_BUFFER_SIZE,
                                             TRUE,
                                             FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_DIR_NAME|
                                             FILE_NOTIFY_CHANGE_ATTRIBUTES|FILE_NOTIFY_CHANGE_SIZE|
                                             FILE_NOTIFY_CHANGE_LAST_WRITE|FILE_NOTIFY_CHANGE_LAST_ACCESS|
                                             FILE_NOTIFY_CHANGE_CREATION|FILE_NOTIFY_CHANGE_SECURITY,
                                             &BytesReturned,//this var not set when using asynchronous mechanisms...
                                             &Overlapped,
                                             NULL);
            if(hResult == 0)
            {
                hResult = GetLastError();
            }
            else if(hResult!=0)
            {
                m_mapCKeyAndDirInfo.insert(std::pair<ULONG_PTR,DIR_INFO>(m_CKey,DirInfo));
                m_ouMonitorDirectoryThread.m_unActionCode = INACTION;
                SetEvent(m_ouMonitorDirectoryThread.m_hActionEvent);
                if(TRUE == m_ouMonitorDirectoryThread.bStartThread(MonitorDirectoryThreadProc))
                {
                    hResult = S_OK;
                }
            }
        }
    }
    return hResult;
}

/**************************************************************************************
    Function Name    :  UnWatchDirectory
    Input(s)         :  std::string strDirName
    Output           :  HRESULT
    Functionality    :  Remove directory from monitoring in MonitorDirectoryThreadProc
    Member of        :  CDirectoryWatcher
    Author(s)        :  Robin G.K.
    Date Created     :  26.06.2015
***************************************************************************************/
HRESULT CDirectoryWatcher::UnWatchDirectory(std::string strDirName)
{
    DIR_INFO DirInfo;
    ULONG_PTR CKey = 0;
    DWORD hResult = S_FALSE;
    bool bDirExists = false;
    for (std::map<ULONG_PTR,DIR_INFO>::iterator it = m_mapCKeyAndDirInfo.begin(); it != m_mapCKeyAndDirInfo.end(); ++it )
    {
        if (it->second.strDirName == strDirName)
        {
            DirInfo = it->second;
            CKey = it->first;
            bDirExists = true;
            break;
        }
    }
    if(true == bDirExists)
    {
        //Overlapped is just used to specify it is in Async mode - It has no significance
        OVERLAPPED  Overlapped;
        memset(&Overlapped, 0, sizeof(Overlapped));
        DWORD BytesReturned = 0;

        if(m_mapCKeyAndDirInfo.size()==1)
        {
            if(m_unActionCode!=EXIT_THREAD)
            {
                m_unActionCode = SUSPEND;
            }
            ResetEvent(m_ouMonitorDirectoryThread.m_hActionEvent);
        }
        hResult = PostQueuedCompletionStatus(m_hIOCPort,BytesReturned,CKey,&Overlapped);
        if(hResult == 0) //Fail
        {
            hResult = ::GetLastError();
        }
    }

    return hResult;
}

/**************************************************************************************
    Function Name    :  NotifyDirectoryHandler
    Input(s)         :  ULONG_PTR CKey
    Output           :  HRESULT
    Functionality    :  Notify IDirectoryUpdate* clients of files changed in directory.
    Member of        :  CDirectoryWatcher
    Author(s)        :  Robin G.K.
    Date Created     :  26.06.2015
***************************************************************************************/
HRESULT CDirectoryWatcher::NotifyDirectoryHandler(ULONG_PTR CKey)
{
    HRESULT hResult = S_OK;

    if(m_mapCKeyAndDirInfo.find(CKey) == m_mapCKeyAndDirInfo.end())
    {
        return S_FALSE;
    }

    std::string strDirName = m_mapCKeyAndDirInfo.at(CKey).strDirName;

    CHAR* pBuffer = m_mapCKeyAndDirInfo.at(CKey).pBuffer;
    if(m_mapDirAndlstDirHdlr.find(strDirName) == m_mapDirAndlstDirHdlr.end())
    {
        return S_FALSE;
    }
    std::list<IDirectoryUpdate*>& lstDirHdlr =  m_mapDirAndlstDirHdlr.at(strDirName);
    for (FILE_NOTIFY_INFORMATION* fni=reinterpret_cast<FILE_NOTIFY_INFORMATION*>(pBuffer);;
            fni = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(
                      reinterpret_cast<char*>(fni) + fni->NextEntryOffset))
    {
        std::wstring filename(fni->FileName, fni->FileName + fni->FileNameLength/sizeof (WCHAR));
        char* lpAbsPath = new char[MAX_PATH];
        memset(lpAbsPath,'\0',MAX_PATH*sizeof(char));
        std::string strFileName(filename.begin(),filename.end());
        CString omStrFileName = strFileName.c_str();
        PathCombine(lpAbsPath,strDirName.c_str(),omStrFileName);
        if(TRUE == PathFileExists(lpAbsPath))
        {
            //Notify all Registered Directory Handlers for the directory strDirName.
            for(std::list<IDirectoryUpdate*>::iterator itr = lstDirHdlr.begin(); itr!=lstDirHdlr.end(); itr++)
            {
                if(*itr != nullptr && fni!=nullptr)
                {
                    (*itr)->OnFileChanged(std::string(lpAbsPath),fni->Action);
                }
            }
        }
        if (nullptr != fni && fni->NextEntryOffset == 0)
        {
            break;
        }
    }

    return S_OK;
}

/**************************************************************************************
    Function Name    :
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CDirectoryWatcher
    Author(s)        :  Robin G.K.
    Date Created     :  26.06.2015
***************************************************************************************/
HRESULT CDirectoryWatcher::ReIssueWatchCommand(ULONG_PTR CKey)
{
    //Overlapped is just used to specify it is in Async mode - It has no significance here
    OVERLAPPED  Overlapped;
    memset(&Overlapped, 0, sizeof(Overlapped));
    DWORD BytesReturned=0;
    DWORD hResult = S_OK;
    if(m_mapCKeyAndDirInfo.find(CKey) == m_mapCKeyAndDirInfo.end())
    {
        return S_FALSE;
    }
    hResult = ReadDirectoryChangesW( m_mapCKeyAndDirInfo.at(CKey).hDirectory,
                                     m_mapCKeyAndDirInfo.at(CKey).pBuffer,//<--FILE_NOTIFY_INFORMATION records are put into this buffer
                                     READ_DIR_CHANGE_BUFFER_SIZE,
                                     TRUE,
                                     FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_DIR_NAME|
                                     FILE_NOTIFY_CHANGE_ATTRIBUTES|FILE_NOTIFY_CHANGE_SIZE|
                                     FILE_NOTIFY_CHANGE_LAST_WRITE|FILE_NOTIFY_CHANGE_LAST_ACCESS|
                                     FILE_NOTIFY_CHANGE_CREATION|FILE_NOTIFY_CHANGE_SECURITY,
                                     &BytesReturned,//this var not set when using asynchronous mechanisms...
                                     &Overlapped,
                                     NULL);

    if(hResult == 0)
    {
        hResult = GetLastError();
    }
    return hResult;
}

/**************************************************************************************
    Function Name    :  AddDirectoryWatch
    Input(s)         :  std::string strDirName,IDirectoryUpdate* pIDirHdlr
    Output           :  HRESULT
    Functionality    :  Add Directory to monitor for changes and receive notification to
                        (IDirectoryUpdate*) clients.
    Member of        :  CDirectoryWatcher
    Author(s)        :  Robin G.K.
    Date Created     :  26.06.2015
***************************************************************************************/
HRESULT CDirectoryWatcher::AddDirectoryWatch(std::string strDirName,IDirectoryUpdate* pIDirHdlr)
{
    EnterCriticalSection(&m_CriticalSection);
    DWORD hResult = S_OK;
    //Check File directory is valid.
    if(PathIsDirectory(strDirName.c_str()) == FALSE || pIDirHdlr == nullptr)
    {
        hResult = S_FALSE;
    }
    else
    {
        //1. If Directory is found in the map
        if(m_mapDirAndlstDirHdlr.find(strDirName) != m_mapDirAndlstDirHdlr.end())
        {
            std::list<IDirectoryUpdate*>& lstDirHdlr = m_mapDirAndlstDirHdlr.find(strDirName)->second;
            //1a. If pIDirHdlr is not found in the map, add it.
            if(std::find(lstDirHdlr.begin(),lstDirHdlr.end(),pIDirHdlr) == lstDirHdlr.end())
            {
                m_mapDirAndlstDirHdlr.find(strDirName)->second.push_back(pIDirHdlr);
            }
        }
        //2. Add the Directory and Handler to map and start Watch directory.
        else
        {
            std::list<IDirectoryUpdate*> lstTemp;
            lstTemp.push_back(pIDirHdlr);
            m_mapDirAndlstDirHdlr.insert(std::pair<std::string,std::list<IDirectoryUpdate*>>(strDirName,lstTemp));
            hResult = WatchDirectory(strDirName);
        }
    }
    LeaveCriticalSection(&m_CriticalSection);
    return hResult;
}

/**************************************************************************************
    Function Name    :  RemoveDirectoryWatch
    Input(s)         :  std::string strDirName,IDirectoryUpdate* pIDirHdlr
    Output           :  HRESULT
    Functionality    :  Remove directory and associated IDirectoryUpdate* client from
                        being monitored.
    Member of        :  CDirectoryWatcher
    Author(s)        :  Robin G.K.
    Date Created     :  26.06.2015
***************************************************************************************/
HRESULT CDirectoryWatcher::RemoveDirectoryWatch(std::string strDirName,IDirectoryUpdate* pIDirHdlr)
{
    EnterCriticalSection(&m_CriticalSection);
    DWORD hResult = S_OK;
    //Check File directory is valid.
    if(PathIsDirectory(strDirName.c_str()) == FALSE || pIDirHdlr == nullptr || m_mapDirAndlstDirHdlr.find(strDirName)==m_mapDirAndlstDirHdlr.end())
    {
        hResult = S_FALSE;
    }
    else
    {
        std::list<IDirectoryUpdate*>& lstDirHdlr = m_mapDirAndlstDirHdlr.find(strDirName)->second;
        if(std::find(lstDirHdlr.begin(),lstDirHdlr.end(),pIDirHdlr)
                != lstDirHdlr.end())
        {
            if(lstDirHdlr.size() > 1)
            {
                lstDirHdlr.remove(pIDirHdlr);
            }
            else
            {
                m_mapDirAndlstDirHdlr.erase(strDirName);
                hResult = UnWatchDirectory(strDirName);
            }
        }
    }
    LeaveCriticalSection(&m_CriticalSection);
    return hResult;
}


