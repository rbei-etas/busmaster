/*****************************************************************************
 HMS Technology Center Ravensburg GmbH
******************************************************************************

 File    : VCINPL.H
 Summary : Native VCI Programming Library.

 Date    : 2005-04-07

 Compiler: MSVC

******************************************************************************
 all rights reserved
*****************************************************************************/

#ifndef _VCINPL_H_
#define _VCINPL_H_

/*##########################################################################*/
/*##                                                                      ##*/
/*##     common declarations                                              ##*/
/*##                                                                      ##*/
/*##########################################################################*/

#include <vcisdk.h>

#if (CAN_ERROR_OTHER != 7)
//
// Redefinition of CAN_ERROR_OTHER for compatibility with VCI version 3.
//
#undef CAN_ERROR_OTHER
#define CAN_ERROR_OTHER 7
//
// Undefine also CAN_ERROR_ codes not available before VCI version 4.
// For backward compatibility VCINPL will translate newer error codes
// to CAN_ERROR_OTHER.
//
#undef CAN_ERROR_FDB
#undef CAN_ERROR_DLC
#endif

/*##########################################################################*/
/*##                                                                      ##*/
/*##   exported API functions                                             ##*/
/*##                                                                      ##*/
/*##########################################################################*/

/*****************************************************************************
 * initialization
 ****************************************************************************/

EXTERN_C HRESULT VCIAPI
  vciInitialize( void );

typedef HRESULT
  (VCIAPI *PF_vciInitialize)( void );


/*****************************************************************************
 * version information
 ****************************************************************************/

EXTERN_C HRESULT VCIAPI
  vciGetVersion( OUT PUINT32 pdwMajorVersion,
                 OUT PUINT32 pdwMinorVersion );

typedef HRESULT
  (VCIAPI *PF_vciGetVersion)( OUT PUINT32 pdwMajorVersion,
                              OUT PUINT32 pdwMinorVersion );

EXTERN_C HRESULT VCIAPI
  vciGetVersionEx( OUT PUINT32 pdwMajorVersion,
                   OUT PUINT32 pdwMinorVersion,
                   OUT PUINT32 pdwRevNumber,
                   OUT PUINT32 pdwBuildNumber );

typedef HRESULT
  (VCIAPI *PF_vciGetVersionEx)( OUT PUINT32 pdwMajorVersion,
                                OUT PUINT32 pdwMinorVersion,
                                OUT PUINT32 pdwRevNumber,
                                OUT PUINT32 pdwBuildNumber );


/*****************************************************************************
 * error handling
 ****************************************************************************/

EXTERN_C void VCIAPI
  vciFormatErrorA( IN  HRESULT hrError,
                   OUT PCHAR   pszText,
                   IN  UINT32  dwLength );

EXTERN_C void VCIAPI
  vciFormatErrorW( IN  HRESULT hrError,
                   OUT PWCHAR  pszText,
                   IN  UINT32  dwLength );

typedef void
  (VCIAPI *PF_vciFormatErrorA)(IN  HRESULT hrError,
                               OUT PCHAR   pszText,
                               IN  UINT32  dwLength );

typedef void
  (VCIAPI *PF_vciFormatErrorW)(IN  HRESULT hrError,
                               OUT PWCHAR  pszText,
                               IN  UINT32  dwLength );

#ifdef UNICODE
#define vciFormatError    vciFormatErrorW
#define PF_vciFormatError PF_vciFormatErrorW
#else
#define vciFormatError    vciFormatErrorA
#define PF_vciFormatError PF_vciFormatErrorA
#endif //UNICODE

EXTERN_C void VCIAPI
  vciDisplayErrorA( IN HWND    hwndParent,
                    IN PCHAR   pszCaption,
                    IN HRESULT hrError );

EXTERN_C void VCIAPI
  vciDisplayErrorW( IN HWND    hwndParent,
                    IN PWCHAR  pszCaption,
                    IN HRESULT hrError );

typedef void
  (VCIAPI *PF_vciDisplayErrorA)( IN HWND    hwndParent,
                                 IN PCHAR   pszCaption,
                                 IN HRESULT hrError );

typedef void
  (VCIAPI *PF_vciDisplayErrorW)( IN HWND    hwndParent,
                                 IN PWCHAR  pszCaption,
                                 IN HRESULT hrError );

#ifdef UNICODE
#define vciDisplayError    vciDisplayErrorW
#define PF_vciDisplayError PF_vciDisplayErrorW
#else
#define vciDisplayError    vciDisplayErrorA
#define PF_vciDisplayError PF_vciFormatErrorA
#endif //UNICODE


/*****************************************************************************
 * utility functions
 ****************************************************************************/

EXTERN_C HRESULT VCIAPI
  vciCreateLuid( OUT PVCIID pVciid );

