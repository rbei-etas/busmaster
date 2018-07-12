#pragma once
#include "DeviceListInfo.h"
#include <string>
#include <list>
#include <Windows.h>
#include "../../Utilities/BaseMsgBufAll.h"

#define LIN_MONITOR_NODE "LIN_MONITOR"
const int LIN_MONITOR_NODE_INDEX = 0;
const int LIN_MONITOR_CLIENT_ID = 1;

enum eLinBusEventType
{
    EVENT_LIN_ERRMSG,
    EVENT_LIN_ERRSYNC,
    EVENT_LIN_ERRNOANS,
    EVENT_LIN_ERRCRC,
    EVENT_LIN_WAKEUP,
    EVENT_LIN_SLEEP,
    EVENT_LIN_CRCINFO,
    EVENT_LIN_DLC,
    EVENT_LIN_ERRBIT,
    EVENT_LIN_ERRHEADER,
    EVENT_LIN_ERRTOUT, // For Timeout
    EVENT_LIN_COLLISION,
    EVENT_LIN_NO_MSTR_SCHED,
    EVENT_LIN_NO_TX_DATA,
    EVENT_LIN_NONE
};

enum eLIN_MSG_TYPE
{
    LIN_INVALID_RESPONSE = 0,
    LIN_MASTER_RESPONSE,
    LIN_SLAVE_RESPONSE,
    LIN_SLAVE_SLAVE,
    LIN_WAKEUP_REQUEST,
    LIN_SLEEP_FRAME,
    LIN_NO_ANSWER,
    LIN_RESPONSE_NOT_CONFIGURED,
    LIN_CHECKSUM_ERROR,
    LIN_DLC_ERROR,
    LIN_MSG_TYPE_MAX, /* Do not add msg type after LIN_MSG_TYPE_MAX */
};

enum eLIN_CHECKSUM_TYPE
{
    LIN_CHECKSUM_CLASSIC = 0,
    LIN_CHECKSUM_ENHANCED,
};

enum eLinMsgType
{
    LIN_MSG,
    LIN_EVENT
};

enum eLinSleepEvent
{
    LIN_SLEEPMODE_SET,
    LIN_SLEEPMODE_WAKEUP,
    LIN_SLEEPMODE_STAYALIVE,
};

enum eCheckSumType
{
    CHECKSUM_CLASSIC,
    CHECKSU_ENHANCED
};


class STLIN_MSG
{
public:
    unsigned char m_ucMsgTyp;       // Message Type (0 - Header / 1 - Response)
    unsigned char m_ucChksumTyp;    // Checksum Type (0 - Classical / 1 - Enhanced)
    unsigned char m_ucDataLen;      // Data len (0..8)
    unsigned char m_ucMsgID;        // Protected Identifier
    unsigned char m_ucData[8];      // Databytes 0..8
    unsigned long m_ulTimeStamp;
    unsigned char m_ucChannel;      // Channel Number
    unsigned char m_ucChksum;       // Checksum
};
typedef STLIN_MSG* PSTLIN_MSG;

struct SEVENT_SYNC_LIN
{
    unsigned char m_ucChannel;
    unsigned int  m_ulTime;
};

struct SEVENT_CHECKSUM_LIN
{
    unsigned char m_ucId;
    unsigned char m_ucCrc;
    unsigned char m_ucChannel;
    unsigned int  m_ulTime;
};

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

/**
* This structure holds Error info
*/
class SERROR_INFO_LIN
{
public:
    unsigned char m_ucId;       // Msg Id - Useful in case of NoAns
    unsigned char m_ucCrc;
    eLinBusEventType m_eEventType;
    //SEVENT_INFO_LIN  m_eEventInfo;
    eLinSleepEvent m_eSleepEvent;
    eCheckSumType m_eCheckSumType;
    //shashank
    unsigned char m_ucErrType;

    unsigned char m_ucReg_ErrCap; // Stores the value of err capture register in
    // case of bus error
    unsigned char m_ucTxErrCount;
    unsigned char m_ucRxErrCount;
    unsigned char m_ucChannel;
    unsigned char m_ucDlc;
    unsigned char m_ucDir;
    ULONGLONG m_ulTimeStamp;
    int           m_nSubError;   //added for providing Error bit details

    SERROR_INFO_LIN()
    {
        this->m_ucTxErrCount = 0;
        this->m_ucRxErrCount = 0;
    }
};

typedef SERROR_INFO_LIN* PSERROR_INFO_LIN;

class SCONTROLLER_DETAILS_LIN
{
public:
    std::string m_strHwUri;
    double m_BaudRate;
    std::string  m_strProtocolVersion;
};
typedef SCONTROLLER_DETAILS_LIN* PSCONTROLLER_DETAILS_LIN;

/**
* This structure is used for communicating between Driver and LIN Application
*/
class STLINDATAINFO
{
public:
    /* The received / transmitted message */
    STLIN_MSG         m_sLINMsg;
    SERROR_INFO_LIN   m_sErrInfo;
};
typedef STLINDATAINFO* PSTLINDATAINFO;


/**
* This structure is used for communicating between Driver and LIN Application
*/
class STLINDATA
{
private:
    static int  m_nSortField;

    /* Multiplication factor */
    static int  m_nMFactor;

public:
    eLinMsgType     m_eLinMsgType;

    /** Type of the message */
    unsigned char    m_ucDataType;

    /** Time stamp, Contains the val returned from */
    LARGE_INTEGER    m_lTickCount;
    // QueryPerf..Counter()
    STLINDATAINFO       m_uDataInfo;

};
typedef STLINDATA* PSTLINDATA;


typedef CBaseMsgBufFSE<STLINDATA> CBaseLINBufFSE;