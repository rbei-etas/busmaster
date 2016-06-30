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

class BMJ1939Msg;
class J1939_MSG;
GCC_EXTERN void copyBM_ST_MSG_J1939(BMJ1939Msg* pBmMsg, J1939_MSG* pUserMsg);
GCC_EXTERN void copyST_BM_MSG_J1939(J1939_MSG* pUserMsg, BMJ1939Msg* pBmMsg);

enum EDIRECTION
{
    DIR_RX = 'R',
    DIR_TX = 'T',
    DIR_ALL
};

enum EJ1939_MSG_TYPE
{
    MSG_TYPE_NONE = 0x0,
    MSG_TYPE_COMMAND,
    MSG_TYPE_REQUEST,
    MSG_TYPE_DATA,
    MSG_TYPE_BROADCAST,
    MSG_TYPE_ACKNOWLEDGEMENT,
    MSG_TYPE_GROUP_FUNCTIONS,

    MSG_TYPE_NM_ACL,
    MSG_TYPE_NM_RQST_ACL,
    MSG_TYPE_NM_CMD_ADDRESS,
    MSG_TYPE_NM_TPCM_BAM,
    MSG_TYPE_NM_TPCM_RTS,
    MSG_TYPE_NM_TPCM_CTS,
    MSG_TYPE_NM_TPCM_EOM_ACK,
    MSG_TYPE_NM_TPCM_CON_ABORT,
    MSG_TYPE_NM_TPDT
};

class J1939Id
{
private:
    unsigned int   extendedId;  // 29bit id = 3bit priority, 18bit PGN, 8bit Source Address
    // 18bit PGN = 1bit reserved, 1 bit datapage, 8bit pdu format, 8 bit pdu specific


public:
    void operator = (unsigned int identifier)
    {
        extendedId = identifier;
    }

    operator unsigned int()
    {
        return extendedId;
    }

    void setId(unsigned int _extendedId)
    {
        extendedId = _extendedId;
    }

    void setSourceAddress(unsigned char _sourceAddress)
    {
        extendedId = (extendedId & 0xFFFFFF00) | _sourceAddress;
    }

    void setPGN(unsigned int _pgn)
    {
        // 11100000000000000000011111111
        _pgn = _pgn << 8; // move to the pgn position
        extendedId = (extendedId & 0x1C0000FF) | _pgn;
    }

    void setPDUFormat(unsigned char _pduFormat)
    {
        // 11111000000001111111111111111
        unsigned int pduformat = _pduFormat;
        pduformat = pduformat << 16; // MOVE to the pduformat position
        extendedId = (extendedId & 0x1F00FFFF) | pduformat;
    }

    void setDataPage(unsigned char _dataPage)
    {
        // 11110111111111111111111111111
        unsigned int dataPage = _dataPage;
        //pduformat = (extendedId & 0x1F00FFFF);
        dataPage = dataPage << 24; // move to the datapage position
        extendedId = (extendedId & 0x1EFFFFFF) | dataPage;

    }

    void setPriority(unsigned char _priority)
    {
        unsigned int priority = _priority;
        //pduformat = (extendedId & 0x1F00FFFF);
        priority = priority << 26; // move to the priority bit position
        extendedId = (extendedId & 0x3FFFFFF) | priority;
    }

    void setPDUSpecific(unsigned char _pduSpecific)
    {
        //11111111111110000000011111111
        unsigned int pduspecific = _pduSpecific;
        pduspecific = pduspecific << 8; // move to the pduspecific bit position
        extendedId = (extendedId & 0x1FFF00FF) | pduspecific;
    }

    unsigned char getSourceAddress()
    {
        return (extendedId & 0xFF);
    }
    unsigned char getPDUFormat()
    {
        unsigned int pgn = extendedId & 0x3FFFFFF;// excluding priority 3 bits
        pgn = (pgn >> 16) & 0xFF; // excluding 8bit Source Address and 8 bit Pdu Specific and 1 bit reserved, 1 bit datapage
        return pgn;
    }

    unsigned int getPGN()
    {
        unsigned int pgn = 0;
        if(getPDUFormat() < 240)
        {

            pgn = extendedId & 0x3FF00FF; // excluding priority 3 bits
            pgn = (pgn >> 8); // excluding 8bit Source Address //and 8 bit Pdu Specific
        }
        else
        {
            // if canid is 0x1CEAA6FD then PGN is EA and pdu specific is A6
            pgn = extendedId & 0x3FFFFFF; // excluding priority 3 bits
            pgn = pgn >> 8; // excluding 8bit Source Address //and 8 bit Pdu Specific
        }



        return pgn;
    }

    unsigned char getDataPage()
    {
        unsigned int pgn = extendedId & 0x3FFFFFF;// excluding priority 3 bits
        pgn = (pgn >> 24) & 1; // excluding 8bit Source Address and 8 bit Pdu Specific and 1 bit reserved

        return pgn;
    }

    unsigned char getPriority()
    {
        unsigned int pgn = extendedId & 0x1C000000;

        pgn = pgn >> 26;
        return pgn;
    }

