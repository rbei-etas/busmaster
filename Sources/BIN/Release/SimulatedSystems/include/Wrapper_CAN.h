/*******************************************************************************
  Project       :  CAN-Monitor
  FileName      :  wrapper.c
  Description   :

  $Log:   Y:/EMC2/Projects/CAN_Monitor_Maintenance/Archives/Source/Misc/GCC/Wrapper.h_v  $
 *
 *    Rev 1.8   15 Sep 2010 17:26:42   rac2kor
 *
 *
 *    Rev 1.7   27 Dec 2006 16:12:14   amb2kor
 * Changes for Global variable support: Multiple database support.
 *
 *    Rev 1.6   02 Feb 2006 19:55:38   amb2kor
 * Changes due to CRH0031 ( Multinode Support )
 *
 *    Rev 1.5   21 Jul 2005 19:05:24   amb2kor
 * changes due to Multi Channel concept. Trace is modified and
 * New API to get version information is added.
 *
 *    Rev 1.4   Aug 07 2004 07:48:08   amb2kor
 * Changes due to CRH0026: After UT.
 *
 *    Rev 1.3   30 Oct 2003 11:01:32   amb2kor
 *
 *
 *    Rev 1.2   02 May 2003 11:55:44   amb2kor
 * Removed parameter name sTxMsg from SendMsg function.
 *
 *    Rev 1.1   24 Apr 2003 09:50:22   amb2kor
 * For system testing
 *
 *    Rev 1.0.1.1   03 Apr 2003 12:20:48   amb2kor
 * prototype of SendMsg function is changed to accept single structure.
 *
 *    Rev 1.0   14 Feb 2003 14:34:36   amb2kor
 * Base Version : Latest version From CAN-Monitor2.x project
 * Version Label in CAN-Monitor2.x : CAN-Monitor2.0 EVAL/CMRL
 *
 *

  Author(s)     :  Amarnath Shastry
  Date Created  :  20/03/2002
  Modified By   :  Ravikumar Patil  on 04.03.03
*******************************************************************************/
#ifndef _WRAPPER_H_
#define _WRAPPER_H_
#include <windows.h>
#include <stdio.h>
#include "Common.h"


//Structures
typedef struct sTCAN_MSG
{
    unsigned int m_unMsgID;     // 11/29 Bit-
    unsigned char m_ucEXTENDED; // true, for (29 Bit) Frame
    unsigned char m_ucRTR;      // true, for remote request
    unsigned char m_ucDLC;  // Data len (0..8)
    unsigned char m_ucChannel;
    union
    {
        unsigned char      m_aucData[8];
        unsigned short int m_auwData[4];
        unsigned long  int m_aulData[2];
    } m_sWhichBit;
    unsigned long m_ulTimeStamp;
    /*STCAN_MSG(UINT unMsgID=0, UCHAR ucExt=0, UCHAR ucRtr=0, UCHAR ucDLC=8, UCHAR ucChannel=1,
            UCHAR b0=0,UCHAR b1=0,UCHAR b2=0,UCHAR b3=0,UCHAR b4=0,UCHAR b5=0,UCHAR b6=0,UCHAR b7=0)
    {
        m_unMsgID = unMsgID;
        m_ucDLC=ucDLC;
        m_ucEXTENDED=ucExt;
        m_ucRTR=ucRtr;
        m_ucChannel=ucChannel;
        m_sWhichBit.m_aucData[0]=b0;
        m_sWhichBit.m_aucData[1]=b1;
        m_sWhichBit.m_aucData[2]=b2;
        m_sWhichBit.m_aucData[3]=b3;
        m_sWhichBit.m_aucData[4]=b4;
        m_sWhichBit.m_aucData[5]=b5;
        m_sWhichBit.m_aucData[6]=b6;
        m_sWhichBit.m_aucData[7]=b7;
    }*/
} STCAN_MSG;

