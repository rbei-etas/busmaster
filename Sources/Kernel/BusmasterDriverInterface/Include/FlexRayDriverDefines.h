#pragma once
#include <string>
#include "DeviceListInfo.h"
#include "../../ProtocolDefinitions/FlexRayEnums.h"
#include "../../Utilities/BaseMsgBufAll.h"

#define def_MAX_CYCLES                  63

#define def_INVALID_BASE_CYCLE          0XFF
#define def_INVALID_CYCLE_REPETITION    0xFF



#define FLEXRAY_MONITOR_NODE                "FLEXRAY_MONITOR"

#define FLXHDRINFO_FRAMETYPE_STATIC                    0x1
#define FLXHDRINFO_FRAMETYPE_DYNAMIC                   0x2
#define FLXHDRINFO_SYNCFRAME                           0x4
#define FLXHDRINFO_STARTUPFRAME                        0x8
#define FLXHDRINFO_PAYLOADPREAMBLE                     0x10
#define FLXHDRINFO_NULLFRAME                           0x20
#define FLXHDRINFO_RESERVED                            0x40
#define FLXHDRINFO_DATA_UPDATE_LOST                    0x80
#define FLXHDRINFO_DATA_UPDATED                        0x100
#define FLXHDRINFO_TX_SIMULATED                        0x200
#define FLXHDRINFO_PDU_UPDATEBIT_FOUND                 0x400
#define FLXHDRINFO_TX_MODE_MONO_SHOT                   0x800
#define FLXHDRINFO_TX_OFF                              0x1000
#define FLXHDRINFO_SELFRECEPTION                       0x2000
#define FLXHDRINFO_CHANNELID                           0x4000
#define FLXHDRINFO_FRAMETYPE                           0x8000
#define FLXTXMSGINFO_CHANGEABLE_IDENTIFIER             0x10000
#define FLXTXMSGINFO_CHANGEABLE_CYCLENUMBER            0x20000
#define FLXTXMSGINFO_CHANGEABLE_DATALENGTH             0x40000
#define FLXTXMSGINFO_CHANGEABLE_CHANNELID              0x80000
#define FLXTXMSGINFO_CHANGEABLE_FRAMETYPE              0x100000
#define FLXTXMSGINFO_CHANGEABLE_DATA                   0x200000

enum eFlexRayPOCStatus
{
    FLEXRAY_POCS_DEFAULT_CONFIG,
    FLEXRAY_POCS_READY,
    FLEXRAY_POCS_NORMAL_ACTIVE,
    FLEXRAY_POCS_NORMAL_PASSIVE,
    FLEXRAY_POCS_HALT,
    FLEXRAY_POCS_MONITOR_MODE,
    FLEXRAY_POCS_CONFIG,
    FLEXRAY_POCS_WAKEUP_STANDBY,
    FLEXRAY_POCS_WAKEUP_LISTEN,
    FLEXRAY_POCS_WAKEUP_SEND,
    FLEXRAY_POCS_WAKEUP_DETECT,
    FLEXRAY_POCS_STARTUP_PREPARE,
    FLEXRAY_POCS_COLDSTART_LISTEN,
    FLEXRAY_POCS_COLDSTART_COLLISION_RESOLUTION,
    FLEXRAY_POCS_COLDSTART_CONSISTENCY_CHECK,
    FLEXRAY_POCS_COLDSTART_GAP,
    FLEXRAY_POCS_COLDSTART_JOIN,
    FLEXRAY_POCS_INTEGRATION_COLDSTART_CHECK,
    FLEXRAY_POCS_INTEGRATION_LISTEN,
    FLEXRAY_POCS_INTEGRATION_CONSISTENCY_CHECK,
    FLEXRAY_POCS_INITIALIZE_SCHEDULE,
    FLEXRAY_POCS_ABORT_STARTUP,
    FLEXRAY_POCS_STARTUP_SUCCESS,
    FLEXRAY_POCS_END
};

enum eFLXMESSAGETYPE
{
    FLXMSGTYPE_UNKNOWN,
    FLXMSGTYPE_STATUS,
    FLXMSGTYPE_DATA,
    FLXMSGTYPE_STARTCYCLE,
    FLXMSGTYPE_POC,
    FLXMSGTYPE_ERROR,
    FLEXMSGTYPE_PDU // Added by bhaskar
};