typedef HRESULT
  (VCIAPI *PF_vciCreateLuid)( OUT PVCIID pVciid );

EXTERN_C HRESULT VCIAPI
  vciLuidToCharA( IN  REFVCIID rVciid,
                  OUT PCHAR    pszLuid,
                  IN  LONG     cbSize );

EXTERN_C HRESULT VCIAPI
  vciLuidToCharW( IN  REFVCIID rVciid,
                  OUT PWCHAR   pszLuid,
                  IN  LONG     cbSize );

typedef HRESULT
  (VCIAPI *PF_vciLuidToCharA)( IN  REFVCIID rVciid,
                               OUT PCHAR    pszLuid,
                               IN  LONG     cbSize );

typedef HRESULT
  (VCIAPI *PF_vciLuidToCharW)( IN  REFVCIID rVciid,
                               OUT PWCHAR   pszLuid,
                               IN  LONG     cbSize );

#ifdef UNICODE
#define vciLuidToChar    vciLuidToCharW
#define PF_vciLuidToChar PF_vciLuidToCharW
#else
#define vciLuidToChar    vciLuidToCharA
#define PF_vciLuidToChar PF_vciLuidToCharA
#endif //UNICODE

EXTERN_C HRESULT VCIAPI
  vciCharToLuidA( IN  PCHAR  pszLuid,
                  OUT PVCIID pVciid );

EXTERN_C HRESULT VCIAPI
  vciCharToLuidW( IN  PWCHAR pszLuid,
                  OUT PVCIID pVciid );

typedef HRESULT
  (VCIAPI *PF_vciCharToLuidA)( IN  PCHAR  pszLuid,
                               OUT PVCIID pVciid );

typedef HRESULT
  (VCIAPI *PF_vciCharToLuidW)( IN  PWCHAR pszLuid,
                               OUT PVCIID pVciid );

#ifdef UNICODE
#define vciCharToLuid    vciCharToLuidW
#define PF_vciCharToLuid PF_vciCharToLuidW
#else
#define vciCharToLuid    vciCharToLuidA
#define PF_vciCharToLuid PF_vciCharToLuidA
#endif //UNICODE

EXTERN_C HRESULT VCIAPI
  vciGuidToCharA( IN  REFGUID rGuid,
                  OUT PCHAR   pszGuid,
                  IN  LONG    cbSize );

EXTERN_C HRESULT VCIAPI
  vciGuidToCharW( IN  REFGUID rGuid,
                  OUT PWCHAR  pszGuid,
                  IN  LONG    cbSize );

typedef HRESULT
  (VCIAPI *PF_vciGuidToCharA)( IN  REFGUID rGuid,
                               OUT PCHAR   pszGuid,
                               IN  LONG    cbSize );

typedef HRESULT
  (VCIAPI *PF_vciGuidToCharW)( IN  REFGUID rGuid,
                               OUT PWCHAR  pszGuid,
                               IN  LONG    cbSize );

#ifdef UNICODE
#define vciGuidToChar    vciGuidToCharW
#define PF_vciGuidToChar PF_vciGuidToCharW
#else
#define vciGuidToChar    vciGuidToCharA
#define PF_vciGuidToChar PF_vciGuidToCharA
#endif //UNICODE

EXTERN_C HRESULT VCIAPI
  vciCharToGuidA( IN  PCHAR pszGuid,
                  OUT PGUID pGuid );

EXTERN_C HRESULT VCIAPI
  vciCharToGuidW( IN  PWCHAR pszGuid,
                  OUT PGUID  pGuid );

typedef HRESULT
  (VCIAPI *PF_vciCharToGuidA)( IN  PCHAR pszGuid,
                               OUT PGUID pGuid );

typedef HRESULT
  (VCIAPI *PF_vciCharToGuidW)( IN  PWCHAR pszGuid,
                               OUT PGUID  pGuid );

#ifdef UNICODE
#define vciCharToGuid    vciCharToGuidW
#define PF_vciCharToGuid PF_vciCharToGuidW
#else
#define vciCharToGuid    vciCharToGuidA
#define PF_vciCharToGuid PF_vciCharToGuidA
#endif //UNICODE


/*****************************************************************************
 * event specific functions
 ****************************************************************************/

EXTERN_C HRESULT VCIAPI
  vciEventCreate( IN  BOOL    fManReset,
                  IN  BOOL    fInitState,
                  OUT PHANDLE phEvent );

typedef HRESULT
  (VCIAPI *PF_vciEventCreate)( IN  BOOL    fManReset,
                               IN  BOOL    fInitState,
                               OUT PHANDLE phEvent );

EXTERN_C HRESULT VCIAPI
  vciEventDelete( IN HANDLE hEvent );

