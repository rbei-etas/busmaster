//////////////////////////////////////////////////////////////////////////
// IXXAT Automation GmbH
//////////////////////////////////////////////////////////////////////////
/**
  Common CAN constants, data types and macros.

  @file "cantype.h"

  @note
    This file is shared between user and kernel mode components. Request a
    kernel mode developer before changing the contents of this file.
*/
//////////////////////////////////////////////////////////////////////////
// (C) 2002-2011 IXXAT Automation GmbH, all rights reserved
//////////////////////////////////////////////////////////////////////////

#ifndef CANTYPE_H
#define CANTYPE_H

//////////////////////////////////////////////////////////////////////////
//  include files
//////////////////////////////////////////////////////////////////////////
#ifdef VCIUSERAPP
#include <pshpack1.h>
#else
#include <XATpshpack1.h>
#endif

//////////////////////////////////////////////////////////////////////////
//  constants, macros, types
//////////////////////////////////////////////////////////////////////////

/*****************************************************************************
 * controller types
 ****************************************************************************/

#define CAN_CTRL_UNKNOWN     0    // unknown
#define CAN_CTRL_82527       1    // Intel 82527
#define CAN_CTRL_82C200      2    // Intel 82C200
#define CAN_CTRL_81C90       3    // Intel 81C90
#define CAN_CTRL_82C90       3    // wrong older definition of Intel 81C90
#define CAN_CTRL_81C92       4    // Intel 81C92
#define CAN_CTRL_82C92       4    // wrong older definition of Intel 81C92
#define CAN_CTRL_SJA1000     5    // Philips SJA 1000
#define CAN_CTRL_82C900      6    // Infinion 82C900 (TwinCAN)
#define CAN_CTRL_TOUCAN      7    // Motorola TOUCAN
#define CAN_CTRL_MSCAN       8    // Freescale Star12 MSCAN
#define CAN_CTRL_FLEXCAN     9    // Freescale Coldfire FLEXCAN
#define CAN_CTRL_IFI        10    // IFI CAN ( ALTERA FPGA CAN )

#define CAN_CTRL_MAXVAL      255  // maximum value for controller type


/*****************************************************************************
 * supported features
 ****************************************************************************/

#define CAN_FEATURE_STDOREXT   0x0001  // 11 OR 29 bit (exclusive)
#define CAN_FEATURE_STDANDEXT  0x0002  // 11 AND 29 bit (simultaneous)
#define CAN_FEATURE_RMTFRAME   0x0004  // reception of remote frames
#define CAN_FEATURE_ERRFRAME   0x0008  // reception of error frames
#define CAN_FEATURE_BUSLOAD    0x0010  // bus load measurement
#define CAN_FEATURE_IDFILTER   0x0020  // exact message filter
#define CAN_FEATURE_LISTONLY   0x0040  // listen only mode
#define CAN_FEATURE_SCHEDULER  0x0080  // cyclic message scheduler
#define CAN_FEATURE_GENERRFRM  0x0100  // error frame generation
#define CAN_FEATURE_DELAYEDTX  0x0200  // delayed message transmitter
#define CAN_FEATURE_SSM        0x0400  // single shot mode
#define CAN_FEATURE_HI_PRIO    0x0800  // high priority message


/*****************************************************************************
 * bus coupling types
 ****************************************************************************/

#define CAN_BUSC_UNDEFINED   0x0000    // undefined
#define CAN_BUSC_LOWSPEED    0x0001    // low speed coupling 
#define CAN_BUSC_HIGHSPEED   0x0002    // high speed coupling


/*****************************************************************************
 * predefined CiA bit rates
 ****************************************************************************/

#define CAN_BT0_10KB     0x31
#define CAN_BT1_10KB     0x1C
#define CAN_BT0_20KB     0x18
#define CAN_BT1_20KB     0x1C
#define CAN_BT0_50KB     0x09
#define CAN_BT1_50KB     0x1C
#define CAN_BT0_100KB    0x04
#define CAN_BT1_100KB    0x1C
#define CAN_BT0_125KB    0x03
#define CAN_BT1_125KB    0x1C
#define CAN_BT0_250KB    0x01
#define CAN_BT1_250KB    0x1C
#define CAN_BT0_500KB    0x00
#define CAN_BT1_500KB    0x1C
#define CAN_BT0_800KB    0x00
#define CAN_BT1_800KB    0x16
#define CAN_BT0_1000KB   0x00
#define CAN_BT1_1000KB   0x14

