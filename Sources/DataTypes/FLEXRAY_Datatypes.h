
#pragma once

#include <afxmt.h>
#include "DataTypes_StdAfx.h"
#include "Base_FlexRay_Buffer.h"
#include "FEALData.h"

typedef enum eTXWNDDETAILS
{
    TX_MSG_BLOCK_COUNT,    // Message Block count
    TX_WND_SPLITTER_DATA,
    TX_WINDOW_PLACEMENT,
};

enum
{
    FLEXRAY_DRIVER_STUB         = 0x00,
    FLEXRAY_DRIVER_ETAS_BOA,
    FLEXRAY_DRIVER_GIGATRONIK_FLEXI,
    FLEXRAY_DIL_TOTAL,          // Its value must be <= MAX_DILS
    FLEXRAY_DAL_NONE            = ~0x0 // none selected
};

typedef enum FLEX_CHANNEL
{
    NO_FLEX_CHANNEL = 0x00,                   //!< No FlexRay channel is selected.
    FLEX_CHANNEL_A  = 0x01,                   //!< Only FlexRay channel A is selected.
    FLEX_CHANNEL_B  = 0x02,                   //!< Only FlexRay channel B is selected.
    FLEX_CHANNEL_AB = FLEX_CHANNEL_A | FLEX_CHANNEL_B //!< FlexRay channel A + B are selected
} FLEX_CHANNEL;

class CTxFlexRayDataStore;
class CTxLINDataStore;
typedef struct tagSCALCEXECTIMETHREAD
{
    bool                    m_bThreadStop;
    CWinThread*             m_pomThreadPtr;
    CEvent                  m_omExitThreadEvent;
    CTxFlexRayDataStore*    m_pTxFlexRayDataStore;
    CTxLINDataStore*        m_pTxLINDataStore;
    tagSCALCEXECTIMETHREAD()
    {
        m_bThreadStop  = FALSE;
        m_pomThreadPtr = nullptr;
        m_pTxLINDataStore = nullptr;
    }
} SCALCEXECTIMETHREAD,*PSCALCEXECTIMETHREAD;
typedef enum MSGBUFTYPE
{
    MSGBUF_NONE,
    MSGBUF_RX,
    MSGBUF_TX,
    MSGBUF_FIFO,
} MSGBUFTYPE;

typedef enum FLEXRAY_TXMODE
{
    FLX_TXMODE_SINGLE_SHOT,
    FLX_TXMODE_CONTINOUS,
} FLEXRAY_TXMODE;

#define  MAX_CHAR_SHORT       128
#define  MAX_CHAR_LONG        512
#define  MAX_CONFIG_FILES     016
#define defNO_OF_DEVICE         16
#define FLEXRAY_MONITER_NODE                "FLEXRAY_MONITER"

typedef struct
{
    /* Out of the 16 configuration files, first 15 will be loaded with imported FIBEX files,
       last array element will be used for manually selected configuration file */
    std::string         m_acConfigFileName[MAX_CONFIG_FILES];
    ABS_CLUSTER_INFO    m_asClusterinfo;
    UINT                m_nFilesCount;
    UINT                m_nSelectedIndex;
} FLEXRAY_CONFIG_FILES;

typedef struct tagFlexRayHwInterface
{
    DWORD    m_dwIdInterface;
    DWORD    m_dwVendor;
    char     m_acNameInterface[MAX_CHAR_SHORT];
    char     m_acDescription[MAX_CHAR_LONG];
    char     m_acDeviceName[MAX_CHAR_SHORT];
} FLEXRAY_INTERFACE_HW;

typedef FLEXRAY_INTERFACE_HW FLEXRAY_INTERFACE_HW_LIST[defNO_OF_DEVICE];

typedef struct tagFlexRayVersionInfo
{
    char     m_acDIL[MAX_CHAR_LONG];
    char     m_acController[MAX_CHAR_LONG];
    char     m_acDriver[MAX_CHAR_LONG];
} FLEXRAY_VERSIONINFO;


#define     MAX_DILS                16
#define     MAX_DILNAME             32

typedef struct
{
    std::string  m_acName;
    DWORD   m_dwDriverID;
    UINT    m_ResourceID;
} FLEXRAY_DILINFO;

typedef FLEXRAY_DILINFO FLEXRAY_DILLIST[MAX_DILS];

typedef struct tagMSGBUFCFG_FIFO
{
    //! Defines the acceptance filter used for frame id rejection.
    UINT FrameIdFilter : 11;
    //! Specifies the relevant bits used for rejection filtering.
    UINT RejectionMask : 11;
    //! Defines the maximum number of 2-byte payload words received.
    UINT PayloadLengthConfigured : 7;
    //! Set this flag to 1 to reject all received static frames of the FIFO.
    UINT RejectInStaticSegment : 1;
    //! Set this flag to 1 to reject all received null frames of the FIFO.
    UINT RejectNullFrames : 1;
} s_MSGBUFCFG_FIFO;

typedef struct tagMSGBUFCFG_RX
{
    UINT FrameId : 11;
    UINT PayloadLengthConfigured : 7;
    UINT PayloadLengthMax : 7;
    UINT RxAcknowledgeEnable : 1;
} s_MSGBUFCFG_RX;

//! This structure specifies the configuration of a transmit message buffer.
//! This buffer type is used to transmit a frame on a specific communication
//! slot.
typedef struct tagMSGBUFCFG_TX
{
    UINT FrameId : 11;
    UINT PayloadLength : 7;
    UINT PayloadLengthMax : 7;
    UINT PayloadPreambleIndicator : 1;
    UINT SyncFrameIndicator : 1;
    UINT StartupFrameIndicator : 1;
    UINT TxAcknowledgeEnable : 1;
    FLEXRAY_TXMODE TransmissionMode;
    UINT TxAcknowledgeShowNullFrames : 1;
    UINT TxAcknowledgeShowPayload : 1;
} s_MSGBUFCFG_TX;

typedef struct tagMSGBUF
{

    //! Defines the buffer type (FIFO, receive or transmit buffer)
    MSGBUFTYPE Type;
    //! dynamic frame, only fcChannelA or fcChannelB is allowed.
    FLEX_CHANNEL ChannelFilter;
    UINT CycleCounterFilter : 7;
    union
    {
        s_MSGBUFCFG_FIFO Fifo;
        s_MSGBUFCFG_RX   Rx;
        s_MSGBUFCFG_TX   Tx;
    };
} s_MSGBUF;


class CFlxTxMsgBuffer
{
public:
    // Data members
    DWORD m_dwTxMsgCount;
    s_FLXTXMSG* m_psFlxTxMsgList;

    // Functions
    CFlxTxMsgBuffer();
    ~CFlxTxMsgBuffer();

    void vResetAll(void);
};

#define INITIALISE_DATA(Data)   memset(&Data, 0, sizeof(Data))
#define INITIALISE_ARRAY(Array) memset(Array, 0, sizeof(Array))
