/*******************************************************************************
  Project       :  BUSMASTER
  FileName      :  Wrapper_LIN.h
  Description   :
  Author(s)     :  Prathiba Bharathi
  Date Created  :  12/12/2013
*******************************************************************************/
#pragma once

#include <windows.h>
#include <stdio.h>
#include "Common.h"


//Structures
typedef struct sTLIN_MSG
{
    unsigned char m_ucMsgTyp;        // Protected Identifier
    unsigned char m_ucChksumTyp;    // Checksum Type (0 - Classical / 1 - Enhanced)
    unsigned char m_ucDataLen;      // Data length (0..8)
    unsigned char m_ucMsgID;        // Protected Identifier
    // Channel Number
    union
    {
        UCHAR m_aucData[8];  // Byte Data
        USHORT m_auwData[4];  // Word Data
        UINT m_aulData[2];  // Long Data
    } m_sWhichBit;         // Data access member
    unsigned long m_ulTimeStamp;
    unsigned char m_ucChannel;
    unsigned char m_ucCRC;       // Checksum

} STLIN_MSG;

//This structure holds the error and the channel number
struct SEVENT_CHECKSUM_LIN
{
    unsigned char m_ucId;
    unsigned char m_ucCrc;
    unsigned char m_ucChannel;
    unsigned int  m_ulTime;
} ;

struct SEVENT_RECEIVE_LIN
{
    unsigned int  m_ulTime;
};

struct SEVENT_SLAVE_NORESP_LIN
{
    unsigned char m_ucId;
    unsigned char m_ucChannel;
    unsigned int  m_ulTime;

};

typedef struct SEVENT_SYNC_LIN
{
    unsigned char m_ucChannel;
    unsigned int  m_ulTime;

};


//typedef
typedef UINT (*APPFUNCPROC) (void*,HMODULE);
typedef UINT (*LOGENABLE) (BOOL);
typedef UINT (*WRITETOLOGFILE) (char*);
typedef UINT (*WRITETOTRACE) (char*);
typedef void (*RESETCONTROLLER) (BOOL);
typedef UINT (*CONTROLLERMODE) (BOOL);
typedef UINT (*GOONLINEOFFLINE) (BOOL,HMODULE) ;
typedef UINT (*DISCONNECT) (BOOL) ;
typedef UINT (*STARTSTOPTIMER) (char*,UINT,BOOL,HMODULE) ;
typedef UINT (*SETTIMERVAL) (char*,UINT,HMODULE);
typedef UINT (*ENABLEDISABLEMSGHANDLERS)(BOOL,HMODULE);
typedef UINT (*ENABLEDISABLEKEYHANDLERS)(BOOL,HMODULE);
typedef UINT (*DLLMSGTXONOFF)(BOOL,HMODULE);
typedef UINT (*ENABLEDISABLEERRORHANDLERS)(BOOL,HMODULE);
typedef HMODULE (*DLLGETNODEHANDLER)(char*);

//typedef void (*GETMESSAGENAME)( DWORD id, char buffer[], DWORD size);
//typedef void (*SETGETMESSAGENAME)(GETMESSAGENAME);

typedef int (*KEYPRESSED)();
typedef void (*SETKEYPRESSED)(KEYPRESSED);

typedef int (*GETMESSAGENAME)(DWORD, DWORD , char* ,DWORD );
typedef void (*DLLGETMESSAGENAME)(GETMESSAGENAME);

typedef int (*TIMENOW)();
typedef void (*DLLTIMENOW)(TIMENOW);

typedef DWORD (*GETFIRSTCANDBNAME)(char* , DWORD );
typedef void (*DLLGETFIRSTCANDBNAME)(GETFIRSTCANDBNAME);



// Please insert any new wrapper prototype here
// Prototypes Start
GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT SendMsg(STLIN_MSG);
GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT EnableLogging() ;
GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT DisableLogging() ;
GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT WriteToLogFile(char*) ;
GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT Trace(char*,...) ;
GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT Connect() ;
GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT Disconnect() ;
GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT StartTimer(char*,UINT) ;
GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT StopTimer(char*) ;
GCC_EXTERN /*extern "C"*/ BOOL GCC_EXPORT SetTimerVal(char*,UINT);
GCC_EXTERN /*extern "C"*/ BOOL GCC_EXPORT EnableMsgHandlers(BOOL);
GCC_EXTERN /*extern "C"*/ BOOL GCC_EXPORT EnableErrorHandlers(BOOL);
GCC_EXTERN /*extern "C"*/ BOOL GCC_EXPORT EnableKeyHandlers(BOOL);
GCC_EXTERN /*extern "C"*/ BOOL GCC_EXPORT bGetProgramVersion(int* pnMajorVersion, int* pnMinorVersion,HMODULE hModuleHandle );
GCC_EXTERN /*extern "C"*/ HMODULE GCC_EXPORT GetDllHandle(char*);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetSendMsgProcAddress(APPFUNCPROC pAppFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetEnableLoggingProcAddress(LOGENABLE pEnableLoggingFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetDisableLoggingProcAddress(LOGENABLE pDisableLoggingFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetWriteToLogFileProcAddress(WRITETOLOGFILE pLogFileFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetTraceProcAddress(WRITETOTRACE pTraceFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetResetControllerProcAddress(RESETCONTROLLER pResetFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetSetActiveModeProcAddress(CONTROLLERMODE pActiveModeFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetSetPassiveModeProcAddress(CONTROLLERMODE pPassiveModeFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetGoOfflineProcAddress(GOONLINEOFFLINE pGoOffline);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetGoOnlineProcAddress(GOONLINEOFFLINE pGoOnline);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetDisconnectProcAddress(DISCONNECT pDisconnect);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetConnectProcAddress(DISCONNECT pConnect);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetStopTimerProcAddress(STARTSTOPTIMER pStopTimer);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetStartTimerProcAddress(STARTSTOPTIMER pStartTimer);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetSetTimerValProcAddress(SETTIMERVAL pSetTimerVal);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetEnableMsgHandlersProcAddress(ENABLEDISABLEMSGHANDLERS pMsgHandlers);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetEnableKeyHandlersProcAddress(ENABLEDISABLEKEYHANDLERS pKeyHandlers);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetEnableErrorHandlersProcAddress(ENABLEDISABLEERRORHANDLERS pErrorHandlers);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetEnableDisableMsgTxProcAddress(DLLMSGTXONOFF pMsgTxFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetGetDllHandleProcAddress(DLLGETNODEHANDLER pGetNodeHandler);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetKeyPressed(KEYPRESSED pKeyPressed);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetGetMessageName(GETMESSAGENAME pGetMessageName);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetTimeNow(TIMENOW pTimeNow);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetGetFirstCANdbName(GETFIRSTCANDBNAME pGetFirstCANdbName);
GCC_EXTERN /*extern "C"*/ HMODULE GCC_EXPORT vSetGetMsgNameProcAddress(GETMESSAGENAME pGetMessageName);

int keypressed_CAPL();
int timenow_CAPL();
int getmessagename_CAPL(DWORD dID, DWORD dContext, char* pBuffer,DWORD dSize);
int getfirstcandbname_CAPL(char* cBuffer, DWORD size);

typedef struct _InternalStructLIN
{
    _InternalStructLIN()
    {
        Trace("temporary Structure");
    }
} InternalStructLIN;
InternalStructLIN objLin;

// Prototypes End
