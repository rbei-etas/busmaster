

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Tue Aug 16 23:08:19 2011
 */
/* Compiler settings for .\BUSMASTER.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __CAN_Monitor_Interface_h__
#define __CAN_Monitor_Interface_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IApplication_FWD_DEFINED__
#define __IApplication_FWD_DEFINED__
typedef interface IApplication IApplication;
#endif 	/* __IApplication_FWD_DEFINED__ */


#ifndef __Application_FWD_DEFINED__
#define __Application_FWD_DEFINED__

#ifdef __cplusplus
typedef class Application Application;
#else
typedef struct Application Application;
#endif /* __cplusplus */

#endif 	/* __Application_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CAN_MonitorApp_LIBRARY_DEFINED__
#define __CAN_MonitorApp_LIBRARY_DEFINED__

/* library CAN_MonitorApp */
/* [version][uuid] */ 

#define	MAX_BLOCK_NAME	( 32 )

#define	MAX_FILE_PATH	( 256 )

typedef 
enum __MIDL___MIDL_itf_BUSMASTER_0000_0000_0001
    {	None	= 0,
	Start	= ( None + 1 ) ,
	Stop	= ( Start + 1 ) ,
	Both	= ( Stop + 1 ) ,
	Stopped	= ( Both + 1 ) 
    } 	eLoggingTriggerState;

typedef 
enum __MIDL___MIDL_itf_BUSMASTER_0000_0000_0002
    {	System	= 0,
	Absolute	= ( System + 1 ) ,
	Relative	= ( Absolute + 1 ) 
    } 	eTimeMode;

typedef 
enum __MIDL___MIDL_itf_BUSMASTER_0000_0000_0003
    {	Hex	= 0,
	Deci	= ( Hex + 1 ) 
    } 	eNumericMode;

typedef 
enum __MIDL___MIDL_itf_BUSMASTER_0000_0000_0004
    {	Append	= 0,
	OverWrite	= ( Append + 1 ) 
    } 	eFileMode;

typedef 
enum __MIDL___MIDL_itf_BUSMASTER_0000_0000_0005
    {	Signal_watch	= 0,
	network_statistics	= ( Signal_watch + 1 ) ,
	trace	= ( network_statistics + 1 ) ,
	graph	= ( trace + 1 ) 
    } 	eWindow;

typedef 
enum __MIDL___MIDL_itf_BUSMASTER_0000_0000_0006
    {	Active	= 0,
	Passive	= ( Active + 1 ) ,
	Simulation	= ( Passive + 1 ) 
    } 	eControllerMode;

typedef 
enum __MIDL___MIDL_itf_BUSMASTER_0000_0000_0007
    {	all	= 0,
	timers	= ( all + 1 ) ,
	key	= ( timers + 1 ) ,
	message	= ( key + 1 ) ,
	error	= ( message + 1 ) 
    } 	eHandlerType;

typedef 
enum __MIDL___MIDL_itf_BUSMASTER_0000_0000_0008
    {	Log	= 0,
	Display	= ( Log + 1 ) 
    } 	EFILTERMODULE;

typedef struct __MIDL___MIDL_itf_BUSMASTER_0000_0000_0009
    {
    unsigned int m_unMsgID;
    unsigned char m_bEXTENDED;
    unsigned char m_bRTR;
    unsigned char m_ucDataLen;
    unsigned char m_ucData[ 8 ];
    unsigned char m_ucChannel;
    } 	CAN_MSGS;