#define CAN_BT01_10KB    0x31,0x1C
#define CAN_BT01_20KB    0x18,0x1C
#define CAN_BT01_50KB    0x09,0x1C
#define CAN_BT01_100KB   0x04,0x1C
#define CAN_BT01_125KB   0x03,0x1C
#define CAN_BT01_250KB   0x01,0x1C
#define CAN_BT01_500KB   0x00,0x1C
#define CAN_BT01_800KB   0x00,0x16
#define CAN_BT01_1000KB  0x00,0x14


/*****************************************************************************
 * controller operating modes
 ****************************************************************************/

#define CAN_OPMODE_UNDEFINED 0x00 // undefined
#define CAN_OPMODE_STANDARD  0x01 // reception of 11-bit id messages
#define CAN_OPMODE_EXTENDED  0x02 // reception of 29-bit id messages
#define CAN_OPMODE_ERRFRAME  0x04 // enable reception of error frames
#define CAN_OPMODE_LISTONLY  0x08 // listen only mode (TX passive)
#define CAN_OPMODE_LOWSPEED  0x10 // use low speed bus interface


/*****************************************************************************
 * controller status
 ****************************************************************************/

#define CAN_STATUS_TXPEND    0x01 // transmission pending
#define CAN_STATUS_OVRRUN    0x02 // data overrun occurred
#define CAN_STATUS_ERRLIM    0x04 // error warning limit exceeded
#define CAN_STATUS_BUSOFF    0x08 // bus off status
#define CAN_STATUS_ININIT    0x10 // init mode active


/*****************************************************************************
 * filter selection
 ****************************************************************************/

#define CAN_FILTER_STD  1 // select standard filter (11-bit)
#define CAN_FILTER_EXT  2 // select extended filter (29-bit)


/*****************************************************************************
 * acceptance filter settings
 ****************************************************************************/

//
// acceptance code and mask to accept all CAN IDs
//

#define CAN_ACC_MASK_ALL          0x00000000
#define CAN_ACC_CODE_ALL          0x00000000

//
// acceptance code and mask to reject all CAN IDs
//

#define CAN_ACC_MASK_NONE         0xFFFFFFFF
#define CAN_ACC_CODE_NONE         0x80000000


/*****************************************************************************
 * filter list sizes
 ****************************************************************************/

#define CAN_FILTER_SIZE_STD       4096 // size of 11-bit ID filter list
#define CAN_FILTER_SIZE_EXT       4096 // size of 29-bit ID filter list


/*****************************************************************************
 * increment modes for cyclic transmit messages
 ****************************************************************************/

#define CAN_CTXMSG_INC_NO         0x00 // no increment
#define CAN_CTXMSG_INC_ID         0x01 // increment CAN identifier
#define CAN_CTXMSG_INC_8          0x02 // increment 8-bit data field
#define CAN_CTXMSG_INC_16         0x03 // increment 16-bit data field

/*****************************************************************************
 * status of cyclic transmit task
 ****************************************************************************/

#define CAN_CTXTSK_STAT_STOPPED   0x00 // cyclic transmit task stopped
#define CAN_CTXTSK_STAT_RUNNING   0x01 // cyclic transmit task running


/*****************************************************************************
 * status of cyclic transmit messages
 ****************************************************************************/

#define CAN_CTXMSG_STAT_EMPTY     0x00 // the message entry is empty
#define CAN_CTXMSG_STAT_BUSY      0x01 // the processing of the
// message is in progress
#define CAN_CTXMSG_STAT_DONE      0x02 // the processing of the
// message has completed


/*****************************************************************************
 * CAN controller bit timing table
 ****************************************************************************/

#define CAN_BTR_TABEL_SIZE 64  // size of bus timing register table

typedef struct _CANBTRTABLE
{
    UINT8 bCount;     // Number of values within the BTR tables
    UINT8 bIndex;     // Index of the selected value within the BTR table
    UINT8 abBtr0[64]; // Test values for bus timing register (BTR) 0
    UINT8 abBtr1[64]; // Test values for bus timing register (BTR) 1
} CANBTRTABLE, *PCANBTRTABLE;


