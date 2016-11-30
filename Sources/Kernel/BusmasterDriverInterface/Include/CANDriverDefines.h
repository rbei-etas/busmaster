#pragma once
#include <string>
#include "DeviceListInfo.h"
#include <windows.h>
#include "../../Utilities\BaseMsgBufAll.h"
#include "DILPluginHelperDefs.h"


#define defMAX_HW                  32
#define defMAX_DILNAME             32
#define defCAN_MSG_IDS             2
#define CAN_MONITOR_NODE           "CAN_MONITOR"
const int CAN_MONITOR_NODE_INDEX = 0;
const int CAN_MONITOR_CLIENT_ID = 1;

//Error Val Interpretation
#define ERROR_BUS                       0x01
#define ERROR_DEVICE_BUFF_OVERFLOW      0x02
#define ERROR_DRIVER_BUFF_OVERFLOW      0x03
#define ERROR_WARNING_LIMIT_REACHED     0x04
#define ERROR_APPLICATION_BUFF_OVERFLOW 0x05
#define ERROR_TX_TIMEOUT                0x06
#define ERROR_INTERRUPT                 0x07
#define ERROR_DLLMSG_BUFF_OVERFLOW      0x08

#define ERROR_UNKNOWN                   0xFF


#define BIT_ERROR_TX                       0x00
#define CRC_ERROR_TX                       0x10
#define FORM_ERROR_TX                      0x40
#define STUFF_ERROR_TX                     0x80
#define OTHER_ERROR_TX                     0xC0
#define BIT_ERROR_RX                       0x20
#define CRC_ERROR_RX                       0xB0
#define FORM_ERROR_RX                      0x60
#define STUFF_ERROR_RX                     0xA0
#define OTHER_ERROR_RX                     0xE0
#define ACK_ERROR_TX                       0xF0
#define ACK_ERROR_RX                       0XF1

#define defMAX_TIME_DELAY                   60000
/**
* This enum defines different filter types
*/
enum eHW_FILTER_TYPES
{
    HW_FILTER_ACCEPT_ALL = 0,
    HW_FILTER_REJECT_ALL,
    HW_FILTER_MANUAL_SET
};

enum ERROR_STATE
{
    ERROR_ACTIVE = 0,
    ERROR_WARNING_LIMIT,
    ERROR_PASSIVE,
    ERROR_BUS_OFF,
    ERROR_FRAME,
    ERROR_INVALID
};


class SCONTROLLER_DETAILS
{
public:
    int     m_nItemUnderFocus;                    // item number under focus
    int     m_nBTR0BTR1;                          // packed value of bit timing register 0
    // and bit timing register 1
    std::string m_omStrCNF1;                      // bit timing register 1 information
    std::string m_omStrCNF2;                      // bit timing register 2 information
    std::string m_omStrCNF3;                      // bit timing register 3 information
    std::string m_omStrBTR0;                      // bit timing register 0 information
    std::string m_omStrBTR1;                      // bit timing register 1 information
    std::string m_omStrBaudrate;                  // baudrate information
    std::string m_omStrClock;                     // clock information
    std::string m_omStrSamplePercentage;          // sampling information
    std::string m_omStrSampling;                  // sampling information
    std::string m_omStrWarningLimit;              // Warning limit of CAN Controller
    std::string m_omStrPropagationDelay;          // Propagation Delay
    std::string m_omStrSjw;
    std::string m_omStrAccCodeByte1[defCAN_MSG_IDS]; // acceptance code byte1 information
    std::string m_omStrAccCodeByte2[defCAN_MSG_IDS]; // acceptance code byte2 information
    std::string m_omStrAccCodeByte3[defCAN_MSG_IDS]; // acceptance code byte3 information
    std::string m_omStrAccCodeByte4[defCAN_MSG_IDS]; // acceptance code byte4 information
    std::string m_omStrAccMaskByte1[defCAN_MSG_IDS]; // acceptance mask byte1 information
    std::string m_omStrAccMaskByte2[defCAN_MSG_IDS]; // acceptance mask byte2 information
    std::string m_omStrAccMaskByte3[defCAN_MSG_IDS]; // acceptance mask byte3 information
    std::string m_omStrAccMaskByte4[defCAN_MSG_IDS]; // acceptance mask byte4 information
    std::string m_omHardwareDesc;                 // Hw description which user can
    // differentiate between the channels
    int     m_bAccFilterMode;                     // acceptance filter mode(0: single, 1: Dual)
    unsigned short m_ucControllerMode;            // Controller mode (1: Active, 2: Passive)
    int     m_bSelfReception;
    int     m_bLowSpeed;
    unsigned int  m_unBTL_Cycles;