typedef struct __MIDL___MIDL_itf_BUSMASTER_0000_0000_0010
    {
    double m_dBusLoad;
    double m_dPeakBusLoad;
    unsigned int m_unTotalMsgCount;
    unsigned int m_unMsgPerSecond;
    unsigned int m_unTotalTxMsgCount;
    double m_dTotalTxMsgRate;
    unsigned int m_unTxSTDMsgCount;
    double m_dTxSTDMsgRate;
    unsigned int m_unTxEXTDMsgCount;
    double m_dTxEXTMsgRate;
    unsigned int m_unTxSTD_RTRMsgCount;
    unsigned int m_unTxEXTD_RTRMsgCount;
    unsigned int m_unTotalRxMsgCount;
    double m_dTotalRxMsgRate;
    unsigned int m_unRxSTDMsgCount;
    double m_dRxSTDMsgRate;
    unsigned int m_unRxEXTDMsgCount;
    double m_dRxEXTMsgRate;
    unsigned int m_unRxSTD_RTRMsgCount;
    unsigned int m_unRxEXTD_RTRMsgCount;
    unsigned int m_unErrorTxCount;
    double m_dErrorTxRate;
    unsigned int m_unErrorRxCount;
    double m_dErrorRxRate;
    unsigned int m_unErrorTotalCount;
    double m_dErrorRate;
    unsigned int m_unDLCCount;
    double m_dBaudRate;
    double m_dTotalBusLoad;
    int m_nSamples;
    double m_dAvarageBusLoad;
    unsigned char m_ucTxErrorCounter;
    unsigned char m_ucRxErrorCounter;
    unsigned char m_ucTxPeakErrorCount;
    unsigned char m_ucRxPeakErrorCount;
    unsigned char m_ucStatus;
    } 	sBUSSTATISTICS_USR;

typedef struct __MIDL___MIDL_itf_BUSMASTER_0000_0000_0011
    {
    unsigned long m_dMessageCode;
    unsigned int m_dNumberOfSignals;
    unsigned int m_dMessageLength;
    BOOL m_bMessageFrameFormat;
    int m_nMsgDataFormat;
    } 	sMESSAGESTRUCT;

typedef struct tagTxBlock_Usr
    {
    BSTR m_strBlockName;
    BOOL m_bPeriodical;
    USHORT m_ushInterval;
    unsigned char m_ucTrigger;
    BOOL m_bTxAllFrame;
    UCHAR m_ucKeyValue;
    UINT m_unTimeInterval;
    } 	STXBLOCK_USR;

typedef struct tagTxBlock_Usr *PSTXBLOCK_USR;

typedef struct __MIDL___MIDL_itf_BUSMASTER_0000_0000_0012
    {
    UCHAR m_ucFilterType;
    UINT m_unMsgIDFrom;
    UINT m_unMsgIDTo;
    UCHAR m_ucDirection;
    UCHAR m_ucEXTENDED;
    UCHAR m_ucRTR;
    UCHAR m_ucChannel;
    } 	SFILTER_USR;

typedef struct __MIDL___MIDL_itf_BUSMASTER_0000_0000_0012 *PSFILTER_USR;

typedef struct __MIDL___MIDL_itf_BUSMASTER_0000_0000_0013
    {
    eLoggingTriggerState m_unTriggerType;
    UINT m_unStartID;
    UINT m_unStopID;
    } 	SLOGTRIGGER_USR;

typedef struct __MIDL___MIDL_itf_BUSMASTER_0000_0000_0013 *PSLOGTRIGGER_USR;

typedef struct tagLoggingBlockUsr
    {
    eTimeMode m_eLogTimerMode;
    eNumericMode m_eNumFormat;
    eFileMode m_eFileMode;
    SLOGTRIGGER_USR m_sLogTrigger;
    unsigned char m_sLogFileName[ 256 ];
    } 	SLOGGINGBLOCK_USR;

typedef struct tagLoggingBlockUsr *PSLOGGINGBLOCK_USR;


DEFINE_GUID(LIBID_CAN_MonitorApp,0xC3F9B41A,0xD3AD,0x4b36,0xBA,0x65,0xC5,0xC2,0xF1,0xA3,0x14,0xD9);

#ifndef __IApplication_INTERFACE_DEFINED__
#define __IApplication_INTERFACE_DEFINED__

/* interface IApplication */
/* [object][dual][oleautomation][uuid] */ 


