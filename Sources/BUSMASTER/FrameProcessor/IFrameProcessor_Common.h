#pragma once
#include "DataTypes/MainSubEntry.h"
#include "include/BaseDefs.h"
#include "BaseLogObject.h"
#include <libxml\parser.h>
class IFrameProcessor_Common
{
public:
    virtual ~IFrameProcessor_Common() {};
    virtual HRESULT EnableLoggingBlock( USHORT ushBlk, bool bEnable ) =0 ;
    virtual HRESULT EnableLogging( bool bEnable )=0;
    virtual HRESULT EnableLogOnConnect(bool enable) = 0;
    virtual HRESULT EnableFilter( USHORT ushBlk, bool bEnable )=0;
    virtual bool IsClientBufferON( void )=0;
    virtual bool IsLoggingON( void )=0;
    virtual bool IsLogOnConnectEnabled() = 0;
    virtual bool IsDataLogged( void )=0;
    virtual void vCloseLogFile()=0;
    virtual bool IsThreadBlocked( void )=0;
    virtual void DisableDataLogFlag( void )=0;
    virtual bool IsFilterON( void )=0;
    virtual HRESULT LogString( CString& omStr )=0;
    virtual HRESULT AddLoggingBlock( const SLOGINFO& sLogObject )=0;
    virtual HRESULT RemoveLoggingBlock( USHORT ushBlk )=0;
    virtual USHORT GetLoggingBlockCount( void )=0;
    virtual HRESULT ClearLoggingBlockList( void )=0;
    virtual HRESULT GetLoggingBlock( USHORT ushBlk, SLOGINFO& sLogObject )=0;
    virtual HRESULT SetLoggingBlock( USHORT ushBlk, const SLOGINFO& sLogObject )=0;
    virtual HRESULT GetConfigData( BYTE** ppvConfigData, UINT& unLength )=0;
    virtual HRESULT SetConfigData( BYTE* pvDataStream, const CString& omStrVersion )=0;
    virtual HRESULT SetConfigData( xmlDocPtr pDoc)=0;
    virtual HRESULT GetConfigData( xmlNodePtr pxmlNodePtr )=0;
    virtual HRESULT Reset( void )=0;
    virtual HRESULT Confirm( void )=0;
    virtual HRESULT StartEditingSession( void )=0;
    virtual HRESULT StopEditingSession( bool bConfirm )=0;
    virtual HRESULT SetDatabaseFiles( const CStringArray& omList )=0;
    virtual void GetDatabaseFiles( CStringArray& omList )=0;
    virtual void SetChannelBaudRateDetails( void* controllerDetails, int nNumChannels)=0;
    //virtual void vPopulateMainSubList( USHORT ushBlk, CMainEntryList& DestList ) = 0;
    //virtual void vPopulateFilterApplied( USHORT ushBlk, CMainEntryList& DestList ) = 0;
};