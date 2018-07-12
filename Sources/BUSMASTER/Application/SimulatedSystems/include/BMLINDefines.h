#pragma once

#include <windows.h>
#include <string>
#include <string.h>
#include <map>
#include <list>
#include <stdio.h>

//BUSMASTER Helper file Includes
#include "BMUtility.h"
#include "BMSignal.h"
#include "Common.h"
#include "Wrapper_Common.h"

class DBLinMsg;
class STLIN_MSG;
GCC_EXTERN void copyBM_ST_MSG_LIN(DBLinMsg* pBmMsg, STLIN_MSG* pUserMsg);
GCC_EXTERN void copyST_BM_MSG_LIN(STLIN_MSG* pUserMsg, DBLinMsg* pBmMsg);

class STLIN_MSG
{
public:
    unsigned char messagetype;        // Protected Identifier
    unsigned char checksumtype;    // Checksum Type (0 - Classical / 1 - Enhanced)
    unsigned char dlc;      // Data length (0..8)
    unsigned char id;        // Protected Identifier
    unsigned char data[8];
    unsigned long timeStamp;
    unsigned char cluster;
    unsigned char crc;       // Checksum


    STLIN_MSG(unsigned char _checksumtype = 0, unsigned char _id = 0, unsigned char _dlc = 8)
        :   messagetype(0), checksumtype(_checksumtype), dlc(_dlc), id(_id), timeStamp(0), cluster(1), crc(0)
    {
        memset(data, 0, sizeof(data));
    }

    bool byteAt(int index, unsigned char val)
    {
        return setByteAt(data, dlc, index, val);
    }
    bool wordAt(int index, unsigned short val)
    {
        return setWordAt(data, dlc, index, val);
    }

    bool longAt(int index, unsigned long val)
    {
        return setLongAt(data, dlc, index, val);
    }

    unsigned char  byteAt(int index)
    {
        return getByteAt(data, dlc, index);
    }
    unsigned short wordAt(int index)
    {
        return getWordAt(data, dlc, index);
    }

    unsigned long longAt(int index)
    {
        return getLongAt(data, dlc, index);
    }
};

class DBLinMsg : public STLIN_MSG
{
public:
    const std::string name;

    DBLinMsg(std::string _name, unsigned char _checksumtype, int _id, int _dlc, STLIN_MSG* pRefMsg = NULL)
        :STLIN_MSG(_checksumtype, _id, _dlc), name(_name)
    {
        copyBM_ST_MSG_LIN(this, pRefMsg);
    }
};


//This structure holds the error and the channel number
class SEVENT_CHECKSUM_LIN
{
public:
    unsigned char m_ucId;
    unsigned char m_ucCrc;
    unsigned char m_ucChannel;
    unsigned int  m_ulTime;
} ;

class SEVENT_RECEIVE_LIN
{
public:
    unsigned int  m_ulTime;
};

class SEVENT_SLAVE_NORESP_LIN
{
public:
    unsigned char m_ucId;
    unsigned char m_ucChannel;
    unsigned int  m_ulTime;

};

class SEVENT_SYNC_LIN
{
public:
    unsigned char m_ucChannel;
    unsigned int  m_ulTime;
};

//LIN Callback function to BUSMASTER
typedef UINT (*APPFUNCPROC) (void*,HMODULE);
typedef void (*RESETCONTROLLER) (BOOL);
typedef UINT (*CONTROLLERMODE) (BOOL);
typedef UINT (*DLLMSGTXONOFF)(BOOL,HMODULE);
typedef int (*GETMESSAGENAME)(DWORD, DWORD , char* ,DWORD );
typedef void (*DLLGETMESSAGENAME)(GETMESSAGENAME);
typedef DWORD (*GETFIRSTCANDBNAME)(char* , DWORD );
typedef void (*DLLGETFIRSTCANDBNAME)(GETFIRSTCANDBNAME);
GCC_EXTERN void GCC_EXPORT vSetSendMsgProcAddress(APPFUNCPROC pAppFunc);
GCC_EXTERN void GCC_EXPORT vSetSetActiveModeProcAddress(CONTROLLERMODE pActiveModeFunc);
GCC_EXTERN void GCC_EXPORT vSetSetPassiveModeProcAddress(CONTROLLERMODE pPassiveModeFunc);
GCC_EXTERN void GCC_EXPORT vSetEnableDisableMsgTxProcAddress(DLLMSGTXONOFF pMsgTxFunc);
GCC_EXTERN void GCC_EXPORT vSetGetMessageName(GETMESSAGENAME pGetMessageName);
GCC_EXTERN void GCC_EXPORT vSetGetFirstCANdbName(GETFIRSTCANDBNAME pGetFirstCANdbName);
GCC_EXTERN HMODULE GCC_EXPORT vSetGetMsgNameProcAddress(GETMESSAGENAME pGetMessageName);

//LIN Client functions
GCC_EXTERN UINT GCC_EXPORT SendMsg(STLIN_MSG);
GCC_EXTERN int getmessagename_CAPL(DWORD dID, DWORD dContext, char* pBuffer,DWORD dSize);
GCC_EXTERN int getfirstcandbname_CAPL(char* cBuffer, DWORD size);
//temporary include to force the linking of lib file
#ifndef NO_INCLUDE_TEMPSTRUCT
typedef struct _InternalStructlin
{
    _InternalStructlin()
    {
        char temp[] = "teporary structur";
        Trace(temp);
    }
} InternalStructlin;
InternalStructlin objlin;
#endif