DEFINE_GUID(IID_IApplication,0xB3DBF7E2,0x93DD,0x4c0c,0xA2,0x37,0x0E,0x8E,0x46,0xD3,0x54,0xC6);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B3DBF7E2-93DD-4c0c-A237-0E8E46D354C6")
    IApplication : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Connect( 
            BOOL bConnect) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadAllDll( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UnLoadAllDll( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartTxMsgBlock( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StopTxMsgBlock( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ResetHW( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ResetSW( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartLogging( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DisplayWindow( 
            eWindow eWhichWindow) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMsgInfo( 
            BSTR MsgName,
            sMESSAGESTRUCT *sMsgStruct) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendKeyValue( 
            UCHAR keyval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadConfiguration( 
            BSTR FileName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ImportDatabase( 
            BSTR DBFilePath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnableDisableHandlers( 
            BOOL bEnable,
            eHandlerType eType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetErrorCounter( 
            UCHAR *Tx,
            UCHAR *Rx,
            INT nChannel) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendCANMSg( 
            CAN_MSGS *sMsg) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetNetworkStatistics( 
            int nChannel,
            sBUSSTATISTICS_USR *sStat) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StopLogging( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteToLogFile( 
            USHORT BlockIndex,
            BSTR pcStr) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveConfiguration( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddLoggingBlock( 
            SLOGGINGBLOCK_USR *psLoggingBlock) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveConfigurationAs( 
            BSTR ConfigPath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddTxBlock( 
            STXBLOCK_USR *psTxBlock) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetTxBlockCount( 
            USHORT *Result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetTxBlock( 
            USHORT BlockIndex,
            STXBLOCK_USR *psTxBlock) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteTxBlock( 
            USHORT BlockIndex) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearTxBlockList( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddMsgToTxBlock( 
            USHORT BlockIndex,
            CAN_MSGS *psMsg) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMsgCount( 
            USHORT BlockIndex,
            USHORT *Result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMsgFromTxBlock( 
            USHORT BlockIndex,
            USHORT MsgIndex,
            CAN_MSGS *psMsg) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteMsgFromTxBlock( 
            USHORT BlockIndex,
            USHORT FrameIndex) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearMsgList( 
            /* [in] */ USHORT BlockIndex) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddFilterScheme( 
            /* [in] */ BSTR pcFilterName,
            /* [in] */ VARIANT_BOOL FilterType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFilterScheme( 
            /* [in] */ USHORT FilterSchINdex,
            /* [in] */ BSTR pcFilterName,
            /* [in] */ VARIANT_BOOL *FilterType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFilterSchCount( 
            /* [out] */ USHORT *pTotal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UpdateFilterSch( 
            /* [in] */ USHORT FilterSchIndex,
            /* [in] */ SFILTER_USR *psFilter) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFilterCountInSch( 
            /* [in] */ USHORT FilterSchIndex,
            /* [out] */ USHORT *pTotal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFilterInFilterSch( 
            /* [in] */ USHORT FilterSchIndex,
            /* [in] */ USHORT FilterIndex,
            /* [out] */ SFILTER_USR *psFilter) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteFilterInSch( 
            /* [in] */ USHORT FilterSchIndex,
            /* [in] */ USHORT FilterIndex) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnableFilterSch( 
            /* [in] */ EFILTERMODULE eModule,
            /* [in] */ BOOL Enable) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddSimulatedSystem( 
            /* [in] */ BSTR pcSimPath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSimulatedSystemCount( 
            /* [out] */ USHORT *pSimTotal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSimulatedSystemName( 
            /* [in] */ USHORT SimSysIndex,
            /* [out] */ BSTR *pcSimPath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteSimulatedSystem( 
            /* [in] */ USHORT SimSysIndex) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveLoggingBlock( 
            /* [in] */ USHORT BlockIndex) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetLoggingBlockCount( 
            /* [out] */ USHORT *BlockCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearLoggingBlockList( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetLoggingBlock( 
            /* [in] */ USHORT BlockIndex,
            /* [out] */ SLOGGINGBLOCK_USR *psLoggingBlock) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IApplicationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IApplication * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IApplication * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IApplication * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IApplication * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IApplication * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IApplication * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IApplication * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            IApplication * This,
            BOOL bConnect);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadAllDll )( 
            IApplication * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *UnLoadAllDll )( 
            IApplication * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StartTxMsgBlock )( 
            IApplication * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StopTxMsgBlock )( 
            IApplication * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ResetHW )( 
            IApplication * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSW )( 
            IApplication * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StartLogging )( 
            IApplication * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DisplayWindow )( 
            IApplication * This,
            eWindow eWhichWindow);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMsgInfo )( 
            IApplication * This,
            BSTR MsgName,
            sMESSAGESTRUCT *sMsgStruct);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SendKeyValue )( 
            IApplication * This,
            UCHAR keyval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadConfiguration )( 
            IApplication * This,
            BSTR FileName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ImportDatabase )( 
            IApplication * This,
            BSTR DBFilePath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnableDisableHandlers )( 
            IApplication * This,
            BOOL bEnable,
            eHandlerType eType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetErrorCounter )( 
            IApplication * This,
            UCHAR *Tx,
            UCHAR *Rx,
            INT nChannel);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SendCANMSg )( 
            IApplication * This,
            CAN_MSGS *sMsg);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetNetworkStatistics )( 
            IApplication * This,
            int nChannel,
            sBUSSTATISTICS_USR *sStat);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StopLogging )( 
            IApplication * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WriteToLogFile )( 
            IApplication * This,
            USHORT BlockIndex,
            BSTR pcStr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SaveConfiguration )( 
            IApplication * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddLoggingBlock )( 
            IApplication * This,
            SLOGGINGBLOCK_USR *psLoggingBlock);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SaveConfigurationAs )( 
            IApplication * This,
            BSTR ConfigPath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddTxBlock )( 
            IApplication * This,
            STXBLOCK_USR *psTxBlock);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetTxBlockCount )( 
            IApplication * This,
            USHORT *Result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetTxBlock )( 
            IApplication * This,
            USHORT BlockIndex,
            STXBLOCK_USR *psTxBlock);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteTxBlock )( 
            IApplication * This,
            USHORT BlockIndex);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClearTxBlockList )( 
            IApplication * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddMsgToTxBlock )( 
            IApplication * This,
            USHORT BlockIndex,
            CAN_MSGS *psMsg);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMsgCount )( 
            IApplication * This,
            USHORT BlockIndex,
            USHORT *Result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMsgFromTxBlock )( 
            IApplication * This,
            USHORT BlockIndex,
            USHORT MsgIndex,
            CAN_MSGS *psMsg);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteMsgFromTxBlock )( 
            IApplication * This,
            USHORT BlockIndex,
            USHORT FrameIndex);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClearMsgList )( 
            IApplication * This,
            /* [in] */ USHORT BlockIndex);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddFilterScheme )( 
            IApplication * This,
            /* [in] */ BSTR pcFilterName,
            /* [in] */ VARIANT_BOOL FilterType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFilterScheme )( 
            IApplication * This,
            /* [in] */ USHORT FilterSchINdex,
            /* [in] */ BSTR pcFilterName,
            /* [in] */ VARIANT_BOOL *FilterType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFilterSchCount )( 
            IApplication * This,
            /* [out] */ USHORT *pTotal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *UpdateFilterSch )( 
            IApplication * This,
            /* [in] */ USHORT FilterSchIndex,
            /* [in] */ SFILTER_USR *psFilter);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFilterCountInSch )( 
            IApplication * This,
            /* [in] */ USHORT FilterSchIndex,
            /* [out] */ USHORT *pTotal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFilterInFilterSch )( 
            IApplication * This,
            /* [in] */ USHORT FilterSchIndex,
            /* [in] */ USHORT FilterIndex,
            /* [out] */ SFILTER_USR *psFilter);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteFilterInSch )( 
            IApplication * This,
            /* [in] */ USHORT FilterSchIndex,
            /* [in] */ USHORT FilterIndex);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnableFilterSch )( 
            IApplication * This,
            /* [in] */ EFILTERMODULE eModule,
            /* [in] */ BOOL Enable);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddSimulatedSystem )( 
            IApplication * This,
            /* [in] */ BSTR pcSimPath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSimulatedSystemCount )( 
            IApplication * This,
            /* [out] */ USHORT *pSimTotal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSimulatedSystemName )( 
            IApplication * This,
            /* [in] */ USHORT SimSysIndex,
            /* [out] */ BSTR *pcSimPath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteSimulatedSystem )( 
            IApplication * This,
            /* [in] */ USHORT SimSysIndex);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RemoveLoggingBlock )( 
            IApplication * This,
            /* [in] */ USHORT BlockIndex);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetLoggingBlockCount )( 
            IApplication * This,
            /* [out] */ USHORT *BlockCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClearLoggingBlockList )( 
            IApplication * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetLoggingBlock )( 
            IApplication * This,
            /* [in] */ USHORT BlockIndex,
            /* [out] */ SLOGGINGBLOCK_USR *psLoggingBlock);
        
        END_INTERFACE
    } IApplicationVtbl;

    interface IApplication
    {
        CONST_VTBL struct IApplicationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IApplication_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IApplication_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IApplication_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IApplication_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IApplication_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IApplication_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IApplication_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IApplication_Connect(This,bConnect)	\
    ( (This)->lpVtbl -> Connect(This,bConnect) ) 

#define IApplication_LoadAllDll(This)	\
    ( (This)->lpVtbl -> LoadAllDll(This) ) 

#define IApplication_UnLoadAllDll(This)	\
    ( (This)->lpVtbl -> UnLoadAllDll(This) ) 

#define IApplication_StartTxMsgBlock(This)	\
    ( (This)->lpVtbl -> StartTxMsgBlock(This) ) 

#define IApplication_StopTxMsgBlock(This)	\
    ( (This)->lpVtbl -> StopTxMsgBlock(This) ) 

#define IApplication_ResetHW(This)	\
    ( (This)->lpVtbl -> ResetHW(This) ) 

#define IApplication_ResetSW(This)	\
    ( (This)->lpVtbl -> ResetSW(This) ) 

#define IApplication_StartLogging(This)	\
    ( (This)->lpVtbl -> StartLogging(This) ) 

#define IApplication_DisplayWindow(This,eWhichWindow)	\
    ( (This)->lpVtbl -> DisplayWindow(This,eWhichWindow) ) 

#define IApplication_GetMsgInfo(This,MsgName,sMsgStruct)	\
    ( (This)->lpVtbl -> GetMsgInfo(This,MsgName,sMsgStruct) ) 

#define IApplication_SendKeyValue(This,keyval)	\
    ( (This)->lpVtbl -> SendKeyValue(This,keyval) ) 

#define IApplication_LoadConfiguration(This,FileName)	\
    ( (This)->lpVtbl -> LoadConfiguration(This,FileName) ) 

#define IApplication_ImportDatabase(This,DBFilePath)	\
    ( (This)->lpVtbl -> ImportDatabase(This,DBFilePath) ) 

#define IApplication_EnableDisableHandlers(This,bEnable,eType)	\
    ( (This)->lpVtbl -> EnableDisableHandlers(This,bEnable,eType) ) 

#define IApplication_GetErrorCounter(This,Tx,Rx,nChannel)	\
    ( (This)->lpVtbl -> GetErrorCounter(This,Tx,Rx,nChannel) ) 

#define IApplication_SendCANMSg(This,sMsg)	\
    ( (This)->lpVtbl -> SendCANMSg(This,sMsg) ) 

#define IApplication_GetNetworkStatistics(This,nChannel,sStat)	\
    ( (This)->lpVtbl -> GetNetworkStatistics(This,nChannel,sStat) ) 

#define IApplication_StopLogging(This)	\
    ( (This)->lpVtbl -> StopLogging(This) ) 

#define IApplication_WriteToLogFile(This,BlockIndex,pcStr)	\
    ( (This)->lpVtbl -> WriteToLogFile(This,BlockIndex,pcStr) ) 

#define IApplication_SaveConfiguration(This)	\
    ( (This)->lpVtbl -> SaveConfiguration(This) ) 

#define IApplication_AddLoggingBlock(This,psLoggingBlock)	\
    ( (This)->lpVtbl -> AddLoggingBlock(This,psLoggingBlock) ) 

#define IApplication_SaveConfigurationAs(This,ConfigPath)	\
    ( (This)->lpVtbl -> SaveConfigurationAs(This,ConfigPath) ) 

#define IApplication_AddTxBlock(This,psTxBlock)	\
    ( (This)->lpVtbl -> AddTxBlock(This,psTxBlock) ) 

#define IApplication_GetTxBlockCount(This,Result)	\
    ( (This)->lpVtbl -> GetTxBlockCount(This,Result) ) 

#define IApplication_GetTxBlock(This,BlockIndex,psTxBlock)	\
    ( (This)->lpVtbl -> GetTxBlock(This,BlockIndex,psTxBlock) ) 

#define IApplication_DeleteTxBlock(This,BlockIndex)	\
    ( (This)->lpVtbl -> DeleteTxBlock(This,BlockIndex) ) 

#define IApplication_ClearTxBlockList(This)	\
    ( (This)->lpVtbl -> ClearTxBlockList(This) ) 

#define IApplication_AddMsgToTxBlock(This,BlockIndex,psMsg)	\
    ( (This)->lpVtbl -> AddMsgToTxBlock(This,BlockIndex,psMsg) ) 

#define IApplication_GetMsgCount(This,BlockIndex,Result)	\
    ( (This)->lpVtbl -> GetMsgCount(This,BlockIndex,Result) ) 

#define IApplication_GetMsgFromTxBlock(This,BlockIndex,MsgIndex,psMsg)	\
    ( (This)->lpVtbl -> GetMsgFromTxBlock(This,BlockIndex,MsgIndex,psMsg) ) 

#define IApplication_DeleteMsgFromTxBlock(This,BlockIndex,FrameIndex)	\
    ( (This)->lpVtbl -> DeleteMsgFromTxBlock(This,BlockIndex,FrameIndex) ) 

#define IApplication_ClearMsgList(This,BlockIndex)	\
    ( (This)->lpVtbl -> ClearMsgList(This,BlockIndex) ) 

#define IApplication_AddFilterScheme(This,pcFilterName,FilterType)	\
    ( (This)->lpVtbl -> AddFilterScheme(This,pcFilterName,FilterType) ) 

#define IApplication_GetFilterScheme(This,FilterSchINdex,pcFilterName,FilterType)	\
    ( (This)->lpVtbl -> GetFilterScheme(This,FilterSchINdex,pcFilterName,FilterType) ) 

#define IApplication_GetFilterSchCount(This,pTotal)	\
    ( (This)->lpVtbl -> GetFilterSchCount(This,pTotal) ) 

#define IApplication_UpdateFilterSch(This,FilterSchIndex,psFilter)	\
    ( (This)->lpVtbl -> UpdateFilterSch(This,FilterSchIndex,psFilter) ) 

#define IApplication_GetFilterCountInSch(This,FilterSchIndex,pTotal)	\
    ( (This)->lpVtbl -> GetFilterCountInSch(This,FilterSchIndex,pTotal) ) 

#define IApplication_GetFilterInFilterSch(This,FilterSchIndex,FilterIndex,psFilter)	\
    ( (This)->lpVtbl -> GetFilterInFilterSch(This,FilterSchIndex,FilterIndex,psFilter) ) 

#define IApplication_DeleteFilterInSch(This,FilterSchIndex,FilterIndex)	\
    ( (This)->lpVtbl -> DeleteFilterInSch(This,FilterSchIndex,FilterIndex) ) 

#define IApplication_EnableFilterSch(This,eModule,Enable)	\
    ( (This)->lpVtbl -> EnableFilterSch(This,eModule,Enable) ) 

#define IApplication_AddSimulatedSystem(This,pcSimPath)	\
    ( (This)->lpVtbl -> AddSimulatedSystem(This,pcSimPath) ) 

#define IApplication_GetSimulatedSystemCount(This,pSimTotal)	\
    ( (This)->lpVtbl -> GetSimulatedSystemCount(This,pSimTotal) ) 

#define IApplication_GetSimulatedSystemName(This,SimSysIndex,pcSimPath)	\
    ( (This)->lpVtbl -> GetSimulatedSystemName(This,SimSysIndex,pcSimPath) ) 

#define IApplication_DeleteSimulatedSystem(This,SimSysIndex)	\
    ( (This)->lpVtbl -> DeleteSimulatedSystem(This,SimSysIndex) ) 

#define IApplication_RemoveLoggingBlock(This,BlockIndex)	\
    ( (This)->lpVtbl -> RemoveLoggingBlock(This,BlockIndex) ) 

#define IApplication_GetLoggingBlockCount(This,BlockCount)	\
    ( (This)->lpVtbl -> GetLoggingBlockCount(This,BlockCount) ) 

#define IApplication_ClearLoggingBlockList(This)	\
    ( (This)->lpVtbl -> ClearLoggingBlockList(This) ) 

#define IApplication_GetLoggingBlock(This,BlockIndex,psLoggingBlock)	\
    ( (This)->lpVtbl -> GetLoggingBlock(This,BlockIndex,psLoggingBlock) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IApplication_INTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Application,0x92D435C1,0xA552,0x4435,0xAD,0x1E,0x46,0x8B,0x4C,0x17,0xBD,0xC7);

#ifdef __cplusplus

class DECLSPEC_UUID("92D435C1-A552-4435-AD1E-468B4C17BDC7")
Application;
#endif
#endif /* __CAN_MonitorApp_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