typedef HRESULT
  (VCIAPI *PF_vciEventDelete)( IN HANDLE hEvent );

EXTERN_C HRESULT VCIAPI
  vciEventSignal( IN HANDLE hEvent );

typedef HRESULT
  (VCIAPI *PF_vciEventSignal)( IN HANDLE hEvent );

EXTERN_C HRESULT VCIAPI
  vciEventReset( HANDLE hEvent );

typedef HRESULT
  (VCIAPI *PF_vciEventReset)( HANDLE hEvent );

EXTERN_C HRESULT VCIAPI
  vciEventWaitFor( IN HANDLE hEvent,
                   IN UINT32 dwTimeout );

typedef HRESULT
  (VCIAPI *PF_vciEventWaitFor)( IN HANDLE hEvent,
                                IN UINT32 dwTimeout );


/*****************************************************************************
 * device manager specific functions
 ****************************************************************************/

EXTERN_C HRESULT VCIAPI
  vciEnumDeviceOpen( OUT PHANDLE hEnum );

typedef HRESULT
  (VCIAPI *PF_vciEnumDeviceOpen)( OUT PHANDLE hEnum );

EXTERN_C HRESULT VCIAPI
  vciEnumDeviceClose( IN HANDLE hEnum );

typedef HRESULT
  (VCIAPI *PF_vciEnumDeviceClose)( IN HANDLE hEnum );

EXTERN_C HRESULT VCIAPI
  vciEnumDeviceNext( IN  HANDLE         hEnum,
                     OUT PVCIDEVICEINFO pInfo );

typedef HRESULT
  (VCIAPI *PF_vciEnumDeviceNext)( IN  HANDLE         hEnum,
                                  OUT PVCIDEVICEINFO pInfo );

EXTERN_C HRESULT VCIAPI
  vciEnumDeviceReset( IN HANDLE hEnum );

typedef HRESULT
  (VCIAPI *PF_vciEnumDeviceReset)( IN HANDLE hEnum );

EXTERN_C HRESULT VCIAPI
  vciEnumDeviceWaitEvent( IN HANDLE hEnum,
                          IN UINT32 dwTimeout );

typedef HRESULT
  (VCIAPI *PF_vciEnumDeviceWaitEvent)( IN HANDLE hEnum,
                                       IN UINT32 dwTimeout );

EXTERN_C HRESULT VCIAPI
  vciFindDeviceByHwid( IN  REFGUID rHwid,
                       OUT PVCIID  pVciid );

typedef HRESULT
  (VCIAPI *PF_vciFindDeviceByHwid)( IN  REFGUID rHwid,
                                    OUT PVCIID  pVciid );

EXTERN_C HRESULT VCIAPI
  vciFindDeviceByClass( IN  REFGUID rClass,
                        IN  UINT32  dwInst,
                        OUT PVCIID  pVciid );

typedef HRESULT
  (VCIAPI *PF_vciFindDeviceByClass)( IN  REFGUID rClass,
                                     IN  UINT32  dwInst,
                                     OUT PVCIID  pVciid );

EXTERN_C HRESULT VCIAPI
  vciSelectDeviceDlg( IN  HWND   hwndParent,
                      OUT PVCIID pVciid );

typedef HRESULT
  (VCIAPI *PF_vciSelectDeviceDlg)( IN  HWND   hwndParent,
                                   OUT PVCIID pVciid );


/*****************************************************************************
 * device specific functions
 ****************************************************************************/

EXTERN_C HRESULT VCIAPI
  vciDeviceOpen( IN  REFVCIID rVciid,
                 OUT PHANDLE  phDevice );

typedef HRESULT
  (VCIAPI *PF_vciDeviceOpen)( IN  REFVCIID rVciid,
                              OUT PHANDLE  phDevice );

EXTERN_C HRESULT VCIAPI
  vciDeviceOpenDlg( IN  HWND    hwndParent,
                    OUT PHANDLE phDevice );

typedef HRESULT
  (VCIAPI *PF_vciDeviceOpenDlg)( IN  HWND    hwndParent,
                                 OUT PHANDLE phDevice );

EXTERN_C HRESULT VCIAPI
  vciDeviceClose( IN HANDLE hDevice );

typedef HRESULT
  (VCIAPI *PF_vciDeviceClose)( IN HANDLE hDevice );

EXTERN_C HRESULT VCIAPI
  vciDeviceGetInfo( IN  HANDLE         hDevice,
                    OUT PVCIDEVICEINFO pInfo );

typedef HRESULT
  (VCIAPI *PF_vciDeviceGetInfo)( IN  HANDLE         hDevice,
                                 OUT PVCIDEVICEINFO pInfo );

