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
#pragma once

#include <windows.h>
#include <stdio.h>
#include "Common.h"
#include "Wrapper_Common.h"

//Structures
typedef struct sTCAN_MSG
{
    unsigned int  m_unMsgID;     // 11/29 Bit-
    unsigned char m_ucEXTENDED; // true, for (29 Bit) Frame
    unsigned char m_ucRTR;      // true, for remote request
    unsigned char m_ucDLC;  // Data len (0..8)
    unsigned char m_ucChannel;
    union
    {
        unsigned char      m_aucData[64];
        unsigned short int m_auwData[4];
        unsigned long  int m_aulData[2];
    } m_sWhichBit;
    unsigned long m_ulTimeStamp;
    bool          m_bCANFD;
} STCAN_MSG;

//This structure holds the error and the channel number
typedef struct sCAN_ERR
{
    unsigned char m_ucTxError ;
    unsigned char m_ucRxError ;
    unsigned char m_ucChannel ;
} SCAN_ERR;

//typedef
typedef void (*RESETCONTROLLER) (BOOL);
typedef UINT (*CONTROLLERMODE) (BOOL);
typedef UINT (*DLLMSGTXONOFF)(BOOL,HMODULE);
typedef UINT (*APPFUNCPROC) (void*,HMODULE);

//typedef void (*GETMESSAGENAME)( DWORD id, char buffer[], DWORD size);
//typedef void (*SETGETMESSAGENAME)(GETMESSAGENAME);


typedef int (*GETMESSAGENAME)(DWORD, DWORD , char* ,DWORD );
typedef void (*DLLGETMESSAGENAME)(GETMESSAGENAME);


typedef DWORD (*GETFIRSTCANDBNAME)(char* , DWORD );
typedef void (*DLLGETFIRSTCANDBNAME)(GETFIRSTCANDBNAME);



// Please insert any new wrapper prototype here
// Prototypes Start
GCC_EXTERN /*extern "C"*/void GCC_EXPORT OnMsgID_111(STCAN_MSG RxMsg);
GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT SendMsg(STCAN_MSG);

GCC_EXTERN /*extern "C"*/ void GCC_EXPORT ResetController(BOOL) ;
GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT SetPassiveMode() ;
GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT SetActiveMode() ;
GCC_EXTERN /*extern "C"*/ BOOL GCC_EXPORT EnableDisableMsgTx(BOOL);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetSendMsgProcAddress(APPFUNCPROC pAppFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetResetControllerProcAddress(RESETCONTROLLER pResetFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetSetActiveModeProcAddress(CONTROLLERMODE pActiveModeFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetSetPassiveModeProcAddress(CONTROLLERMODE pPassiveModeFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetEnableDisableMsgTxProcAddress(DLLMSGTXONOFF pMsgTxFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetGetMessageName(GETMESSAGENAME pGetMessageName);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetGetFirstCANdbName(GETFIRSTCANDBNAME pGetFirstCANdbName);
GCC_EXTERN /*extern "C"*/ HMODULE GCC_EXPORT vSetGetMsgNameProcAddress(GETMESSAGENAME pGetMessageName);


GCC_EXTERN int getmessagename_CAPL(DWORD dID, DWORD dContext, char* pBuffer,DWORD dSize);
GCC_EXTERN int getfirstcandbname_CAPL(char* cBuffer, DWORD size);
#ifndef NO_INCLUDE_TEMPSTRUCT
typedef struct _InternalStructCAN
{
    _InternalStructCAN()
    {
        char temp[] = "teporary structur";
        Trace(temp);
    }
} InternalStructCAN;
_InternalStructCAN objcan;
#endif

// Prototypes End
