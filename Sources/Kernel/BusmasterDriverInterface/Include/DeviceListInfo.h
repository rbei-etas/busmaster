#pragma once
#include <windows.h>
#define defMAX_LENGTH_NAME         256
#define defMAX_DILS                32
#define defCHANNEL_CAN_MAX         16
#include <string>

const BYTE MSGBUF_ADD = 0x1;
const BYTE MSGBUF_CLEAR = 0x0;

#define TX_FLAG                 0x01
#define RX_FLAG                 0x02
#define ERR_FLAG                0x04
#define INTR_FLAG               0x08

#define IS_TX_MESSAGE(a)        (a & TX_FLAG)
#define IS_RX_MESSAGE(a)        (a & RX_FLAG)
#define IS_A_MESSAGE(a)         ((a) & 0x03)
#define IS_ERR_MESSAGE(a)       (a & ERR_FLAG)
#define IS_INTR_MESSAGE(a)      (((a) & INTR_FLAG))

#define defMODE_ACTIVE                         1
#define defMODE_PASSIVE                        2
#define defMODE_SIMULATE                       3

#define defCONTROLLER_ACTIVE                   1
#define defCONTROLLER_PASSIVE                  2
#define defCONTROLLER_BUSOFF                   3



typedef unsigned int TYPE_CHANNEL;

const TYPE_CHANNEL CHANNEL_All_UNSPECIFIED = (UINT)-1; // Unspecified channel
const TYPE_CHANNEL CAN_CHANNEL_ALL = 0;     // All channel notation
const TYPE_CHANNEL CHANNEL_CAN_MIN = 1;     // Minimum value of a CAN channel
const TYPE_CHANNEL CHANNEL_CAN_MAX = 32;    // Maximum value of a CAN channel
const TYPE_CHANNEL CHANNEL_ALLOWED = 16;
const TYPE_CHANNEL FLEXRAY_CHANNEL_ALLOWED = 1;
#define defNO_OF_CHANNELS CHANNEL_CAN_MAX

const TYPE_CHANNEL LIN_CHANNEL_ALL = 0;     // All channel notation
const TYPE_CHANNEL CHANNEL_LIN_MIN = 1;     // Minimum value of a LIN channel
const TYPE_CHANNEL CHANNEL_LIN_MAX = 32;    // Maximum value of a LIN channel
const TYPE_CHANNEL LIN_CHANNEL_ALLOWED = 1;
#define defNO_OF_LIN_CHANNELS CHANNEL_LIN_MAX

const TYPE_CHANNEL FLEXRAY_CHANNEL_ALL = 0;     // All channel notation
const TYPE_CHANNEL CHANNEL_FLEXRAY_MIN = 1;     // Minimum value of a FlexRay channel
const TYPE_CHANNEL CHANNEL_FLEXRAY_MAX = 32;    // Maximum value of a FlexRay channel
#define defNO_OF_FLEXRAY_CHANNELS CHANNEL_FLEXRAY_MAX

class DILINFO
{
public:
    std::string m_acName;
    unsigned long  m_dwDriverID;
    unsigned int   m_ResourceID;
};

class STATUSMSG
{
public:
    unsigned short wControllerStatus;    // Current controller state
    unsigned long  dwStatusInfoFlags;    // Flagfield of status information (UCI_FLXSTSINFO_???)
};
/**
* This structure holds Error count of Rx and Tx Process
*/
class SERROR_CNT
{
public:
    /** transmit error count */
    unsigned char m_ucTxErrCount;

    /** receive error count */
    unsigned char m_ucRxErrCount;
};

enum ECONTR_PARAM
{
    NUMBER_HW = 0,
    NUMBER_CONNECTED_HW,
    PEAK_ERR_CNT,
    DRIVER_STATUS,
    ERR_CNT,
    HW_MODE,
    CNTR_STATUS,
    CON_TEST,
};

enum ECONTR_STATUS
{
    RESET_STATE = 0,    // reset
    INITIALISED,        // stopped / initialized
    WAITING,            // started / waiting for startup completion
    NORMAL_ACTIVE,      // started / normal active (running)
    NORMAL_PASSIVE,     // started / normal passiv
    NOT_DEFINED,        // started / halt mode
};

class INTERFACE_HW
{
public:
    unsigned long   m_dwIdInterface;
    unsigned long   m_dwVendor;
    unsigned char   m_bytNetworkID;
    std::string m_acNameInterface;
    std::string m_acDescription;
    std::string m_acDeviceName;
    std::string m_acAdditionalInfo;
};

/*Todo*/
enum eDirection
{
    DIR_RX = 'R',
    DIR_TX = 'T',
    DIR_ALL
};
typedef eDirection EDIRECTION;

typedef DILINFO DILLIST[defMAX_DILS];
typedef INTERFACE_HW INTERFACE_HW_LIST[defCHANNEL_CAN_MAX];
typedef SERROR_CNT* SPERROR_CNT;