EXTERN_C HRESULT VCIAPI
  vciDeviceGetCaps( IN  HANDLE         hDevice,
                    OUT PVCIDEVICECAPS pCaps );

typedef HRESULT
  (VCIAPI *PF_vciDeviceGetCaps)( IN  HANDLE         hDevice,
                                 OUT PVCIDEVICECAPS pCaps );


/*****************************************************************************
 * CAN controller specific functions
 ****************************************************************************/

EXTERN_C HRESULT VCIAPI
  canControlOpen( IN  HANDLE  hDevice,
                  IN  UINT32  dwCanNo,
                  OUT PHANDLE phCanCtl );

typedef HRESULT
  (VCIAPI *PF_canControlOpen)( IN  HANDLE  hDevice,
                               IN  UINT32  dwCanNo,
                               OUT PHANDLE phCanCtl );

EXTERN_C HRESULT VCIAPI
  canControlClose( IN HANDLE hCanCtl );

typedef HRESULT
  (VCIAPI *PF_canControlClose)( IN HANDLE hCanCtl );

EXTERN_C HRESULT VCIAPI
  canControlGetCaps( IN  HANDLE           hCanCtl,
                     OUT PCANCAPABILITIES pCanCaps );

typedef HRESULT
  (VCIAPI *PF_canControlGetCaps)( IN  HANDLE           hCanCtl,
                                  OUT PCANCAPABILITIES pCanCaps );

EXTERN_C HRESULT VCIAPI
  canControlGetStatus( IN  HANDLE         hCanCtl,
                       OUT PCANLINESTATUS pStatus );

typedef HRESULT
  (VCIAPI *PF_canControlGetStatus)( IN  HANDLE         hCanCtl,
                                    OUT PCANLINESTATUS pStatus );

EXTERN_C HRESULT VCIAPI
  canControlDetectBitrate( IN  HANDLE  hCanCtl,
                           IN  UINT16  wMsTimeout,
                           IN  UINT32  dwCount,
                           IN  PUINT8  pabBtr0,
                           IN  PUINT8  pabBtr1,
                           OUT PINT32  plIndex );

typedef HRESULT
  (VCIAPI *PF_canControlDetectBitrate)( IN  HANDLE  hCanCtl,
                                        IN  UINT16  wMsTimeout,
                                        IN  UINT32  dwCount,
                                        IN  PUINT8  pabBtr0,
                                        IN  PUINT8  pabBtr1,
                                        OUT PINT32  plIndex );

EXTERN_C HRESULT VCIAPI
  canControlInitialize( IN HANDLE hCanCtl,
                        IN UINT8  bMode,
                        IN UINT8  bBtr0,
                        IN UINT8  bBtr1 );

typedef HRESULT
  (VCIAPI *PF_canControlInitialize)( IN HANDLE hCanCtl,
                                     IN UINT8  bMode,
                                     IN UINT8  bBtr0,
                                     IN UINT8  bBtr1 );

EXTERN_C HRESULT VCIAPI
  canControlReset( IN HANDLE hCanCtl );

typedef HRESULT
  (VCIAPI *PF_canControlReset)( IN HANDLE hCanCtl );

EXTERN_C HRESULT VCIAPI
  canControlStart( IN HANDLE hCanCtl,
                   IN BOOL   fStart );

typedef HRESULT
  (VCIAPI *PF_canControlStart)( IN HANDLE hCanCtl,
                                IN BOOL   fStart );

EXTERN_C HRESULT VCIAPI
  canControlSetAccFilter( IN HANDLE hCanCtl,
                          IN BOOL   fExtend,
                          IN UINT32 dwCode,
                          IN UINT32 dwMask );

typedef HRESULT
  (VCIAPI *PF_canControlSetAccFilter)( IN HANDLE hCanCtl,
                                       IN BOOL   fExtend,
                                       IN UINT32 dwCode,
                                       IN UINT32 dwMask );

EXTERN_C HRESULT VCIAPI
  canControlAddFilterIds( IN HANDLE hCanCtl,
                          IN BOOL   fExtend,
                          IN UINT32 dwCode,
                          IN UINT32 dwMask );

typedef HRESULT
  (VCIAPI *PF_canControlAddFilterIds)( IN HANDLE hCanCtl,
                                       IN BOOL   fExtend,
                                       IN UINT32 dwCode,
                                       IN UINT32 dwMask );

EXTERN_C HRESULT VCIAPI
  canControlRemFilterIds( IN HANDLE hCanCtl,
                          IN BOOL   fExtend,
                          IN UINT32 dwCode,
                          IN UINT32 dwMask );

typedef HRESULT
  (VCIAPI *PF_canControlRemFilterIds)( IN HANDLE hCanCtl,
                                       IN BOOL   fExtend,
                                       IN UINT32 dwCode,
                                       IN UINT32 dwMask );


