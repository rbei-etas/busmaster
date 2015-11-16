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
#include "Wrapper_Common.h"

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

struct SEVENT_SYNC_LIN
{
    unsigned char m_ucChannel;
    unsigned int  m_ulTime;

};


//typedef
typedef UINT (*APPFUNCPROC) (void*,HMODULE);
typedef void (*RESETCONTROLLER) (BOOL);
typedef UINT (*CONTROLLERMODE) (BOOL);
typedef UINT (*DLLMSGTXONOFF)(BOOL,HMODULE);

//typedef void (*GETMESSAGENAME)( DWORD id, char buffer[], DWORD size);
//typedef void (*SETGETMESSAGENAME)(GETMESSAGENAME);


typedef int (*GETMESSAGENAME)(DWORD, DWORD , char* ,DWORD );
typedef void (*DLLGETMESSAGENAME)(GETMESSAGENAME);


typedef DWORD (*GETFIRSTCANDBNAME)(char* , DWORD );
typedef void (*DLLGETFIRSTCANDBNAME)(GETFIRSTCANDBNAME);



// Please insert any new wrapper prototype here
// Prototypes Start
GCC_EXTERN /*extern "C"*/ UINT GCC_EXPORT SendMsg(STLIN_MSG);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetSendMsgProcAddress(APPFUNCPROC pAppFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetSetActiveModeProcAddress(CONTROLLERMODE pActiveModeFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetSetPassiveModeProcAddress(CONTROLLERMODE pPassiveModeFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetEnableDisableMsgTxProcAddress(DLLMSGTXONOFF pMsgTxFunc);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetGetMessageName(GETMESSAGENAME pGetMessageName);
GCC_EXTERN /*extern "C"*/ void GCC_EXPORT vSetGetFirstCANdbName(GETFIRSTCANDBNAME pGetFirstCANdbName);
GCC_EXTERN /*extern "C"*/ HMODULE GCC_EXPORT vSetGetMsgNameProcAddress(GETMESSAGENAME pGetMessageName);

GCC_EXTERN int getmessagename_CAPL(DWORD dID, DWORD dContext, char* pBuffer,DWORD dSize);
GCC_EXTERN int getfirstcandbname_CAPL(char* cBuffer, DWORD size);
#ifndef NO_INCLUDE_TEMPSTRUCT
struct InternalStructLIN
{
    InternalStructLIN()
    {
        char temp[] = "teporary structure";
        Trace(temp);
    }
};
InternalStructLIN objLin;
#endif
// Prototypes End