/*****************************************************************************
 * CAN controller initialization structure
 ****************************************************************************/

typedef struct _CANINITLINE
{
    UINT8 bOpMode;   // CAN operating mode (see CAN_OPMODE_ constants)
    UINT8 bReserved; // reserved set to 0
    UINT8 bBtReg0;   // value for bus timing register 0
    // according to SJA1000/16MHz
    UINT8 bBtReg1;   // value for bus timing register 1
    // according to SJA1000/16MHz
} CANINITLINE, *PCANINITLINE;


/*****************************************************************************
 * CAN capabilities
 ****************************************************************************/

typedef struct _CANCAPABILITIES
{
    UINT16 wCtrlType;     // Type of CAN controller (see CAN_CTRL_ const)
    UINT16 wBusCoupling;  // Type of Bus coupling (see CAN_BUSC_ const)
    UINT32 dwFeatures;    // supported features (see CAN_FEATURE_ constants)
    UINT32 dwClockFreq;   // clock frequency of the primary counter in Hz
    UINT32 dwTscDivisor;  // divisor for the message time stamp counter
    UINT32 dwCmsDivisor;  // divisor for the cyclic message scheduler
    UINT32 dwCmsMaxTicks; // maximum tick count value of the cyclic message
    // scheduler
    UINT32 dwDtxDivisor;  // divisor for the delayed message transmitter
    UINT32 dwDtxMaxTicks; // maximum tick count value of the delayed
    // message transmitter
} CANCAPABILITIES, *PCANCAPABILITIES;


/*****************************************************************************
 * CAN controller status information structure
 ****************************************************************************/

typedef struct _CANLINESTATUS
{
    UINT8  bOpMode;  // current CAN operating mode
    UINT8  bBtReg0;  // current bus timing register 0 value
    UINT8  bBtReg1;  // current bus timing register 1 value
    UINT8  bBusLoad; // average bus load in percent (0..100)
    UINT32 dwStatus; // status of the CAN controller (see CAN_STATUS_)
} CANLINESTATUS, *PCANLINESTATUS;


/*****************************************************************************
 * CAN message channel status information structure
 ****************************************************************************/

typedef struct _CANCHANSTATUS
{
    CANLINESTATUS sLineStatus; // current CAN line status
    UINT32        fActivated;  // TRUE if the channel is activated
    UINT32        fRxOverrun;  // TRUE if receive FIFO overrun occurs
    UINT8         bRxFifoLoad; // receive FIFO load in percent (0..100)
    UINT8         bTxFifoLoad; // transmit FIFO load in percent (0..100)
} CANCHANSTATUS, *PCANCHANSTATUS;


/*****************************************************************************
 * status of the cyclic message scheduler
 ****************************************************************************/

#define CAN_MAX_CTX_MSGS  16 // maximum possible cyclic transmit messages

typedef struct _CANSCHEDULERSTATUS
{
    UINT8 bTaskStat;     // status of cyclic transmit task
    UINT8 abMsgStat[16]; // status of all cyclic transmit messages
} CANSCHEDULERSTATUS, *PCANSCHEDULERSTATUS;


/*****************************************************************************
 * CAN message information
 ****************************************************************************/

/*lint -save -e46 (field type should be int) */
typedef union _CANMSGINFO
{
    struct
    {
        UINT8  bType;     // type (see CAN_MSGTYPE_ constants)
        UINT8  bAddFlags; // additional flags
        UINT8  bFlags;    // flags (see CAN_MSGFLAGS_ constants)
        UINT8  bAccept;   // accept code (see CAN_ACCEPT_ constants)
    } Bytes;

    struct
    {
        UINT32 type: 8;   // message type
        UINT32 ssm : 1;   // single shot mode
        UINT32 hi  : 2;   // high priority message
        UINT32 res : 5;   // reserved
        UINT32 dlc : 4;   // data length code
        UINT32 ovr : 1;   // possible data overrun
        UINT32 srr : 1;   // self reception request
        UINT32 rtr : 1;   // remote transmission request
        UINT32 ext : 1;   // extended frame format (0=standard, 1=extended)
        UINT32 afc : 8;   // acceptance filter code
    } Bits;

} CANMSGINFO, *PCANMSGINFO;
/*lint -restore */