/*****************************************************************************
 * CAN message channel specific functions
 ****************************************************************************/

EXTERN_C HRESULT VCIAPI
  canChannelOpen( IN  HANDLE  hDevice,
                  IN  UINT32  dwCanNo,
                  IN  BOOL    fExclusive,
                  OUT PHANDLE phCanChn );

typedef HRESULT
  (VCIAPI *PF_canChannelOpen)( IN  HANDLE  hDevice,
                               IN  UINT32  dwCanNo,
                               IN  BOOL    fExclusive,
                               OUT PHANDLE phCanChn );

EXTERN_C HRESULT VCIAPI
  canChannelClose( IN HANDLE hCanChn );

typedef HRESULT
  (VCIAPI *PF_canChannelClose)( IN HANDLE hCanChn );

EXTERN_C HRESULT VCIAPI
  canChannelGetCaps( IN  HANDLE           hCanChn,
                     OUT PCANCAPABILITIES pCanCaps );

typedef HRESULT
  (VCIAPI *PF_canChannelGetCaps)( IN  HANDLE           hCanChn,
                                  OUT PCANCAPABILITIES pCanCaps );

EXTERN_C HRESULT VCIAPI
  canChannelGetStatus( IN  HANDLE         hCanChn,
                       OUT PCANCHANSTATUS pStatus );

typedef HRESULT
  (VCIAPI *PF_canChannelGetStatus)( IN  HANDLE         hCanChn,
                                    OUT PCANCHANSTATUS pStatus );

EXTERN_C HRESULT VCIAPI
  canChannelInitialize( IN HANDLE hCanChn,
                        IN UINT16 wRxFifoSize,
                        IN UINT16 wRxThreshold,
                        IN UINT16 wTxFifoSize,
                        IN UINT16 wTxThreshold );

typedef HRESULT
  (VCIAPI *PF_canChannelInitialize)( IN HANDLE hCanChn,
                                     IN UINT16 wRxFifoSize,
                                     IN UINT16 wRxThreshold,
                                     IN UINT16 wTxFifoSize,
                                     IN UINT16 wTxThreshold );

EXTERN_C HRESULT VCIAPI
  canChannelActivate( IN HANDLE hCanChn,
                      IN BOOL   fEnable );

typedef HRESULT
  (VCIAPI *PF_canChannelActivate)( IN HANDLE hCanChn,
                                   IN BOOL   fEnable );

EXTERN_C HRESULT VCIAPI
  canChannelPeekMessage( IN  HANDLE  hCanChn,
                         OUT PCANMSG pCanMsg );

typedef HRESULT
  (VCIAPI *PF_canChannelPeekMessage)( IN  HANDLE  hCanChn,
                                      OUT PCANMSG pCanMsg );

EXTERN_C HRESULT VCIAPI
  canChannelPeekMsgMult( IN  HANDLE  hCanChn,
                         OUT CANMSG  aCanMsg[],
                         IN  UINT32  dwCount,
                         OUT PUINT32 pdwDone );

typedef HRESULT
  (VCIAPI *PF_canChannelPeekMsgMult)( IN  HANDLE  hCanChn,
                                      OUT CANMSG  aCanMsg[],
                                      IN  UINT32  dwCount,
                                      OUT PUINT32 pdwDone );

EXTERN_C HRESULT VCIAPI
  canChannelPostMessage( IN HANDLE  hCanChn,
                         IN PCANMSG pCanMsg );

typedef HRESULT
  (VCIAPI *PF_canChannelPostMessage)( IN HANDLE  hCanChn,
                                      IN PCANMSG pCanMsg );

EXTERN_C HRESULT VCIAPI
  canChannelPostMsgMult( IN  HANDLE  hCanChn,
                         IN  CANMSG  aCanMsg[],
                         IN  UINT32  dwCount,
                         OUT PUINT32 pdwDone );

typedef HRESULT
  (VCIAPI *PF_canChannelPostMsgMult)( IN  HANDLE  hCanChn,
                                      IN  CANMSG  aCanMsg[],
                                      IN  UINT32  dwCount,
                                      OUT PUINT32 pdwDone );

EXTERN_C HRESULT VCIAPI
  canChannelWaitRxEvent( IN HANDLE hCanChn,
                         IN UINT32 dwTimeout );

typedef HRESULT
  (VCIAPI *PF_canChannelWaitRxEvent)( IN HANDLE hCanChn,
                                      IN UINT32 dwTimeout );

EXTERN_C HRESULT VCIAPI
  canChannelWaitTxEvent( IN HANDLE hCanChn,
                         IN UINT32 dwTimeout );

typedef HRESULT
  (VCIAPI *PF_canChannelWaitTxEvent)( IN HANDLE hCanChn,
                                      IN UINT32 dwTimeout );