    unsigned char getPDUSpecific()
    {
        unsigned char pgn = (extendedId >> 8) & 0xFF; // excluding priority 3 bits
        return pgn;
    }
};

class J1939_MSG
{
public:

    unsigned __int64        timeStamp; // Timestamp.
    unsigned char           cluster;     // Channel number.
    EJ1939_MSG_TYPE         msgType;
    enum EDIRECTION         direction;    // DIR_TX, DIR_RX
    J1939Id                     id;
    unsigned int dlc;            // Data length. 0 <= m_unDLC <= 1785 bytes.
    unsigned char data[1785];

    J1939_MSG(unsigned int _id=0, unsigned char _dlc=8) :
        timeStamp(0), cluster(1), msgType(MSG_TYPE_BROADCAST), direction(DIR_RX), dlc(_dlc)
    {
        id.setPGN(_id);
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

class BMJ1939Msg : public J1939_MSG
{
public:
    const std::string name;


    BMJ1939Msg(std::string _name, unsigned int _id, int _dlc, J1939_MSG* pRefMsg ):
        J1939_MSG(_id, _dlc), name(_name)
    {
        id.setPGN(_id);
        copyBM_ST_MSG_J1939(this, pRefMsg);
    }
};



//J1939 Callback function to BUSMASTER
typedef UINT    (*APPSENDPROC_J) (void*, HMODULE);
typedef UINT    (*APPREQUESTPGN_J) (UINT, BYTE, UINT, HMODULE);
typedef UINT    (*APPACKMSG_J) (UINT, BYTE, BYTE, UINT, HMODULE);
typedef UINT    (*APPCLAIMADDRESS_J) (BYTE, UINT, HMODULE);
typedef UINT    (*APPRQSTADDRESS_J) (UINT, HMODULE);
typedef UINT    (*APPCMDADDRESS_J) (UINT64, BYTE, UINT, HMODULE);
typedef UINT    (*APPSETTIMEOUT_J) (BYTE, UINT, HMODULE);
typedef UINT    (*APPONLINEOFFLINE_J) (BOOL, HMODULE);
typedef UINT    (*GOONLINEOFFLINE) (BOOL, HMODULE) ;
typedef BOOL    (*MSGTXONOFF)(BOOL, HMODULE);
GCC_EXTERN BOOL GCC_EXPORT EnableDisableMsgTx(BOOL);
GCC_EXTERN void GCC_EXPORT vSetSendMsgProcAddress(APPSENDPROC_J pAppFunc);
GCC_EXTERN void GCC_EXPORT vSetGoOfflineProcAddress(GOONLINEOFFLINE pGoOffline);
GCC_EXTERN void GCC_EXPORT vSetGoOnlineProcAddress(GOONLINEOFFLINE pGoOnline);
GCC_EXTERN void GCC_EXPORT vSetEnableDisableMsgTxProcAddress(MSGTXONOFF pMsgTxFunc);
GCC_EXTERN void GCC_EXPORT vSetClaimAddressProcAddress(APPCLAIMADDRESS_J pClaimAddress);
GCC_EXTERN void GCC_EXPORT vSetCommandAddressProcAddress(APPCMDADDRESS_J pCommandAddress);
GCC_EXTERN void GCC_EXPORT vSetRequestPGNProcAddress(APPREQUESTPGN_J pRequestPGN);
GCC_EXTERN void GCC_EXPORT vSetRequestAddressProcAddress(APPRQSTADDRESS_J pRequestAddress);
GCC_EXTERN void GCC_EXPORT vSetSendAckMsgProcAddress(APPACKMSG_J pSendAckMsg);
GCC_EXTERN void GCC_EXPORT vSetSetTimeoutProcAddress(APPSETTIMEOUT_J pSetTimeout);



//J1939 Client functions
GCC_EXTERN UINT GCC_EXPORT GoOnline() ;
GCC_EXTERN UINT GCC_EXPORT GoOffline() ;
GCC_EXTERN UINT GCC_EXPORT SendMsg(J1939_MSG& sTxMsg);
GCC_EXTERN UINT GCC_EXPORT RequestPGN(UINT, BYTE, UINT);
GCC_EXTERN UINT GCC_EXPORT SendAckMsg(UINT, BYTE, BYTE, UINT);
GCC_EXTERN UINT GCC_EXPORT ClaimAddress(BYTE, UINT);
GCC_EXTERN UINT GCC_EXPORT RequestAddress(UINT);
GCC_EXTERN UINT GCC_EXPORT CommandAddress(UINT64, BYTE, UINT);
GCC_EXTERN UINT GCC_EXPORT SetTimout(BYTE, UINT);





//temporary include to force the linking of lib file
#ifndef NO_INCLUDE_TEMPSTRUCT
typedef struct _InternalStructJ1939
{
    _InternalStructJ1939()
    {
        char temp[] = "teporary structur";
        Trace(temp);
    }
} InternalStructJ1939;
InternalStructJ1939 objj1939;
#endif