/** declaration of FlexRay data message */
class s_FLXDATAMSG
{
public:
    /** FlexRay Identifier (0x001-0x7FF {11 bit}) */
    unsigned short m_nSlotID;

    /** Cycle Number (0-63 {6 bit}) */
    unsigned short m_ucCycleNumber;

    unsigned short m_ucRepetition;

    unsigned short m_nBaseCycle;

    /** Cluster */
    unsigned short m_nCluster;

    /** A or B or AB */
    eFlexRayChannel m_eChannel;

    /**  Data length (number of data words) (0-127 {7 bit}) */
    unsigned short m_nDLC;

    /*Header CRC (0-2047{11 bit})*/
    unsigned short m_nHeaderCrc;

    /** Flagfield of Header information (FLXHDRINFO_???) */
    unsigned long   m_lHeaderInfoFlags;

    bool m_bIsRxMsg;

    /**  FlexRay Data */
    unsigned char m_ucData[254];

};
typedef s_FLXDATAMSG* ps_FLXDATAMSG;



class s_FLXPDUDATAMSG
{
public:
    /** FlexRay Identifier (0x001-0x7FF {11 bit}) */
    unsigned short m_nSlotID;

    /** Cycle Number (0-63 {6 bit}) */
    unsigned short m_ucCycleNumber;

    unsigned short m_ucRepetition;

    unsigned short m_nBaseCycle;

    /** Cluster */
    unsigned short m_nCluster;

    /** A or B or AB */
    //eFlexRayChannel m_eChannel2;
    eFlexRayChannel m_eChannel;

    /**  Data length (number of data words) (0-127 {7 bit}) */
    unsigned short m_nDLC;

    /*Header CRC (0-2047{11 bit})*/
    unsigned short m_nHeaderCrc;

    /** Flagfield of Header information (FLXHDRINFO_???) */
    unsigned long   m_lHeaderInfoFlags;

    bool m_bIsRxMsg;

    int m_nStartBit;

    int m_nUpdateBitPos;

    /**  FlexRay Data */
    unsigned char m_ucData[254];

    char m_ucDescription[254];

};
typedef s_FLXPDUDATAMSG* ps_FLXPDUDATAMSG;


class VERSIONINFO
{
public:
    std::string m_acDIL;
    std::string m_acController;
    std::string m_acDriver;
};

class s_FLXControllerCapabilities
{
public:
    int m_nMaximumTxBuffers;
    bool m_bSRSupport;          //Self Receiption
};

class FlexRayControllerParams
{
public:
    std::string m_strInterFaceName;
    unsigned int m_nBaudRate;
    std::string m_strProtocolVersion;
};

class s_FLXSTARTCYCLE
{
public:
    int m_nCycleNumber;     //Present only cycle number.
};

class s_FLXPOCSTATUS
{
public:
    eFlexRayPOCStatus m_oeFlexRayStatus;
};

//----------------------------------------------------------------------------
// union for FlexRay data and status message types
//----------------------------------------------------------------------------
class s_FLXMSG
{
public:
    eFLXMESSAGETYPE m_eMessageType;            // Identifies the message type
    // One of the following message types:
    // FLXMSGTYPE_UNKNOWN := 0 -> Message invalid
    // FLXMSGTYPE_STATUS  := 1 -> stcStatusMsg is valid
    // FLXMSGTYPE_DATA    := 2 -> stcDataMsg is valid
    unsigned __int64 m_unTimestamp;
    int             m_nChannel;

    /** A or B or AB */
    eFlexRayChannel m_eChannel;

    union
    {
        //s_FLXSTATUSMSG stcStatusMsg;          // Status message structure
        s_FLXDATAMSG   stcDataMsg;            // Data message structure
        s_FLXSTARTCYCLE stcStartCycle;
        s_FLXPOCSTATUS stcPocStatus;
        s_FLXPDUDATAMSG stcPDUMsg; //Added by bhaskar
    };
};
typedef s_FLXMSG* ps_FLXMSG;

typedef CBaseMsgBufFSE<s_FLXMSG> CBaseFLEXBufFSE;