EXTERN_C HRESULT VCIAPI
  canChannelReadMessage( IN  HANDLE  hCanChn,
                         IN  UINT32  dwTimeout,
                         OUT PCANMSG pCanMsg );

typedef HRESULT
  (VCIAPI *PF_canChannelReadMessage)( IN  HANDLE  hCanChn,
                                      IN  UINT32  dwTimeout,
                                      OUT PCANMSG pCanMsg );

EXTERN_C HRESULT VCIAPI
  canChannelReadMsgMult( IN  HANDLE  hCanChn,
                         IN  UINT32  dwTimeout,
                         OUT CANMSG  aCanMsg[],
                         IN  UINT32  dwCount,
                         OUT PUINT32 pdwDone );

typedef HRESULT
  (VCIAPI *PF_canChannelReadMsgMult)( IN  HANDLE  hCanChn,
                                      IN  UINT32  dwTimeout,
                                      OUT CANMSG  aCanMsg[],
                                      IN  UINT32  dwCount,
                                      OUT PUINT32 pdwDone );

EXTERN_C HRESULT VCIAPI
  canChannelSendMessage( IN HANDLE  hCanChn,
                         IN UINT32  dwTimeout,
                         IN PCANMSG pCanMsg );

typedef HRESULT
  (VCIAPI *PF_canChannelSendMessage)( IN HANDLE  hCanChn,
                                      IN UINT32  dwTimeout,
                                      IN PCANMSG pCanMsg );

EXTERN_C HRESULT VCIAPI
  canChannelSendMsgMult( IN  HANDLE  hCanChn,
                         IN  UINT32  dwTimeout,
                         IN  CANMSG  aCanMsg[],
                         IN  UINT32  dwCount,
                         OUT PUINT32 pdwDone );

typedef HRESULT
  (VCIAPI *PF_canChannelSendMsgMult)( IN  HANDLE  hCanChn,
                                      IN  UINT32  dwTimeout,
                                      IN  CANMSG  aCanMsg[],
                                      IN  UINT32  dwCount,
                                      OUT PUINT32 pdwDone );

//
// legacy functions (do not use for new applications)
//
#ifndef _VCINPL_NO_LEGACY_

EXTERN_C HRESULT VCIAPI
  canChannelPeekMultipleMessages( IN     HANDLE  hCanChn,
                                  IN OUT PUINT32 pdwCount,
                                  OUT    CANMSG  aCanMsg[] );
  // use canChannelPeekMsgMult instead

EXTERN_C HRESULT VCIAPI
  canChannelPostMultipleMessages( IN     HANDLE  hCanChn,
                                  IN OUT PUINT32 pdwCount,
                                  IN     CANMSG  aCanMsg[] );
  // use canChannelPostMsgMult instead

EXTERN_C HRESULT VCIAPI
  canChannelReadMultipleMessages(IN     HANDLE  hCanChn,
                                 IN     UINT32  dwTimeout,
                                 IN OUT PUINT32 pdwCount, 
                                 OUT    CANMSG  aCanMsg[] );
  // use canChannelReadMsgMult instead

EXTERN_C HRESULT VCIAPI
  canChannelSendMultipleMessages( IN     HANDLE  hCanChn,
                                  IN     UINT32  dwTimeout,
                                  IN OUT PUINT32 pdwCount,
                                  IN     CANMSG  aCanMsg[] );
  // use canChannelSendMsgMult instead

#endif //_VCINPL_NO_LEGACY_



/*****************************************************************************
 * CAN message scheduler specific functions
 ****************************************************************************/

EXTERN_C HRESULT VCIAPI
  canSchedulerOpen( IN  HANDLE  hDevice,
                    IN  UINT32  dwCanNo,
                    OUT PHANDLE phCanShd );

typedef HRESULT
  (VCIAPI *PF_canSchedulerOpen)( IN  HANDLE  hDevice,
                                 IN  UINT32  dwCanNo,
                                 OUT PHANDLE phCanShd );

EXTERN_C HRESULT VCIAPI
  canSchedulerClose( IN HANDLE hCanShd );

typedef HRESULT
  (VCIAPI *PF_canSchedulerClose)( IN HANDLE hCanShd );

EXTERN_C HRESULT VCIAPI
  canSchedulerGetCaps( IN  HANDLE           hCanShd,
                       OUT PCANCAPABILITIES pCanCaps );

typedef HRESULT
  (VCIAPI *PF_canSchedulerGetCaps)( IN  HANDLE           hCanShd,
                                    OUT PCANCAPABILITIES pCanCaps );

EXTERN_C HRESULT VCIAPI
  canSchedulerGetStatus( IN  HANDLE              hCanShd,
                         OUT PCANSCHEDULERSTATUS pStatus );