//
// message types (used by <CANMSGINFO.Bytes.bType>)
//

#define CAN_MSGTYPE_DATA     0 // data frame
#define CAN_MSGTYPE_INFO     1 // info frame
#define CAN_MSGTYPE_ERROR    2 // error frame
#define CAN_MSGTYPE_STATUS   3 // status frame
#define CAN_MSGTYPE_WAKEUP   4 // wakeup frame
#define CAN_MSGTYPE_TIMEOVR  5 // timer overrun
#define CAN_MSGTYPE_TIMERST  6 // timer reset

//
// additional message information flags (used by <CANMSGINFO.Bytes.bAddFlags>)
//
#define CAN_MSGADDFLAGS_SSM  0x01 // single shot mode
#define CAN_MSGADDFLAGS_HI   0x06 // high priority message

//
// message information flags (used by <CANMSGINFO.Bytes.bFlags>)
//

#define CAN_MSGFLAGS_DLC     0x0F // data length code
#define CAN_MSGFLAGS_OVR     0x10 // data overrun flag
#define CAN_MSGFLAGS_SRR     0x20 // self reception request
#define CAN_MSGFLAGS_RTR     0x40 // remote transmission request
#define CAN_MSGFLAGS_EXT     0x80 // frame format (0=11-bit, 1=29-bit)

#define CAN_MAKE_MSGFLAGS(dlc,ovr,srr,rtr,ext) (UINT8) \
    ( (((ext) & 0x01) << 7) | (((rtr) & 0x01) << 6) |   \
      (((srr) & 0x01) << 5) | (((ovr) & 0x01) << 4) |   \
      ((dlc) & 0x0F) )

//
// acceptance filter status (used by <CANMSGINFO.Bytes.bAccept>)
//

#define CAN_ACCEPT_REJECT    0x00 // message not accepted
#define CAN_ACCEPT_ALWAYS    0xFF // message always accepted
#define CAN_ACCEPT_FILTER_1  0x01 // message accepted by 1. filter
#define CAN_ACCEPT_FILTER_2  0x02 // message accepted by 2. filter

//
// Information supplied in the abData[0] field of info frames
// (CANMSGINFO.Bytes.bType = CAN_MSGTYPE_INFO).
//

#define CAN_INFO_START       1 // start of CAN controller
#define CAN_INFO_STOP        2 // stop of CAN controller
#define CAN_INFO_RESET       3 // reset of CAN controller

//
// Error information supplied in the abData[0] field of error frames
// (CANMSGINFO.Bytes.bType = CAN_MSGTYPE_ERROR).
//

#define CAN_ERROR_STUFF      1 // stuff error
#define CAN_ERROR_FORM       2 // form error
#define CAN_ERROR_ACK        3 // acknowledgment error
#define CAN_ERROR_BIT        4 // bit error
#define CAN_ERROR_CRC        6 // CRC error
#define CAN_ERROR_OTHER      7 // other (unspecified) error


/*****************************************************************************
 * CAN message structure
 ****************************************************************************/

typedef struct _CANMSG
{
    UINT32     dwTime;    // time stamp for receive message
    UINT32     dwMsgId;   // CAN message identifier (INTEL format)
    CANMSGINFO uMsgInfo;  // message information (bit field)
    UINT8      abData[8]; // message data
} CANMSG, *PCANMSG;     // size is 20 bytes


/*****************************************************************************
 * cyclic CAN transmit message
 ****************************************************************************/

typedef struct _CANCYCLICTXMSG
{
    UINT16     wCycleTime; // cycle time for the message in ticks
    UINT8      bIncrMode;  // auto increment mode (see CAN_CTXMSG_INC_ const)
    UINT8      bByteIndex; // index of the byte within abData[] to increment
    UINT32     dwMsgId;    // message identifier (INTEL format)
    CANMSGINFO uMsgInfo;   // message information (bit field)
    UINT8      abData[8];  // message data
} CANCYCLICTXMSG, *PCANCYCLICTXMSG;

#ifdef VCIUSERAPP
#include <poppack.h>
#else
#include <XATpoppack.h>
#endif

#endif //_CANTYPE_H_
