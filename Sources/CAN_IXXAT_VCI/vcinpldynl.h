/*****************************************************************************
 IXXAT Automation GmbH
******************************************************************************

 File    : VCINPLDYNL.H
 Summary : Native VCI Programming Library.- for dynamic loading of vcinpl.dll

 Date    : 2007-07-03
 Author  : Michael Ummenhofer

 Compiler: MSVC

******************************************************************************
 all rights reserved
*****************************************************************************/

#ifndef _VCINPLDYNL_H_
#define _VCINPLDYNL_H_

#include <vcinpl.h>
#include <pshpack4.h>

/*****************************************************************************
* constants and macros
*****************************************************************************/

//*** DLL access Macros
#define DYNFUNC(FUNC) g_pfNPL_##FUNC
#define DYNCALL(FUNC) DYNFUNC(FUNC)         //  Compatibility
#define SAFEDYNCALL(FUNC,ARGS) ((DYNFUNC(FUNC)==NULL) ? E_UNEXPECTED : DYNFUNC(FUNC) ARGS)
#define DECLARE_FUNCTION_POINTER_EXT(FUNC) extern PF_NPL_##FUNC DYNFUNC(FUNC);


/*##########################################################################*/
/*##                                                                      ##*/
/*##   exported API functions (definitions)                               ##*/
/*##                                                                      ##*/
/*##########################################################################*/