typedef HRESULT
  (VCIAPI *PF_canSchedulerGetStatus)( IN  HANDLE              hCanShd,
                                      OUT PCANSCHEDULERSTATUS pStatus );

EXTERN_C HRESULT VCIAPI
  canSchedulerActivate( IN HANDLE hCanShd,
                        IN BOOL   fEnable );

typedef HRESULT
  (VCIAPI *PF_canSchedulerActivate)( IN HANDLE hCanShd,
                                     IN BOOL   fEnable );

EXTERN_C HRESULT VCIAPI
  canSchedulerReset( IN HANDLE hCanShd );

typedef HRESULT
  (VCIAPI *PF_canSchedulerReset)( IN HANDLE hCanShd );

EXTERN_C HRESULT VCIAPI
  canSchedulerAddMessage( IN  HANDLE          hCanShd,
                          IN  PCANCYCLICTXMSG pMessage,
                          OUT PUINT32         pdwIndex );

typedef HRESULT
  (VCIAPI *PF_canSchedulerAddMessage)( IN  HANDLE          hCanShd,
                                       IN  PCANCYCLICTXMSG pMessage,
                                       OUT PUINT32         pdwIndex );

EXTERN_C HRESULT VCIAPI
  canSchedulerRemMessage( IN HANDLE hCanShd,
                          IN UINT32 dwIndex );

typedef HRESULT
  (VCIAPI *PF_canSchedulerRemMessage)( IN HANDLE hCanShd,
                                       IN UINT32 dwIndex );

EXTERN_C HRESULT VCIAPI
  canSchedulerStartMessage( IN HANDLE hCanShd,
                            IN UINT32 dwIndex,
                            IN UINT16 wRepeat );

typedef HRESULT
  (VCIAPI *PF_canSchedulerStartMessage)( IN HANDLE hCanShd,
                                         IN UINT32 dwIndex,
                                         IN UINT16 wRepeat );

EXTERN_C HRESULT VCIAPI
  canSchedulerStopMessage( IN HANDLE hCanShd,
                           IN UINT32 dwIndex );

typedef HRESULT
  (VCIAPI *PF_canSchedulerStopMessage)( IN HANDLE hCanShd,
                                        IN UINT32 dwIndex );



/*****************************************************************************
 * LIN controller specific functions
 ****************************************************************************/

EXTERN_C HRESULT VCIAPI
  linControlOpen( IN  HANDLE  hDevice,
                  IN  UINT32  dwLinNo,
                  OUT PHANDLE phLinCtl );

typedef HRESULT
  (VCIAPI *PF_linControlOpen)( IN  HANDLE  hDevice,
                               IN  UINT32  dwLinNo,
                               OUT PHANDLE phLinCtl );

EXTERN_C HRESULT VCIAPI
  linControlClose( IN HANDLE hLinCtl );

typedef HRESULT
  (VCIAPI *PF_linControlClose)( IN HANDLE hLinCtl );

EXTERN_C HRESULT VCIAPI
  linControlGetCaps( IN  HANDLE           hLinCtl,
                     OUT PLINCAPABILITIES pLinCaps );

typedef HRESULT
  (VCIAPI *PF_linControlGetCaps)( IN  HANDLE           hLinCtl,
                                  OUT PLINCAPABILITIES pLinCaps );

EXTERN_C HRESULT VCIAPI
  linControlGetStatus( IN  HANDLE         hLinCtl,
                       OUT PLINLINESTATUS pStatus );

typedef HRESULT
  (VCIAPI *PF_linControlGetStatus)( IN  HANDLE         hLinCtl,
                                    OUT PLINLINESTATUS pStatus );

EXTERN_C HRESULT VCIAPI
  linControlInitialize( IN HANDLE hLinCtl,
                        IN UINT8  bMode,
                        IN UINT16 wBitrate );

typedef HRESULT
  (VCIAPI *PF_linControlInitialize)( IN HANDLE hLinCtl,
                                     IN UINT8  bMode,
                                     IN UINT16 wBitrate );

EXTERN_C HRESULT VCIAPI
  linControlReset( IN HANDLE hLinCtl );

typedef HRESULT
  (VCIAPI *PF_linControlReset)( IN HANDLE hLinCtl );

EXTERN_C HRESULT VCIAPI
  linControlStart( IN HANDLE hLinCtl,
                   IN BOOL   fStart );

typedef HRESULT
  (VCIAPI *PF_linControlStart)( IN HANDLE hLinCtl,
                                IN BOOL   fStart );

EXTERN_C HRESULT VCIAPI
  linControlWriteMessage( IN HANDLE  hLinCtl,
                          IN BOOL    fSend,
                          IN PLINMSG pLinMsg );

