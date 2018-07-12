#pragma once

#include <string>
#include <string.h>
#include <map>
#include <list>
#include <stdio.h>

#include "BMSignal.h"
#include "Common.h"
#include "Wrapper_Common.h"
#include "BMUtility.h"

class DBCanMsg;
class STCAN_MSG;
GCC_EXTERN void copyBM_ST_MSG_CAN(DBCanMsg* pBmMsg, STCAN_MSG* pUserMsg);
GCC_EXTERN void copyST_BM_MSG_CAN(STCAN_MSG* pUserMsg, DBCanMsg* pBmMsg);

class STCAN_MSG
{
public:
    unsigned int id;        // 11/29 Bit-
    bool isExtended; // true, for (29 Bit) Frame Standard/Extended
    bool isRtr;    // true, for remote request
    unsigned char dlc;      // Data length (0..8)
    unsigned char cluster;
    unsigned char data[64];
    unsigned long timeStamp;
    bool isCanfd;
    STCAN_MSG(unsigned int _id=0, bool _isExtended = 0, unsigned char _dlc=8, bool _isCanfd = 0) :
        id(_id), isExtended(_isExtended), isRtr(0), dlc(_dlc), cluster(1), timeStamp(0), isCanfd(_isCanfd)
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

class DBCanMsg : public STCAN_MSG
{
public:
    const std::string name;

    DBCanMsg(std::string _name, bool _isExtended, unsigned int _id, int _dlc, STCAN_MSG* pRefMsg = NULL, bool _isCanfd = 0)
        :STCAN_MSG(_id, _isExtended, _dlc, _isCanfd), name(_name)
    {
        copyBM_ST_MSG_CAN(this, pRefMsg);
    }
};

//This structure holds the error and the channel number
typedef struct sCAN_ERR
{
    unsigned char m_ucTxError ;
    unsigned char m_ucRxError ;
    unsigned char m_ucChannel ;
} SCAN_ERR;


//call back functions to BUSMASTER
typedef void (*RESETCONTROLLER) (BOOL);
typedef UINT (*CONTROLLERMODE) (BOOL);
typedef UINT (*DLLMSGTXONOFF)(BOOL,HMODULE);
typedef UINT (*APPFUNCPROC) (void*,HMODULE);
typedef int (*GETMESSAGENAME)(DWORD, DWORD , char* ,DWORD );
typedef void (*DLLGETMESSAGENAME)(GETMESSAGENAME);
typedef DWORD (*GETFIRSTCANDBNAME)(char* , DWORD );
typedef void (*DLLGETFIRSTCANDBNAME)(GETFIRSTCANDBNAME);



// BUSMASTER interface functions
GCC_EXTERN UINT GCC_EXPORT SendMsg(STCAN_MSG);
GCC_EXTERN void GCC_EXPORT ResetController(BOOL) ;
GCC_EXTERN UINT GCC_EXPORT SetPassiveMode() ;
GCC_EXTERN UINT GCC_EXPORT SetActiveMode() ;
GCC_EXTERN BOOL GCC_EXPORT EnableDisableMsgTx(BOOL);
GCC_EXTERN void GCC_EXPORT vSetSendMsgProcAddress(APPFUNCPROC pAppFunc);
GCC_EXTERN void GCC_EXPORT vSetResetControllerProcAddress(RESETCONTROLLER pResetFunc);
GCC_EXTERN void GCC_EXPORT vSetSetActiveModeProcAddress(CONTROLLERMODE pActiveModeFunc);
GCC_EXTERN void GCC_EXPORT vSetSetPassiveModeProcAddress(CONTROLLERMODE pPassiveModeFunc);
GCC_EXTERN void GCC_EXPORT vSetEnableDisableMsgTxProcAddress(DLLMSGTXONOFF pMsgTxFunc);
GCC_EXTERN void GCC_EXPORT vSetGetMessageName(GETMESSAGENAME pGetMessageName);
GCC_EXTERN void GCC_EXPORT vSetGetFirstCANdbName(GETFIRSTCANDBNAME pGetFirstCANdbName);
GCC_EXTERN HMODULE GCC_EXPORT vSetGetMsgNameProcAddress(GETMESSAGENAME pGetMessageName);
GCC_EXTERN int getmessagename_CAPL(DWORD dID, DWORD dContext, char* pBuffer,DWORD dSize);
GCC_EXTERN int getfirstcandbname_CAPL(char* cBuffer, DWORD size);
unsigned long* longAt(STCAN_MSG* sMsg,int nByte);
unsigned short* wordAt(STCAN_MSG* sMsg,int nByte);



//temporary include to force the linking of lib file
#ifndef NO_INCLUDE_TEMPSTRUCT
typedef struct _InternalStructcan
{
    _InternalStructcan()
    {
        char temp[] = "teporary structur";
        Trace(temp);
    }
} InternalStructcan;
InternalStructcan objcan;
#endif