//This structure holds the error and the channel number
typedef struct sCAN_ERR
{
    unsigned char m_ucTxError ;
    unsigned char m_ucRxError ;
    unsigned char m_ucChannel ;
} SCAN_ERR;

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
GCC_EXTERN UINT GCC_EXPORT SendMsg(STCAN_MSG);
GCC_EXTERN UINT GCC_EXPORT EnableLogging() ;
GCC_EXTERN UINT GCC_EXPORT DisableLogging() ;
GCC_EXTERN UINT GCC_EXPORT WriteToLogFile(char*) ;
GCC_EXTERN UINT GCC_EXPORT Trace(char*,...) ;
GCC_EXTERN void GCC_EXPORT ResetController(BOOL) ;
GCC_EXTERN UINT GCC_EXPORT SetPassiveMode() ;
GCC_EXTERN UINT GCC_EXPORT SetActiveMode() ;
GCC_EXTERN UINT GCC_EXPORT GoOnline() ;
GCC_EXTERN UINT GCC_EXPORT GoOffline() ;
GCC_EXTERN UINT GCC_EXPORT Connect() ;
GCC_EXTERN UINT GCC_EXPORT Disconnect() ;
GCC_EXTERN UINT GCC_EXPORT StartTimer(char*,UINT) ;
GCC_EXTERN UINT GCC_EXPORT StopTimer(char*) ;
GCC_EXTERN BOOL GCC_EXPORT SetTimerVal(char*,UINT);
GCC_EXTERN BOOL GCC_EXPORT EnableMsgHandlers(BOOL);
GCC_EXTERN BOOL GCC_EXPORT EnableErrorHandlers(BOOL);
GCC_EXTERN BOOL GCC_EXPORT EnableKeyHandlers(BOOL);
GCC_EXTERN BOOL GCC_EXPORT bGetProgramVersion(int* pnMajorVersion, int* pnMinorVersion,HMODULE hModuleHandle );
GCC_EXTERN BOOL GCC_EXPORT EnableDisableMsgTx(BOOL);
GCC_EXTERN HMODULE GCC_EXPORT GetDllHandle(char*);
GCC_EXTERN void GCC_EXPORT vSetSendMsgProcAddress(APPFUNCPROC pAppFunc);
GCC_EXTERN void GCC_EXPORT vSetEnableLoggingProcAddress(LOGENABLE pEnableLoggingFunc);
GCC_EXTERN void GCC_EXPORT vSetDisableLoggingProcAddress(LOGENABLE pDisableLoggingFunc);
GCC_EXTERN void GCC_EXPORT vSetWriteToLogFileProcAddress(WRITETOLOGFILE pLogFileFunc);
GCC_EXTERN void GCC_EXPORT vSetTraceProcAddress(WRITETOTRACE pTraceFunc);
GCC_EXTERN void GCC_EXPORT vSetResetControllerProcAddress(RESETCONTROLLER pResetFunc);
GCC_EXTERN void GCC_EXPORT vSetSetActiveModeProcAddress(CONTROLLERMODE pActiveModeFunc);
GCC_EXTERN void GCC_EXPORT vSetSetPassiveModeProcAddress(CONTROLLERMODE pPassiveModeFunc);
GCC_EXTERN void GCC_EXPORT vSetGoOfflineProcAddress(GOONLINEOFFLINE pGoOffline);
GCC_EXTERN void GCC_EXPORT vSetGoOnlineProcAddress(GOONLINEOFFLINE pGoOnline);
GCC_EXTERN void GCC_EXPORT vSetDisconnectProcAddress(DISCONNECT pDisconnect);
GCC_EXTERN void GCC_EXPORT vSetConnectProcAddress(DISCONNECT pConnect);
GCC_EXTERN void GCC_EXPORT vSetStopTimerProcAddress(STARTSTOPTIMER pStopTimer);
GCC_EXTERN void GCC_EXPORT vSetStartTimerProcAddress(STARTSTOPTIMER pStartTimer);
GCC_EXTERN void GCC_EXPORT vSetSetTimerValProcAddress(SETTIMERVAL pSetTimerVal);
GCC_EXTERN void GCC_EXPORT vSetEnableMsgHandlersProcAddress(ENABLEDISABLEMSGHANDLERS pMsgHandlers);
GCC_EXTERN void GCC_EXPORT vSetEnableKeyHandlersProcAddress(ENABLEDISABLEKEYHANDLERS pKeyHandlers);
GCC_EXTERN void GCC_EXPORT vSetEnableErrorHandlersProcAddress(ENABLEDISABLEERRORHANDLERS pErrorHandlers);
GCC_EXTERN void GCC_EXPORT vSetEnableDisableMsgTxProcAddress(DLLMSGTXONOFF pMsgTxFunc);
GCC_EXTERN void GCC_EXPORT vSetGetDllHandleProcAddress(DLLGETNODEHANDLER pGetNodeHandler);
GCC_EXTERN void GCC_EXPORT vSetKeyPressed(KEYPRESSED pKeyPressed);
GCC_EXTERN void GCC_EXPORT vSetGetMessageName(GETMESSAGENAME pGetMessageName);
GCC_EXTERN void GCC_EXPORT vSetTimeNow(TIMENOW pTimeNow);
GCC_EXTERN void GCC_EXPORT vSetGetFirstCANdbName(GETFIRSTCANDBNAME pGetFirstCANdbName);
GCC_EXTERN HMODULE GCC_EXPORT vSetGetMsgNameProcAddress(GETMESSAGENAME pGetMessageName);

int keypressed_CAPL();
int timenow_CAPL();
int getmessagename_CAPL(DWORD dID, DWORD dContext, char* pBuffer,DWORD dSize);
int getfirstcandbname_CAPL(char* cBuffer, DWORD size);



// Prototypes End
#endif