typedef HRESULT
  (VCIAPI *PF_linControlWriteMessage)( IN HANDLE  hLinCtl,
                                       IN BOOL    fSend,
                                       IN PLINMSG pLinMsg );


/*****************************************************************************
 * LIN message monitor specific functions
 ****************************************************************************/

EXTERN_C HRESULT VCIAPI
  linMonitorOpen( IN  HANDLE  hDevice,
                  IN  UINT32  dwLinNo,
                  IN  BOOL    fExclusive,
                  OUT PHANDLE phLinMon );

typedef HRESULT
  (VCIAPI *PF_linMonitorOpen)( IN  HANDLE  hDevice,
                               IN  UINT32  dwLinNo,
                               IN  BOOL    fExclusive,
                               OUT PHANDLE phLinMon );

EXTERN_C HRESULT VCIAPI
  linMonitorClose( IN HANDLE hLinMon );

typedef HRESULT
  (VCIAPI *PF_linMonitorClose)( IN HANDLE hLinMon );

EXTERN_C HRESULT VCIAPI
  linMonitorGetCaps( IN  HANDLE           hLinMon,
                     OUT PLINCAPABILITIES pLinCaps );

typedef HRESULT
  (VCIAPI *PF_linMonitorGetCaps)( IN  HANDLE           hLinMon,
                                  OUT PLINCAPABILITIES pLinCaps );

EXTERN_C HRESULT VCIAPI
  linMonitorGetStatus( IN  HANDLE            hLinMon,
                       OUT PLINMONITORSTATUS pStatus );

typedef HRESULT
  (VCIAPI *PF_linMonitorGetStatus)( IN  HANDLE            hLinMon,
                                    OUT PLINMONITORSTATUS pStatus );

EXTERN_C HRESULT VCIAPI
  linMonitorInitialize( IN HANDLE hLinMon,
                        IN UINT16 wFifoSize,
                        IN UINT16 wThreshold );

typedef HRESULT
  (VCIAPI *PF_linMonitorInitialize)( IN HANDLE hLinMon,
                                     IN UINT16 wFifoSize,
                                     IN UINT16 wThreshold );

EXTERN_C HRESULT VCIAPI
  linMonitorActivate( IN HANDLE hLinMon,
                      IN BOOL   fEnable );

typedef HRESULT
  (VCIAPI *PF_linMonitorActivate)( IN HANDLE hLinMon,
                                   IN BOOL   fEnable );

EXTERN_C HRESULT VCIAPI
  linMonitorPeekMessage( IN  HANDLE  hLinMon,
                         OUT PLINMSG pLinMsg );

typedef HRESULT
  (VCIAPI *PF_linMonitorPeekMessage)( IN  HANDLE  hLinMon,
                                      OUT PLINMSG pLinMsg );

EXTERN_C HRESULT VCIAPI
  linMonitorPeekMsgMult( IN  HANDLE  hLinMon,
                         OUT LINMSG  aLinMsg[],
                         IN  UINT32  dwCount,
                         OUT PUINT32 pdwDone );

typedef HRESULT
  (VCIAPI *PF_linMonitorPeekMsgMult)( IN  HANDLE  hLinMon,
                                      OUT LINMSG  aLinMsg[],
                                      IN  UINT32  dwCount,
                                      OUT PUINT32 pdwDone );

EXTERN_C HRESULT VCIAPI
  linMonitorWaitRxEvent( IN HANDLE hLinMon,
                         IN UINT32 dwTimeout );

typedef HRESULT
  (VCIAPI *PF_linMonitorWaitRxEvent)( IN HANDLE hLinMon,
                                      IN UINT32 dwTimeout );

EXTERN_C HRESULT VCIAPI
  linMonitorReadMessage( IN  HANDLE  hLinMon,
                         IN  UINT32  dwTimeout,
                         OUT PLINMSG pLinMsg );

typedef HRESULT
  (VCIAPI *PF_linMonitorReadMessage)( IN  HANDLE  hLinMon,
                                      IN  UINT32  dwTimeout,
                                      OUT PLINMSG pLinMsg );

EXTERN_C HRESULT VCIAPI
  linMonitorReadMsgMult( IN  HANDLE  hLinMon,
                         IN  UINT32  dwTimeout,
                         OUT LINMSG  aLinMsg[],
                         IN  UINT32  dwCount,
                         OUT PUINT32 pdwDone );

typedef HRESULT
  (VCIAPI *PF_linMonitorReadMsgMult)( IN  HANDLE  hLinMon,
                                      IN  UINT32  dwTimeout,
                                      OUT LINMSG  aLinMsg[],
                                      IN  UINT32  dwCount,
                                      OUT PUINT32 pdwDone );

#endif //_VCINPL_H_