    /* CAN FD related parameters */
	bool	m_bcanFDEnabled;
	bool	m_bSupportCANFD;
    unsigned int  m_unDataBitRate;
    unsigned int  m_unDataSamplePoint;
    unsigned int  m_unDataBTL_Cycles;
    unsigned int  m_unDataSJW;
    bool            m_bTxDelayCompensationControl;
    unsigned int  m_unTxSecondarySamplePointOffset;
    unsigned short m_bytCanRxMode;
    unsigned short m_bytCanFdRxMode;
    unsigned short m_bytCanFdTxConfig;

    int     m_bDebug;                            // debug mode for channel driver
    int     m_bPassiveMode;                      // passive mode (no bus interaction, acknowledge, etc.)
    std::string  m_omStrLocation;                // location (serial port, ip-address, ...)
    int     m_bHWTimestamps;                     // timestamps from the controllers hardware

    //Filter type: 1. Accept All 2. Reject All 3. Manual setting
    eHW_FILTER_TYPES m_enmHWFilterType[defCAN_MSG_IDS];
};

/**
* This structure is used for sending/reciving messages to/from the CAN network
*/
class STCAN_MSG
{
public:
    /** 11/29 Bit */
    unsigned int  m_unMsgID;

    /** true, for (29 Bit) Frame */
    unsigned char m_ucEXTENDED;

    /** true, for remote request */
    unsigned char m_ucRTR;

    /** Data Length (0..8, 12, 16, 20, 24, 32, 48, 64) */
    unsigned char m_ucDataLen;

    /** Message Length */
    unsigned char m_ucChannel;

    /** Databytes 0..63 */
    unsigned char m_ucData[64];

    /* CAN FD frame */
    bool m_bCANFD;
} ;



struct SCAN_ERR
{
    /** transmit error */
    unsigned char m_ucTxError;

    /** receive error */
    unsigned char m_ucRxError;

    /** channel */
    unsigned char m_ucChannel;
};
/**
* This structure holds Error info
*/
class SERROR_INFO
{
public:
    /**
    * Error Type.
    * Can be ERROR_BUS, ERROR_DEVICE_BUFF_OVERFLOW, ERROR_DRIVER_BUFF_OVERFLOW, ERROR_UNKNOWN
    */
    unsigned char m_ucErrType;

    /** Stores the value of err capture register in case of bus error */
    unsigned char m_ucReg_ErrCap;

    /** transmit error count */
    unsigned char m_ucTxErrCount;

    /** receive error count */
    unsigned char m_ucRxErrCount;

    /** channel */
    unsigned char m_ucChannel;

    /** added for providing Error bit details */
    int m_nSubError;
};


/**
* This structure is used for communicating between Driver and CAN Application
*/
union STDATAINFO
{
    /** The received / transmitted message */
    STCAN_MSG m_sCANMsg;

    /** error info */
    SERROR_INFO m_sErrInfo;
};


/**
* This structure is used for communicating between Driver & CAN Application
*/
class STCANDATA
{
private:
    static int  m_nSortField;

    /** Multiplication factor */
    static int  m_nMFactor;

public:
    /** Type of the message */
    unsigned char    m_ucDataType;

    /** Time stamp, Contains the val returned from QueryPerf..Counter() */
    LARGE_INTEGER    m_lTickCount;
    STDATAINFO       m_uDataInfo;
};




typedef SCONTROLLER_DETAILS*  PSCONTROLLER_DETAILS;
typedef STCAN_MSG*     PSTCAN_MSG;
typedef STATUSMSG*    pSTATUSMSG;
typedef STCANDATA* PSTCANDATA;
typedef SERROR_INFO* PSERROR_INFO;
typedef STDATAINFO* PSTDATAINFO;
typedef CBaseMsgBufFSE<STCANDATA> CBaseCANBufFSE;