#pragma once
#include "BaseSignalWatch.h"
#include "Utility/Utility_Thread.h"
#include "SigWatchDlg.h"
class CBaseSignalWatchImp : public CBaseSignalWatch
{


public:
    CBaseSignalWatchImp( ETYPE_BUS );
    virtual ~CBaseSignalWatchImp();

    virtual HRESULT SW_DoInitialization( IBMNetWorkGetService* info );
    virtual HRESULT SW_ShowAddDelSignalsDlg( CWnd* pParent, void* info );
    virtual HRESULT SW_ShowSigWatchWnd( CWnd* pParent, HWND hMainWnd, INT nCmd );
    virtual HRESULT SW_GetConfigData( xmlNodePtr pNodePtr );
    virtual HRESULT SW_SetConfigData( xmlNodePtr pNode );
    virtual HRESULT SW_ClearSigWatchWnd( void );
    virtual BOOL    SW_IsWindowVisible( void );
    virtual HRESULT SW_SetDisplayMode( BOOL bHex );
    virtual HRESULT SW_SetClusterInfo( void* ouCluster );

    virtual HRESULT DoInitialization() = 0;
    virtual HRESULT StartSigWatchReadThread() = 0;
protected:
    ETYPE_BUS mBusType;
    bool m_bHex;
    class CSigWatchDlg* m_pouSigWnd;
    CPARAM_THREADPROC m_ouReadThread;
    CRITICAL_SECTION m_omCritSecSW;
    IBMNetWorkGetService* mDbCluster;
    std::map<long, std::list<std::string>> m_mapMsgIDtoSignallst[16];
};