/* C calling convention */
#ifdef __cplusplus
extern "C"
{
#endif

    /*****************************************************************************
     * general VCI functions
     ****************************************************************************/

    typedef HRESULT
    (VCIAPI* PF_NPL_vciInitialize  )( void );

    typedef void
    (VCIAPI* PF_NPL_vciFormatError)( IN  HRESULT hrError,
                                     OUT PCHAR   pszText,
                                     IN  UINT32  dwSize );

    typedef void
    (VCIAPI* PF_NPL_vciDisplayError)( IN HWND    hwndParent,
                                      IN PCHAR   pszCaption,
                                      IN HRESULT hrError );

    typedef HRESULT
    (VCIAPI* PF_NPL_vciGetVersion)( OUT PUINT32 pdwMajorVersion,
                                    OUT PUINT32 pdwMinorVersion );

    typedef HRESULT
    (VCIAPI* PF_NPL_vciLuidToChar)( IN  REFVCIID rVciid,
                                    OUT PCHAR    pszLuid,
                                    IN  LONG     cbSize );

    typedef HRESULT
    (VCIAPI* PF_NPL_vciCharToLuid)( IN  PCHAR  pszLuid,
                                    OUT PVCIID pVciid );

    typedef HRESULT
    (VCIAPI* PF_NPL_vciGuidToChar)( IN  REFGUID rGuid,
                                    OUT PCHAR   pszGuid,
                                    IN  LONG    cbSize );

    typedef HRESULT
    (VCIAPI* PF_NPL_vciCharToGuid)( IN  PCHAR pszGuid,
                                    OUT GUID* pGuid );


    /*****************************************************************************
     * event specific functions
     ****************************************************************************/

    typedef HRESULT
    (VCIAPI* PF_NPL_vciEventCreate)( IN  BOOL    fManReset,
                                     IN  BOOL    fInitState,
                                     OUT PHANDLE phEvent );

    typedef HRESULT
    (VCIAPI* PF_NPL_vciEventDelete)( IN HANDLE hEvent );

    typedef HRESULT
    (VCIAPI* PF_NPL_vciEventSignal)( IN HANDLE hEvent );

    typedef HRESULT
    (VCIAPI* PF_NPL_vciEventReset)( HANDLE hEvent );

    typedef HRESULT
    (VCIAPI* PF_NPL_vciEventWaitFor)( IN HANDLE hEvent,
                                      IN UINT32 dwMsTimeout );


    /*****************************************************************************
     * device manager specific functions
     ****************************************************************************/

    typedef HRESULT
    (VCIAPI* PF_NPL_vciEnumDeviceOpen)( OUT PHANDLE hEnum );

    typedef HRESULT
    (VCIAPI* PF_NPL_vciEnumDeviceClose )( IN HANDLE hEnum );

    typedef HRESULT
    (VCIAPI* PF_NPL_vciEnumDeviceNext)( IN  HANDLE         hEnum,
                                        OUT PVCIDEVICEINFO pInfo );

    typedef HRESULT
    (VCIAPI* PF_NPL_vciEnumDeviceReset)( IN HANDLE hEnum );

    typedef HRESULT
    (VCIAPI* PF_NPL_vciEnumDeviceWaitEvent)( IN HANDLE hEnum,
            IN UINT32 dwMsTimeout );

    typedef HRESULT
    (VCIAPI* PF_NPL_vciFindDeviceByHwid)( IN  REFGUID rHwid,
                                          OUT PVCIID  pVciid );

    typedef HRESULT
    (VCIAPI* PF_NPL_vciFindDeviceByClass)( IN  REFGUID rClass,
                                           IN  UINT32  dwInst,
                                           OUT PVCIID  pVciid );

    typedef HRESULT
    (VCIAPI* PF_NPL_vciSelectDeviceDlg)( IN  HWND   hwndParent,
                                         OUT PVCIID pVciid );


    /*****************************************************************************
     * device specific functions
     ****************************************************************************/

    typedef HRESULT
    (VCIAPI* PF_NPL_vciDeviceOpen)( IN  REFVCIID rVciid,
                                    OUT PHANDLE  phDevice );

    typedef HRESULT
    (VCIAPI* PF_NPL_vciDeviceOpenDlg)( IN  HWND    hwndParent,
                                       OUT PHANDLE phDevice );

    typedef HRESULT
    (VCIAPI* PF_NPL_vciDeviceClose)( IN HANDLE hDevice );

    typedef HRESULT
    (VCIAPI* PF_NPL_vciDeviceGetInfo)( IN  HANDLE         hDevice,
                                       OUT PVCIDEVICEINFO pInfo );

    typedef HRESULT
    (VCIAPI* PF_NPL_vciDeviceGetCaps)( IN  HANDLE         hDevice,
                                       OUT PVCIDEVICECAPS pCaps );


    /*****************************************************************************
     * CAN controller specific functions
     ****************************************************************************/

    typedef HRESULT
    (VCIAPI* PF_NPL_canControlOpen)( IN  HANDLE  hDevice,
                                     IN  UINT32  dwCanNo,
                                     OUT PHANDLE phCanCtl );

    typedef HRESULT
    (VCIAPI* PF_NPL_canControlClose)( IN HANDLE hCanCtl );

    typedef HRESULT
    (VCIAPI* PF_NPL_canControlGetCaps)( IN  HANDLE           hCanCtl,
                                        OUT PCANCAPABILITIES pCanCaps );

    typedef HRESULT
    (VCIAPI* PF_NPL_canControlGetStatus)( IN  HANDLE         hCanCtl,
                                          OUT PCANLINESTATUS pStatus );

    typedef HRESULT
    (VCIAPI* PF_NPL_canControlDetectBitrate)( IN  HANDLE  hCanCtl,
            IN  UINT16  wMsTimeout,
            IN  UINT32  dwCount,
            IN  PUINT8  pabBtr0,
            IN  PUINT8  pabBtr1,
            OUT PINT32  plIndex );

    typedef HRESULT
    (VCIAPI* PF_NPL_canControlInitialize)( IN HANDLE hCanCtl,
                                           IN UINT8  bMode,
                                           IN UINT8  bBtr0,
                                           IN UINT8  bBtr1 );

    typedef HRESULT
    (VCIAPI* PF_NPL_canControlReset)( IN HANDLE hCanCtl );

    typedef HRESULT
    (VCIAPI* PF_NPL_canControlStart)( IN HANDLE hCanCtl,
                                      IN BOOL   fStart );

    typedef HRESULT
    (VCIAPI* PF_NPL_canControlSetAccFilter)( IN HANDLE hCanCtl,
            IN BOOL   fExtend,
            IN UINT32 dwCode,
            IN UINT32 dwMask );

    typedef HRESULT
    (VCIAPI* PF_NPL_canControlAddFilterIds)( IN HANDLE hCanCtl,
            IN BOOL   fExtend,
            IN UINT32 dwCode,
            IN UINT32 dwMask );

    typedef HRESULT
    (VCIAPI* PF_NPL_canControlRemFilterIds)( IN HANDLE hCanCtl,
            IN BOOL   fExtend,
            IN UINT32 dwCode,
            IN UINT32 dwMask );


    /*****************************************************************************
     * CAN message channel specific functions
     ****************************************************************************/

    typedef HRESULT
    (VCIAPI* PF_NPL_canChannelOpen)( IN  HANDLE  hDevice,
                                     IN  UINT32  dwCanNo,
                                     IN  BOOL    fExclusive,
                                     OUT PHANDLE phCanChn );

    typedef HRESULT
    (VCIAPI* PF_NPL_canChannelClose)( IN HANDLE hCanChn );

    typedef HRESULT
    (VCIAPI* PF_NPL_canChannelGetCaps)( IN  HANDLE           hCanChn,
                                        OUT PCANCAPABILITIES pCanCaps );

    typedef HRESULT
    (VCIAPI* PF_NPL_canChannelGetStatus)( IN  HANDLE         hCanChn,
                                          OUT PCANCHANSTATUS pStatus );

    typedef HRESULT
    (VCIAPI* PF_NPL_canChannelInitialize)( IN HANDLE hCanChn,
                                           IN UINT16 wRxFifoSize,
                                           IN UINT16 wRxThreshold,
                                           IN UINT16 wTxFifoSize,
                                           IN UINT16 wTxThreshold );

    typedef HRESULT
    (VCIAPI* PF_NPL_canChannelActivate)( IN HANDLE hCanChn,
                                         IN BOOL   fEnable );

    typedef HRESULT
    (VCIAPI* PF_NPL_canChannelPeekMessage)( IN  HANDLE  hCanChn,
                                            OUT PCANMSG pCanMsg );

    typedef HRESULT
    (VCIAPI* PF_NPL_canChannelPostMessage)( IN HANDLE  hCanChn,
                                            IN PCANMSG pCanMsg );

    typedef HRESULT
    (VCIAPI* PF_NPL_canChannelWaitRxEvent)( IN HANDLE hCanChn,
                                            IN UINT32 dwMsTimeout );

    typedef HRESULT
    (VCIAPI* PF_NPL_canChannelWaitTxEvent)( IN HANDLE hCanChn,
                                            IN UINT32 dwMsTimeout );

    typedef HRESULT
    (VCIAPI* PF_NPL_canChannelReadMessage)( IN  HANDLE  hCanChn,
                                            IN  UINT32  dwMsTimeout,
                                            OUT PCANMSG pCanMsg );

    typedef HRESULT
    (VCIAPI* PF_NPL_canChannelReadMultipleMessages)( IN     HANDLE  hCanChn,
            IN     UINT32  dwMsTimeout,
            IN OUT UINT32* pdwNum,
            OUT    CANMSG aCanMsg[] );



    typedef HRESULT
    (VCIAPI* PF_NPL_canChannelSendMessage)( IN HANDLE  hCanChn,
                                            IN UINT32  dwMsTimeout,
                                            IN PCANMSG pCanMsg );


    /*****************************************************************************
     * cyclic CAN message scheduler specific functions
     ****************************************************************************/

    typedef HRESULT
    (VCIAPI* PF_NPL_canSchedulerOpen)( IN  HANDLE  hDevice,
                                       IN  UINT32  dwCanNo,
                                       OUT PHANDLE phCanShd );

    typedef HRESULT
    (VCIAPI* PF_NPL_canSchedulerClose)( IN HANDLE hCanShd );

    typedef HRESULT
    (VCIAPI* PF_NPL_canSchedulerGetCaps)( IN  HANDLE           hCanShd,
                                          OUT PCANCAPABILITIES pCanCaps );

    typedef HRESULT
    (VCIAPI* PF_NPL_canSchedulerGetStatus)( IN  HANDLE              hCanShd,
                                            OUT PCANSCHEDULERSTATUS pStatus );

    typedef HRESULT
    (VCIAPI* PF_NPL_canSchedulerActivate)( IN HANDLE hCanShd,
                                           IN BOOL   fEnable );

    typedef HRESULT
    (VCIAPI* PF_NPL_canSchedulerReset)( IN HANDLE hCanShd );

    typedef HRESULT
    (VCIAPI* PF_NPL_canSchedulerAddMessage)( IN  HANDLE          hCanShd,
            IN  PCANCYCLICTXMSG pMessage,
            OUT PUINT32         pdwIndex );

    typedef HRESULT
    (VCIAPI* PF_NPL_canSchedulerRemMessage)( IN HANDLE hCanShd,
            IN UINT32 dwIndex );

    typedef HRESULT
    (VCIAPI* PF_NPL_canSchedulerStartMessage)( IN HANDLE hCanShd,
            IN UINT32 dwIndex,
            IN UINT16 wRepeat );

    typedef HRESULT
    (VCIAPI* PF_NPL_canSchedulerStopMessage)( IN HANDLE hCanShd,
            IN UINT32 dwIndex );


    /*****************************************************************************
     * LIN controller specific functions
     ****************************************************************************/

    typedef HRESULT
    (VCIAPI* PF_NPL_linControlOpen)( IN  HANDLE  hDevice,
                                     IN  UINT32  dwLinNo,
                                     OUT PHANDLE phLinCtl );

    typedef HRESULT
    (VCIAPI* PF_NPL_linControlClose)( IN HANDLE hLinCtl );

    typedef HRESULT
    (VCIAPI* PF_NPL_linControlGetCaps)( IN  HANDLE           hLinCtl,
                                        OUT PLINCAPABILITIES pLinCaps );

    typedef HRESULT
    (VCIAPI* PF_NPL_linControlGetStatus)( IN  HANDLE         hLinCtl,
                                          OUT PLINLINESTATUS pStatus );

    typedef HRESULT
    (VCIAPI* PF_NPL_linControlInitialize)( IN HANDLE hLinCtl,
                                           IN UINT8  bMode,
                                           IN UINT16 wBitrate );

    typedef HRESULT
    (VCIAPI* PF_NPL_linControlReset)( IN HANDLE hLinCtl );

    typedef HRESULT
    (VCIAPI* PF_NPL_linControlStart)( IN HANDLE hLinCtl,
                                      IN BOOL   fStart );

    typedef HRESULT
    (VCIAPI* PF_NPL_linControlWriteMessage)( IN HANDLE  hLinCtl,
            IN BOOL    fSend,
            IN PLINMSG pLinMsg );


    /*****************************************************************************
     * LIN message monitor specific functions
     ****************************************************************************/

    typedef HRESULT
    (VCIAPI* PF_NPL_linMonitorOpen)( IN  HANDLE  hDevice,
                                     IN  UINT32  dwLinNo,
                                     IN  BOOL    fExclusive,
                                     OUT PHANDLE phLinMon );

    typedef HRESULT
    (VCIAPI* PF_NPL_linMonitorClose)( IN HANDLE hLinMon );

    typedef HRESULT
    (VCIAPI* PF_NPL_linMonitorGetCaps)( IN  HANDLE           hLinMon,
                                        OUT PLINCAPABILITIES pLinCaps );

    typedef HRESULT
    (VCIAPI* PF_NPL_linMonitorGetStatus)( IN  HANDLE            hLinMon,
                                          OUT PLINMONITORSTATUS pStatus );

    typedef HRESULT
    (VCIAPI* PF_NPL_linMonitorInitialize)( IN HANDLE hLinMon,
                                           IN UINT16 wFifoSize,
                                           IN UINT16 wThreshold );

    typedef HRESULT
    (VCIAPI* PF_NPL_linMonitorActivate)( IN HANDLE hLinMon,
                                         IN BOOL   fEnable );

    typedef HRESULT
    (VCIAPI* PF_NPL_linMonitorPeekMessage)( IN  HANDLE  hLinMon,
                                            OUT PLINMSG pLinMsg );

    typedef HRESULT
    (VCIAPI* PF_NPL_linMonitorWaitRxEvent)( IN HANDLE hLinMon,
                                            IN UINT32 dwMsTimeout );

    typedef HRESULT
    (VCIAPI* PF_NPL_linMonitorReadMessage)( IN  HANDLE  hLinMon,
                                            IN  UINT32  dwMsTimeout,
                                            OUT PLINMSG pLinMsg );


    /*##########################################################################*/
    /*##                                                                      ##*/
    /*##   exported API functions (extern declarations)                       ##*/
    /*##                                                                      ##*/
    /*##########################################################################*/

    /*****************************************************************************
     * general VCI functions
     ****************************************************************************/
    DECLARE_FUNCTION_POINTER_EXT(vciInitialize);
    DECLARE_FUNCTION_POINTER_EXT(vciFormatError);
    DECLARE_FUNCTION_POINTER_EXT(vciDisplayError);
    DECLARE_FUNCTION_POINTER_EXT(vciGetVersion);
    DECLARE_FUNCTION_POINTER_EXT(vciLuidToChar);
    DECLARE_FUNCTION_POINTER_EXT(vciCharToLuid);
    DECLARE_FUNCTION_POINTER_EXT(vciGuidToChar);
    DECLARE_FUNCTION_POINTER_EXT(vciCharToGuid);

    /*****************************************************************************
     * event specific functions
     ****************************************************************************/
    DECLARE_FUNCTION_POINTER_EXT(vciEventCreate);
    DECLARE_FUNCTION_POINTER_EXT(vciEventDelete);
    DECLARE_FUNCTION_POINTER_EXT(vciEventSignal);
    DECLARE_FUNCTION_POINTER_EXT(vciEventReset);
    DECLARE_FUNCTION_POINTER_EXT(vciEventWaitFor);

    /*****************************************************************************
     * device manager specific functions
     ****************************************************************************/
    DECLARE_FUNCTION_POINTER_EXT(vciEnumDeviceOpen);
    DECLARE_FUNCTION_POINTER_EXT(vciEnumDeviceClose );
    DECLARE_FUNCTION_POINTER_EXT(vciEnumDeviceNext);
    DECLARE_FUNCTION_POINTER_EXT(vciEnumDeviceReset);
    DECLARE_FUNCTION_POINTER_EXT(vciEnumDeviceWaitEvent);
    DECLARE_FUNCTION_POINTER_EXT(vciFindDeviceByHwid);
    DECLARE_FUNCTION_POINTER_EXT(vciFindDeviceByClass);
    DECLARE_FUNCTION_POINTER_EXT(vciSelectDeviceDlg);

    /*****************************************************************************
     * device specific functions
     ****************************************************************************/
    DECLARE_FUNCTION_POINTER_EXT(vciDeviceOpen);
    DECLARE_FUNCTION_POINTER_EXT(vciDeviceOpenDlg);
    DECLARE_FUNCTION_POINTER_EXT(vciDeviceClose);
    DECLARE_FUNCTION_POINTER_EXT(vciDeviceGetInfo);
    DECLARE_FUNCTION_POINTER_EXT(vciDeviceGetCaps);

    /*****************************************************************************
     * CAN controller specific functions
     ****************************************************************************/
    DECLARE_FUNCTION_POINTER_EXT(canControlOpen);
    DECLARE_FUNCTION_POINTER_EXT(canControlClose);
    DECLARE_FUNCTION_POINTER_EXT(canControlGetCaps);
    DECLARE_FUNCTION_POINTER_EXT(canControlGetStatus);
    DECLARE_FUNCTION_POINTER_EXT(canControlDetectBitrate);
    DECLARE_FUNCTION_POINTER_EXT(canControlInitialize);
    DECLARE_FUNCTION_POINTER_EXT(canControlReset);
    DECLARE_FUNCTION_POINTER_EXT(canControlStart);
    DECLARE_FUNCTION_POINTER_EXT(canControlSetAccFilter);
    DECLARE_FUNCTION_POINTER_EXT(canControlAddFilterIds);
    DECLARE_FUNCTION_POINTER_EXT(canControlRemFilterIds);

    /*****************************************************************************
     * CAN message channel specific functions
     ****************************************************************************/
    DECLARE_FUNCTION_POINTER_EXT(canChannelOpen);
    DECLARE_FUNCTION_POINTER_EXT(canChannelClose);
    DECLARE_FUNCTION_POINTER_EXT(canChannelGetCaps);
    DECLARE_FUNCTION_POINTER_EXT(canChannelGetStatus);
    DECLARE_FUNCTION_POINTER_EXT(canChannelInitialize);
    DECLARE_FUNCTION_POINTER_EXT(canChannelActivate);
    DECLARE_FUNCTION_POINTER_EXT(canChannelPeekMessage);
    DECLARE_FUNCTION_POINTER_EXT(canChannelPostMessage);
    DECLARE_FUNCTION_POINTER_EXT(canChannelWaitRxEvent);
    DECLARE_FUNCTION_POINTER_EXT(canChannelWaitTxEvent);
    DECLARE_FUNCTION_POINTER_EXT(canChannelReadMessage);
    DECLARE_FUNCTION_POINTER_EXT(canChannelReadMultipleMessages);
    DECLARE_FUNCTION_POINTER_EXT(canChannelSendMessage);

    /*****************************************************************************
     * cyclic CAN message scheduler specific functions
     ****************************************************************************/
    DECLARE_FUNCTION_POINTER_EXT(canSchedulerOpen);
    DECLARE_FUNCTION_POINTER_EXT(canSchedulerClose);
    DECLARE_FUNCTION_POINTER_EXT(canSchedulerGetCaps);
    DECLARE_FUNCTION_POINTER_EXT(canSchedulerGetStatus);
    DECLARE_FUNCTION_POINTER_EXT(canSchedulerActivate);
    DECLARE_FUNCTION_POINTER_EXT(canSchedulerReset);
    DECLARE_FUNCTION_POINTER_EXT(canSchedulerAddMessage);
    DECLARE_FUNCTION_POINTER_EXT(canSchedulerRemMessage);
    DECLARE_FUNCTION_POINTER_EXT(canSchedulerStartMessage);
    DECLARE_FUNCTION_POINTER_EXT(canSchedulerStopMessage);

    /*****************************************************************************
     * LIN controller specific functions
     ****************************************************************************/
    DECLARE_FUNCTION_POINTER_EXT(linControlOpen);
    DECLARE_FUNCTION_POINTER_EXT(linControlClose);
    DECLARE_FUNCTION_POINTER_EXT(linControlGetCaps);
    DECLARE_FUNCTION_POINTER_EXT(linControlGetStatus);
    DECLARE_FUNCTION_POINTER_EXT(linControlInitialize);
    DECLARE_FUNCTION_POINTER_EXT(linControlReset);
    DECLARE_FUNCTION_POINTER_EXT(linControlStart);
    DECLARE_FUNCTION_POINTER_EXT(linControlWriteMessage);

    /*****************************************************************************
     * LIN message monitor specific functions
     ****************************************************************************/
    DECLARE_FUNCTION_POINTER_EXT(linMonitorOpen);
    DECLARE_FUNCTION_POINTER_EXT(linMonitorClose);
    DECLARE_FUNCTION_POINTER_EXT(linMonitorGetCaps);
    DECLARE_FUNCTION_POINTER_EXT(linMonitorGetStatus);
    DECLARE_FUNCTION_POINTER_EXT(linMonitorInitialize);
    DECLARE_FUNCTION_POINTER_EXT(linMonitorActivate);
    DECLARE_FUNCTION_POINTER_EXT(linMonitorPeekMessage);
    DECLARE_FUNCTION_POINTER_EXT(linMonitorWaitRxEvent);
    DECLARE_FUNCTION_POINTER_EXT(linMonitorReadMessage);


    /*##########################################################################*/
    /*##                                                                      ##*/
    /*##   exported DYNLOAD functions                                         ##*/
    /*##                                                                      ##*/
    /*##########################################################################*/


    /*****************************************************************************
     Function:
      LoadVciNplLib

     Description:
      loads the VciNpl.DLL dynamically

     Arguments:
      none

     Results:
      S_OK on success
      otherwise an error value
    *****************************************************************************/
    HRESULT LoadVciNplLib(void);

    /*****************************************************************************
     Function:
      FreeVciNplLib

     Description:
      free dynamically loaded VciNpl.DLL

     Arguments:
      none

      Results:
      S_OK on success
      otherwise an error value
    *****************************************************************************/
    HRESULT FreeVciNplLib(void);

    /*****************************************************************************
     Function:
      MapGeneralVciFunctions

     Description:
      Loads the function pointer addresses of the "general VCI functions"

     Arguments:
      none

      Results:
      S_OK on success
      otherwise an error value
    *****************************************************************************/
    HRESULT MapGeneralVciFunctions(void);

    /*****************************************************************************
     Function:
      UnmapGeneralVciFunctions

     Description:
      Unloads the function pointer addresses of the "general VCI functions"

     Arguments:
      none

      Results:
      S_OK on success
      otherwise an error value
    *****************************************************************************/
    HRESULT UnmapGeneralVciFunctions(void);

    /*****************************************************************************
     Function:
      MapEventFunctions

     Description:
      Loads the function pointer addresses of the "event specific functions"

     Arguments:
      none

      Results:
      S_OK on success
      otherwise an error value
    *****************************************************************************/
    HRESULT MapEventFunctions(void);

    /*****************************************************************************
     Function:
      UnmapEventFunctions

     Description:
      Unloads the function pointer addresses of the "event specific functions"

     Arguments:
      none

      Results:
      S_OK on success
      otherwise an error value
    *****************************************************************************/
    HRESULT UnmapEventFunctions(void);

    /*****************************************************************************
     Function:
      MapDeviceManagerFunctions

     Description:
      Loads the function pointer addresses of the "device manager specific
      functions"

     Arguments:
      none

      Results:
      S_OK on success
      otherwise an error value
    *****************************************************************************/
    HRESULT MapDeviceManagerFunctions(void);

    /*****************************************************************************
     Function:
      UnmapDeviceManagerFunctions

     Description:
      Unloads the function pointer addresses of the "device manager specific
      functions"

     Arguments:
      none

      Results:
      S_OK on success
      otherwise an error value
    *****************************************************************************/
    HRESULT UnmapDeviceManagerFunctions(void);

    /*****************************************************************************
     Function:
      MapDeviceFunctions

     Description:
      Loads the function pointer addresses of the "device specific functions"

     Arguments:
      none

      Results:
      S_OK on success
      otherwise an error value
    *****************************************************************************/
    HRESULT MapDeviceFunctions(void);

    /*****************************************************************************
     Function:
      UnmapDeviceFunctions

     Description:
      Unloads the function pointer addresses of the "device specific functions"

     Arguments:
      none

      Results:
      S_OK on success
      otherwise an error value
    *****************************************************************************/
    HRESULT UnmapDeviceFunctions(void);

    /*****************************************************************************
     Function:
      MapCANControllerFunctions

     Description:
      Loads the function pointer addresses of the "CAN controller specific
      functions"

     Arguments:
      none

      Results:
      S_OK on success
      otherwise an error value
    *****************************************************************************/
    HRESULT MapCANControllerFunctions(void);

    /*****************************************************************************
     Function:
      UnmapCANControllerFunctions

     Description:
      Unloads the function pointer addresses of the "CAN controller specific
      functions"

     Arguments:
      none

      Results:
      S_OK on success
      otherwise an error value
    *****************************************************************************/
    HRESULT UnmapCANControllerFunctions(void);

    /*****************************************************************************
     Function:
      MapCANMessageChannelFunctions

     Description:
      Loads the function pointer addresses of the "CAN message channel specific
      functions"

     Arguments:
      none

      Results:
      S_OK on success
      otherwise an error value
    *****************************************************************************/
    HRESULT MapCANMessageChannelFunctions(void);

    /*****************************************************************************
     Function:
      UnmapCANMessageChannelFunctions

     Description:
      Unloads the function pointer addresses of the "CAN message channel specific
      functions"

     Arguments:
      none

      Results:
      S_OK on success
      otherwise an error value
    *****************************************************************************/
    HRESULT UnmapCANMessageChannelFunctions(void);

    /*****************************************************************************
     Function:
      MapCANMsgSchedulerFunctions

     Description:
      Loads the function pointer addresses of the "cyclic CAN message scheduler
      specific functions"

     Arguments:
      none

      Results:
      S_OK on success
      otherwise an error value
    *****************************************************************************/
    HRESULT MapCANMsgSchedulerFunctions(void);

    /*****************************************************************************
     Function:
      UnmapCANMessageSchedulerFunctions

     Description:
      Unloads the function pointer addresses of the "cyclic CAN message scheduler
      specific functions"

     Arguments:
      none

      Results:
      S_OK on success
      otherwise an error value
    *****************************************************************************/
    HRESULT UnmapCANMessageSchedulerFunctions(void);

    /*****************************************************************************
     Function:
      MapLINControllerFunctions

     Description:
      Loads the function pointer addresses of the "LIN controller specific
      functions"

     Arguments:
      none

      Results:
      S_OK on success
      otherwise an error value
    *****************************************************************************/
    HRESULT MapLINControllerFunctions(void);

    /*****************************************************************************
     Function:
      UnmapLINControllerFunctions

     Description:
      Unloads the function pointer addresses of the "LIN controller specific
      functions"

     Arguments:
      none

      Results:
      S_OK on success
      otherwise an error value
    *****************************************************************************/
    HRESULT UnmapLINControllerFunctions(void);

    /*****************************************************************************
     Function:
      MapLINMessageMonitorFunctions

     Description:
      Loads the function pointer addresses of the "LIN message monitor specific
      functions"

     Arguments:
      none

      Results:
      S_OK on success
      otherwise an error value
    *****************************************************************************/
    HRESULT MapLINMessageMonitorFunctions(void);

    /*****************************************************************************
     Function:
      UnmapLINMessageMonitorFunctions

     Description:
      Unloads the function pointer addresses of the "LIN message monitor specific
      functions"

     Arguments:
      none

      Results:
      S_OK on success
      otherwise an error value
    *****************************************************************************/
    HRESULT UnmapLINMessageMonitorFunctions(void);

    /* C calling convention */
#ifdef __cplusplus
}
#endif

#include <poppack.h>
#endif //_VCINPLDYNL_H_
