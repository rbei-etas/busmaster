/*****************************************************************************
 HMS Technology Center Ravensburg GmbH
******************************************************************************

 File    : CANTYPE.H
 Summary : Common CAN constants, data types and macros.

 Date    : 2003-04-14
 Author  : Hartmut Heim

 Compiler: MSVC

******************************************************************************
 all rights reserved
*****************************************************************************/

#ifndef _CANTYPE_H_
#define _CANTYPE_H_

#include <stdtype.h>
#include <pshpack1.h>

/*****************************************************************************
 * predefined CiA bit rates
 ****************************************************************************/

//
// register values for SJA1000 (16MHz clock)
//
#define CAN_BT0_5KB      0x3F  // SJW=0, BRP=63
#define CAN_BT1_5KB      0x7F  // SAM=0, TSEG2=8, TSEG1=16
#define CAN_BT0_10KB     0x31  // SJW=0, BRP=49
#define CAN_BT1_10KB     0x1C  // SAM=0, TSEG2=2, TSEG1=13
#define CAN_BT0_20KB     0x18  // SJW=0, BRP=24
#define CAN_BT1_20KB     0x1C  // SAM=0, TSEG2=2, TSEG1=13
#define CAN_BT0_50KB     0x09  // SJW=0, BRP=9
#define CAN_BT1_50KB     0x1C  // SAM=0, TSEG2=2, TSEG1=13
#define CAN_BT0_100KB    0x04  // SJW=0, BRP=4
#define CAN_BT1_100KB    0x1C  // SAM=0, TSEG2=2, TSEG1=13
#define CAN_BT0_125KB    0x03  // SJW=0, BRP=3
#define CAN_BT1_125KB    0x1C  // SAM=0, TSEG2=2, TSEG1=13
#define CAN_BT0_250KB    0x01  // SJW=0, BRP=1
#define CAN_BT1_250KB    0x1C  // SAM=0, TSEG2=2, TSEG1=13
#define CAN_BT0_500KB    0x00  // SJW=0, BRP=0
#define CAN_BT1_500KB    0x1C  // SAM=0, TSEG2=2, TSEG1=13
#define CAN_BT0_800KB    0x00  // SJW=0, BRP=0
#define CAN_BT1_800KB    0x16  // SAM=0, TSEG2=2, TSEG1=7
#define CAN_BT0_1000KB   0x00  // SJW=0, BRP=0
#define CAN_BT1_1000KB   0x14  // SAM=0, TSEG2=2, TSEG1=5

#define CAN_BT01_5KB     0x3F,0x7F
#define CAN_BT01_10KB    0x31,0x1C
#define CAN_BT01_20KB    0x18,0x1C
#define CAN_BT01_50KB    0x09,0x1C
#define CAN_BT01_100KB   0x04,0x1C
#define CAN_BT01_125KB   0x03,0x1C
#define CAN_BT01_250KB   0x01,0x1C
#define CAN_BT01_500KB   0x00,0x1C
#define CAN_BT01_800KB   0x00,0x16
#define CAN_BT01_1000KB  0x00,0x14


//
// bit timing parameters
//
#define CAN_BTP_EMPTY  {0,      0, 0,0,0,0}
#define CAN_BTP_5KB    {0,   5000,17,8,1,0}
#define CAN_BTP_10KB   {0,  10000,14,2,1,0}
#define CAN_BTP_20KB   {0,  20000,14,2,1,0}
#define CAN_BTP_50KB   {0,  50000,14,2,1,0}
#define CAN_BTP_100KB  {0, 100000,14,2,1,0}
#define CAN_BTP_125KB  {0, 125000,14,2,1,0}
#define CAN_BTP_250KB  {0, 250000,14,2,1,0}
#define CAN_BTP_500KB  {0, 500000,14,2,1,0}
#define CAN_BTP_800KB  {0, 800000, 8,2,1,0}
#define CAN_BTP_1000KB {0,1000000, 6,2,1,0}


/*****************************************************************************
 * filter selection
 ****************************************************************************/

#define CAN_FILTER_STD  1    // select standard filter (11-bit)
#define CAN_FILTER_EXT  2    // select extended filter (29-bit)


/*****************************************************************************
 * filter mode
 ****************************************************************************/

#define CAN_FILTER_VOID 0x00 // invalid or unknown filter mode
                             // (do not use for initialization)
#define CAN_FILTER_LOCK 0x01 // lock filter (inhibit all IDs)
#define CAN_FILTER_PASS 0x02 // bypass filter (pass all IDs)
#define CAN_FILTER_INCL 0x03 // inclusive filtering (pass registered IDs)
#define CAN_FILTER_EXCL 0x04 // exclusive filtering (inhibit registered IDs)

//
// additional filter mode flag for ICanChannel2 initialization
//
#define CAN_FILTER_SRRA 0x80 // pass self-rec messages from all channels


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
 * filter list sizes (defaults)
 ****************************************************************************/

#define CAN_FILTER_SIZE_STD       2048 // size of 11-bit ID filter list
#define CAN_FILTER_SIZE_EXT       2048 // size of 29-bit ID filter list


/*****************************************************************************
 * CAN controller bit timing register
 ****************************************************************************/

//
//*** normal mode *******************************
//
// <------- Tbit --------->
// +------+-------+-------+
// | SYNC | TSEG1 | TSEG2 |
// +------+---- --+-------+
// |     wTS1     | wTS2  |
// +--------------+-------+
//                |
//                +-> Sample Point
//
//*** raw mode **********************************
//
// <-------- Tbit -------->
// +------+-------+-------+
// | SYNC | TSEG1 | TSEG2 |
// +------+-------+-------+
// |  1   |  wTS1 |  wTS2 |
// +------+-------+-------+
//                |
//                +-> Sample Point
//
// SYNC  := Re-Synchronization Segment
// TSEG1 := Time Segment 1
// TSEG2 := Time Segment 2
//
//

//
// bit timing parameter
//
typedef struct _CANBTP
{
  UINT32 dwMode;  // timing mode (see CAN_BTMODE_ const)
  UINT32 dwBPS;   // bits per second or prescaler (see CAN_BTMODE_RAW)
  UINT16 wTS1;    // length of time segment 1 in quantas
  UINT16 wTS2;    // length of time segment 2 in quantas
  UINT16 wSJW;    // re-synchronization jump width in quantas
  UINT16 wTDO;    // transceiver delay compensation offset in quantas
                  // (0 = disabled)
} CANBTP, *PCANBTP;

#define CAN_BTMODE_RAW  0x00000001 // raw mode
#define CAN_BTMODE_TSM  0x00000002 // triple sampling mode

//
// Remarks:
// If the bit CAN_BTMODE_RAW is set within CANBTP.dwMode the other fields
// of CANBTP contain controller specific bit timing parameters. The possible
// range for this controller specific values can be determined by invocation
// of ICanSocket2::GetCapabilities. The fields sSdrRangeMin, sSdrRangeMax,
// sFdrRangeMin and sFdrRangeMax of the returned CANCAPABILITIES2 structure
// contains the lower and upper boundaries of the values.
//

//------------------------------------------------------------------------
// predefined CiA bit rates
//------------------------------------------------------------------------

#ifdef __cplusplus
const CANBTP CAN_BITRATE_NONE   = CAN_BTP_EMPTY;
const CANBTP CAN_BITRATE_5KB    = CAN_BTP_5KB;
const CANBTP CAN_BITRATE_10KB   = CAN_BTP_10KB;
const CANBTP CAN_BITRATE_20KB   = CAN_BTP_20KB;
const CANBTP CAN_BITRATE_50KB   = CAN_BTP_50KB;
const CANBTP CAN_BITRATE_100KB  = CAN_BTP_100KB;
const CANBTP CAN_BITRATE_125KB  = CAN_BTP_125KB;
const CANBTP CAN_BITRATE_250KB  = CAN_BTP_250KB;
const CANBTP CAN_BITRATE_500KB  = CAN_BTP_500KB;
const CANBTP CAN_BITRATE_800KB  = CAN_BTP_800KB;
const CANBTP CAN_BITRATE_1000KB = CAN_BTP_1000KB;
#else
#ifdef INIT_CAN_BITRATES
const CANBTP CAN_BITRATE_NONE   = CAN_BTP_EMPTY;
const CANBTP CAN_BITRATE_5KB    = CAN_BTP_5KB;
const CANBTP CAN_BITRATE_10KB   = CAN_BTP_10KB;
const CANBTP CAN_BITRATE_20KB   = CAN_BTP_20KB;
const CANBTP CAN_BITRATE_50KB   = CAN_BTP_50KB;
const CANBTP CAN_BITRATE_100KB  = CAN_BTP_100KB;
const CANBTP CAN_BITRATE_125KB  = CAN_BTP_125KB;
const CANBTP CAN_BITRATE_250KB  = CAN_BTP_250KB;
const CANBTP CAN_BITRATE_500KB  = CAN_BTP_500KB;
const CANBTP CAN_BITRATE_800KB  = CAN_BTP_800KB;
const CANBTP CAN_BITRATE_1000KB = CAN_BTP_1000KB;
#else
extern const CANBTP CAN_BITRATE_NONE;
extern const CANBTP CAN_BITRATE_5KB;
extern const CANBTP CAN_BITRATE_10KB;
extern const CANBTP CAN_BITRATE_20KB;
extern const CANBTP CAN_BITRATE_50KB;
extern const CANBTP CAN_BITRATE_100KB;
extern const CANBTP CAN_BITRATE_125KB;
extern const CANBTP CAN_BITRATE_250KB;
extern const CANBTP CAN_BITRATE_500KB;
extern const CANBTP CAN_BITRATE_800KB;
extern const CANBTP CAN_BITRATE_1000KB;
#endif
#endif


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


#define CAN_BTP_TABEL_SIZE 64  // size of bus timing parameter table

typedef struct _CANBTPTABLE
{
  UINT8    bCount;    // Number of values within the BTR table
  UINT8    bIndex;    // Index of the selected value within the BTR table
  struct
  {
    CANBTP sSdr;      // standard bit rate timing
    CANBTP sFdr;      // fast data bit rate timing
  }        asBTP[64]; // array of bit rate test values
} CANBTPTABLE, *PCANBTPTABLE;


/*****************************************************************************
 * CAN controller capabilities
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


typedef struct _CANCAPABILITIES2
{
  UINT16 wCtrlType;     // Type of CAN controller (see CAN_CTRL_ const)
  UINT16 wBusCoupling;  // Type of Bus coupling (see CAN_BUSC_ const)
  UINT32 dwFeatures;    // supported features (see CAN_FEATURE_ constants)

  UINT32 dwCanClkFreq;  // CAN clock frequency [Hz]
  CANBTP sSdrRangeMin;  // minimum bit timing values for standard bit rate
  CANBTP sSdrRangeMax;  // maximum bit timing values for standard bit rate
  CANBTP sFdrRangeMin;  // minimum bit timing values for fast data bit rate
  CANBTP sFdrRangeMax;  // maximum bit timing values for fast data bit rate

  UINT32 dwTscClkFreq;  // clock frequency of the time stamp counter [Hz]
  UINT32 dwTscDivisor;  // divisor for the message time stamp counter

  UINT32 dwCmsClkFreq;  // clock frequency of cyclic message scheduler [Hz]
  UINT32 dwCmsDivisor;  // divisor for the cyclic message scheduler
  UINT32 dwCmsMaxTicks; // maximum tick count value of the cyclic message
                        // scheduler

  UINT32 dwDtxClkFreq;  // clock frequency of the delayed message transmitter [Hz]
  UINT32 dwDtxDivisor;  // divisor for the delayed message transmitter
  UINT32 dwDtxMaxTicks; // maximum tick count value of the delayed
                        // message transmitter
} CANCAPABILITIES2, *PCANCAPABILITIES2;


//
// Remarks: All values within the sSdrRangeMin, sSdrRangeMax, sFdrRangeMin
// and sFdrRangeMax are controller specific values, i.e. CAN_BTMODE_RAW
// must be set in the corresponding CANBTP.dwMode field
//

//
// controller types
//

#define CAN_CTRL_UNKNOWN          0    // unknown
#define CAN_CTRL_82527            1    // Intel 82527
#define CAN_CTRL_82C200           2    // Intel 82C200
#define CAN_CTRL_81C90            3    // Intel 81C90
#define CAN_CTRL_81C92            4    // Intel 81C92
#define CAN_CTRL_SJA1000          5    // Philips SJA 1000
#define CAN_CTRL_82C900           6    // Infineon 82C900 (TwinCAN)
#define CAN_CTRL_TOUCAN           7    // Motorola TOUCAN
#define CAN_CTRL_MSCAN            8    // Freescale Star12 MSCAN
#define CAN_CTRL_FLEXCAN          9    // Freescale FlexCAN
#define CAN_CTRL_IFICAN           10   // IFI CAN Controller
#define CAN_CTRL_BCCAN            11   // Bosch C_CAN
#define CAN_CTRL_BXCAN            12   // ST BX_CAN
#define CAN_CTRL_IFICFD           13   // IFI CAN FD Controller
#define CAN_CTRL_BMCAN            14   // Bosch M_CAN
#define CAN_CTRL_MAXVAL           255  // maximum value for controller type

//
// bus coupling types
//

#define CAN_BUSC_UNDEFINED        0x0000    // undefined
#define CAN_BUSC_LOWSPEED         0x0001    // low speed coupling
#define CAN_BUSC_HIGHSPEED        0x0002    // high speed coupling

//
// supported features
//

#define CAN_FEATURE_STDOREXT      0x00000001 // 11 OR 29 bit (exclusive)
#define CAN_FEATURE_STDANDEXT     0x00000002 // 11 AND 29 bit (simultaneous)
#define CAN_FEATURE_RMTFRAME      0x00000004 // reception of remote frames
#define CAN_FEATURE_ERRFRAME      0x00000008 // reception of error frames
#define CAN_FEATURE_BUSLOAD       0x00000010 // bus load measurement
#define CAN_FEATURE_IDFILTER      0x00000020 // exact message filter
#define CAN_FEATURE_LISTONLY      0x00000040 // listen only mode
#define CAN_FEATURE_SCHEDULER     0x00000080 // cyclic message scheduler
#define CAN_FEATURE_GENERRFRM     0x00000100 // error frame generation
#define CAN_FEATURE_DELAYEDTX     0x00000200 // delayed message transmitter
#define CAN_FEATURE_SINGLESHOT    0x00000400 // single shot mode
#define CAN_FEATURE_HIGHPRIOR     0x00000800 // high priority message
#define CAN_FEATURE_AUTOBAUD      0x00001000 // automatic bit rate detection
#define CAN_FEATURE_EXTDATA       0x00002000 // extended data length (CANFD)
#define CAN_FEATURE_FASTDATA      0x00004000 // fast data bit rate (CANFD)
#define CAN_FEATURE_ISOFRAME      0x00008000 // ISO conform frame (CANFD)
#define CAN_FEATURE_NONISOFRM     0x00010000 // non ISO conform frame (CANFD)
                                             // (different CRC computation)
#define CAN_FEATURE_64BITTSC      0x00020000 // 64-bit time stamp counter


/*****************************************************************************
 * CAN controller initialization
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

typedef struct _CANINITLINE2
{
  UINT8  bOpMode;  // CAN operating mode (see CAN_OPMODE_ constants)
  UINT8  bExMode;  // Extended mode (see CAN_EXMODE_ constants)
  UINT8  bSFMode;  // mode for line specific 11-bit ID filter
  UINT8  bEFMode;  // mode for line specific 29-bit ID filter
  UINT32 dwSFIds;  // size of line specific 11-bit ID filter
  UINT32 dwEFIds;  // size of line specific 29-bit ID filter
  CANBTP sBtpSdr;  // standard bit rate timing
  CANBTP sBtpFdr;  // fast data bit rate timing
} CANINITLINE2, *PCANINITLINE2;

//
// controller operating modes
//

#define CAN_OPMODE_UNDEFINED 0x00 // undefined
#define CAN_OPMODE_STANDARD  0x01 // reception of 11-bit id messages
#define CAN_OPMODE_EXTENDED  0x02 // reception of 29-bit id messages
#define CAN_OPMODE_ERRFRAME  0x04 // reception of error frames
#define CAN_OPMODE_LISTONLY  0x08 // listen only mode (TX passive)
#define CAN_OPMODE_LOWSPEED  0x10 // use low speed bus interface
#define CAN_OPMODE_AUTOBAUD  0x20 // automatic bit rate detection

//
// extended operating modes
//

#define CAN_EXMODE_DISABLED  0x00 // no extended operation
#define CAN_EXMODE_EXTDATA   0x01 // extended data length
#define CAN_EXMODE_FASTDATA  0x02 // fast data bit rate
#define CAN_EXMODE_NONISO    0x04 // non ISO conform frames

/*****************************************************************************
 * CAN controller status
 ****************************************************************************/

typedef struct _CANLINESTATUS
{
  UINT8  bOpMode;  // current CAN operating mode
  UINT8  bBtReg0;  // current bus timing register 0 value
  UINT8  bBtReg1;  // current bus timing register 1 value
  UINT8  bBusLoad; // average bus load in percent (0..100)
  UINT32 dwStatus; // status of the CAN controller (see CAN_STATUS_)
} CANLINESTATUS, *PCANLINESTATUS;

typedef struct _CANLINESTATUS2
{
  UINT8  bOpMode;   // current CAN operating mode
  UINT8  bExMode;   // current CAN extended operating mode
  UINT8  bBusLoad;  // average bus load in percent (0..100)
  UINT8  bReserved; // reserved set to 0
  CANBTP sBtpSdr;   // standard bit rate timing
  CANBTP sBtpFdr;   // fast data bit rate timing
  UINT32 dwStatus;  // status of the CAN controller (see CAN_STATUS_)
} CANLINESTATUS2, *PCANLINESTATUS2;

//
// controller status
//

#define CAN_STATUS_TXPEND    0x00000001 // transmission pending
#define CAN_STATUS_OVRRUN    0x00000002 // data overrun occurred
#define CAN_STATUS_ERRLIM    0x00000004 // error warning limit exceeded
#define CAN_STATUS_BUSOFF    0x00000008 // bus off status
#define CAN_STATUS_ININIT    0x00000010 // init mode active
#define CAN_STATUS_BUSCERR   0x00000020 // bus coupling error


/*****************************************************************************
 * CAN message channel status
 ****************************************************************************/

typedef struct _CANCHANSTATUS
{
  CANLINESTATUS sLineStatus; // current CAN line status
  BOOL32        fActivated;  // TRUE if the channel is activated
  BOOL32        fRxOverrun;  // TRUE if receive FIFO overrun occurred
  UINT8         bRxFifoLoad; // receive FIFO load in percent (0..100)
  UINT8         bTxFifoLoad; // transmit FIFO load in percent (0..100)
} CANCHANSTATUS, *PCANCHANSTATUS;

typedef struct _CANCHANSTATUS2
{
  CANLINESTATUS2 sLineStatus;   // current CAN line status
  BOOL8          fActivated;    // TRUE if the channel is activated
  BOOL8          fRxOverrun;    // TRUE if a receive FIFO overrun occurred
  UINT8          bRxFifoLoad;   // receive FIFO load in percent (0..100)
  UINT8          bTxFifoLoad;   // transmit FIFO load in percent (0..100)
} CANCHANSTATUS2, *PCANCHANSTATUS2;


/*****************************************************************************
 * CAN run-time status information
 ****************************************************************************/

typedef struct _CANRTINFO
{
  UINT32 dwNumChannels; // total number of open channels
  UINT32 dwActChannels; // number of active channels
  UINT32 dwLockStatus;  // lock status of various interfaces
                        // see CAN_RTI_LOCKSTAT_x constants below
  UINT16 wRxFifoLoad;   // device receive FIFO load in percent (0..100)
  UINT16 wTxFifoLoad;   // device transmit FIFO load in percent (0..100)
} CANRTINFO, *PCANRTINFO;

//
// interface lock status
//
#define CAN_RTI_LOCKSTAT_CTL  0x00000001 // ICanControl locked
#define CAN_RTI_LOCKSTAT_SHD  0x00000002 // ICanScheduler locked
#define CAN_RTI_LOCKSTAT_CHN  0x80000000 // exclusive channel lock


/*****************************************************************************
 * CAN cyclic message scheduler status
 ****************************************************************************/

#define CAN_MAX_CTX_MSGS  16 // maximum possible cyclic transmit messages

typedef struct _CANSCHEDULERSTATUS
{
  UINT8 bTaskStat;     // status of cyclic transmit task
  UINT8 abMsgStat[16]; // status of all cyclic transmit messages
} CANSCHEDULERSTATUS, *PCANSCHEDULERSTATUS;

typedef struct _CANSCHEDULERSTATUS2
{
  CANLINESTATUS2 sLineStatus;   // current CAN line status
  UINT8          bTaskStat;     // status of cyclic transmit task
  UINT8          abMsgStat[16]; // status of all cyclic transmit messages
} CANSCHEDULERSTATUS2, *PCANSCHEDULERSTATUS2;

//
// status of cyclic transmit task
//

#define CAN_CTXTSK_STAT_STOPPED   0x00 // cyclic transmit task stopped
#define CAN_CTXTSK_STAT_RUNNING   0x01 // cyclic transmit task running

//
// status of cyclic transmit messages
//

#define CAN_CTXMSG_STAT_EMPTY     0x00 // the message entry is empty
#define CAN_CTXMSG_STAT_BUSY      0x01 // the processing of the
                                       // message is in progress
#define CAN_CTXMSG_STAT_DONE      0x02 // the processing of the
                                       // message has completed


/*****************************************************************************
 * CAN message information
 ****************************************************************************/

#ifndef __cplusplus
#ifdef _MSC_VER
#pragma warning(disable:4201) // nameless struct/union
#endif
#endif

typedef union _CANMSGINFO
{
  struct
  {
    UINT8  bType;       // type (see CAN_MSGTYPE_ constants)
    union
    {
      UINT8 bReserved;  // reserved
      UINT8 bFlags2;    // extended flags (see CAN_MSGFLAGS2_ constants)
    };

    UINT8  bFlags;      // flags (see CAN_MSGFLAGS_ constants)
    UINT8  bAccept;     // accept code (see CAN_ACCEPT_ constants)
  } Bytes;

  struct
  {
    UINT32 type: 8;     // message type

    UINT32 ssm : 1;     // single shot mode
    UINT32 hpm : 1;     // high priority message
    UINT32 edl : 1;     // extended data length
    UINT32 fdr : 1;     // fast data bit rate
    UINT32 esi : 1;     // error state indicator
    UINT32 res : 3;     // reserved set to 0

    UINT32 dlc : 4;     // data length code
    UINT32 ovr : 1;     // data overrun
    UINT32 srr : 1;     // self reception request
    UINT32 rtr : 1;     // remote transmission request
    UINT32 ext : 1;     // extended frame format (0=standard, 1=extended)

    UINT32 afc : 8;     // acceptance filter code
  } Bits;

} CANMSGINFO, *PCANMSGINFO;

#ifndef __cplusplus
#ifdef _MSC_VER
#pragma warning(default:4201) // nameless struct/union
#endif
#endif

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
// message information flags (used by <CANMSGINFO.Bytes.bFlags>)
//

#define CAN_MSGFLAGS_DLC     0x0F //[bit 0] data length code 
#define CAN_MSGFLAGS_OVR     0x10 //[bit 4] data overrun flag
#define CAN_MSGFLAGS_SRR     0x20 //[bit 5] self reception request
#define CAN_MSGFLAGS_RTR     0x40 //[bit 6] remote transmission request
#define CAN_MSGFLAGS_EXT     0x80 //[bit 7] frame format (0=11-bit, 1=29-bit)

#define CAN_MAKE_MSGFLAGS(dlc,ovr,srr,rtr,ext) (UINT8) \
  ( (((ext) & 0x01) << 7) | (((rtr) & 0x01) << 6) |    \
    (((srr) & 0x01) << 5) | (((ovr) & 0x01) << 4) |    \
     ((dlc) & 0x0F) )


//
// extended message information flags (used by <CANMSGINFO.Bytes.bFlags2>)
//

#define CAN_MSGFLAGS2_SSM    0x01 //[bit 0] single shot mode
#define CAN_MSGFLAGS2_HPM    0x02 //[bit 1] high priority message
#define CAN_MSGFLAGS2_EDL    0x04 //[bit 2] extended data length
#define CAN_MSGFLAGS2_FDR    0x08 //[bit 3] fast data bit rate
#define CAN_MSGFLAGS2_ESI    0x10 //[bit 4] error state indicator
#define CAN_MSGFLAGS2_RES    0xE0 //[bit 5..7] reserved bits

#define CAN_MAKE_MSGFLAGS2(ssm,hpm,edl,fdr,esi) (UINT8) \
 ( (((esi) & 0x01) << 4) | (((fdr) & 0x01) << 3) |      \
   (((edl) & 0x01) << 2) | (((hpm) & 0x01) << 1) |      \
   (((ssm) & 0x01) << 0) )


//
// acceptance filter status (used by <CANMSGINFO.Bytes.bAccept>)
//

#define CAN_ACCEPT_REJECT    0x00 // message not accepted
#define CAN_ACCEPT_ALWAYS    0xFF // message always accepted
#define CAN_ACCEPT_FILTER_1  0x01 // message accepted by filter 1
#define CAN_ACCEPT_FILTER_2  0x02 // message accepted by filter 2
#define CAN_ACCEPT_PASSEXCL  0x03 // message passes exclusion filter


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

#define CAN_ERROR_NONE       0 // no additional error information
#define CAN_ERROR_STUFF      1 // stuff error
#define CAN_ERROR_FORM       2 // form error
#define CAN_ERROR_ACK        3 // acknowledgment error
#define CAN_ERROR_BIT        4 // bit error
#define CAN_ERROR_FDB        5 // fast data bit error
#define CAN_ERROR_CRC        6 // CRC error
#define CAN_ERROR_DLC        7 // data length error
#define CAN_ERROR_OTHER      8 // other (unspecified) error


/*****************************************************************************
 * special CAN message identifier
 ****************************************************************************/

#define CAN_MSGID_MASK11     0x000007FF // mask for 11-bit identifier
#define CAN_MSGID_MASK29     0x1FFFFFFF // mask for 29-bit identifier
#define CAN_MSGID_INVALID    0xE0000000 // invalid message identifier

#define CAN_MSGID_INFO       0xFFFFFFFF // message ID for CAN_MSGTYPE_INFO
#define CAN_MSGID_ERROR      0xFFFFFFFF // message ID for CAN_MSGTYPE_ERROR
#define CAN_MSGID_STATUS     0xFFFFFFFF // message ID for CAN_MSGTYPE_STATUS
#define CAN_MSGID_WAKEUP     0xFFFFFFFF // message ID for CAN_MSGTYPE_WAKEUP


/*****************************************************************************
 * CAN message structure
 ****************************************************************************/

typedef struct _CANMSG
{
  UINT32     dwTime;     // time stamp for receive message
  UINT32     dwMsgId;    // CAN message identifier (INTEL format)
  CANMSGINFO uMsgInfo;   // message information (bit field)
  UINT8      abData[8];  // message data
} CANMSG, *PCANMSG;      // size is 20 bytes


typedef struct _CANMSG2
{
  UINT32     dwTime;     // time stamp for receive message
  UINT32     _rsvd_;     // reserved (set to 0)
  UINT32     dwMsgId;    // CAN message identifier (INTEL format)
  CANMSGINFO uMsgInfo;   // message information (bit field)
  UINT8      abData[64]; // message data
} CANMSG2, *PCANMSG2;    // size is 80 bytes


/*****************************************************************************
 * CAN specific DLC to byte length translation
 ****************************************************************************/

#ifdef __cplusplus
const UINT8 CAN_EDLC_XLT[] =
{
  0,1,2,3,4,5,6,7,8,12,16,20,24,32,48,64
};
const UINT8 CAN_ELEN_XLT[] =
{
  /* 0*/ 0,
  /* 1*/ 1,/* 2*/ 2,/* 3*/ 3,/* 4*/ 4,
  /* 5*/ 5,/* 6*/ 6,/* 7*/ 7,/* 8*/ 8,
  /* 9*/ 9,/*10*/ 9,/*11*/ 9,/*12*/ 9,
  /*13*/10,/*14*/10,/*15*/10,/*16*/10,
  /*17*/11,/*18*/11,/*19*/11,/*20*/11,
  /*21*/12,/*22*/12,/*23*/12,/*24*/12,
  /*25*/13,/*26*/13,/*27*/13,/*28*/13,
  /*29*/13,/*30*/13,/*31*/13,/*32*/13,
  /*33*/14,/*34*/14,/*35*/14,/*36*/14,
  /*37*/14,/*38*/14,/*39*/14,/*40*/14,
  /*41*/14,/*42*/14,/*43*/14,/*44*/14,
  /*45*/14,/*46*/14,/*47*/14,/*48*/14,
  /*49*/15,/*50*/15,/*51*/15,/*52*/15,
  /*53*/15,/*54*/15,/*55*/15,/*56*/15,
  /*57*/15,/*58*/15,/*59*/15,/*60*/15,
  /*61*/15,/*62*/15,/*63*/15,/*64*/15
};
#else
#ifdef INIT_CAN_EDLC_XLT
const UINT8 CAN_EDLC_XLT[] =
{
  0,1,2,3,4,5,6,7,8,12,16,20,24,32,48,64
};
const UINT8 CAN_ELEN_XLT[] =
{
  /* 0*/ 0,
  /* 1*/ 1,/* 2*/ 2,/* 3*/ 3,/* 4*/ 4,
  /* 5*/ 5,/* 6*/ 6,/* 7*/ 7,/* 8*/ 8,
  /* 9*/ 9,/*10*/ 9,/*11*/ 9,/*12*/ 9,
  /*13*/10,/*14*/10,/*15*/10,/*16*/10,
  /*17*/11,/*18*/11,/*19*/11,/*20*/11,
  /*21*/12,/*22*/12,/*23*/12,/*24*/12,
  /*25*/13,/*26*/13,/*27*/13,/*28*/13,
  /*29*/13,/*30*/13,/*31*/13,/*32*/13,
  /*33*/14,/*34*/14,/*35*/14,/*36*/14,
  /*37*/14,/*38*/14,/*39*/14,/*40*/14,
  /*41*/14,/*42*/14,/*43*/14,/*44*/14,
  /*45*/14,/*46*/14,/*47*/14,/*48*/14,
  /*49*/15,/*50*/15,/*51*/15,/*52*/15,
  /*53*/15,/*54*/15,/*55*/15,/*56*/15,
  /*57*/15,/*58*/15,/*59*/15,/*60*/15,
  /*61*/15,/*62*/15,/*63*/15,/*64*/15
};
#else
extern const UINT8 CAN_EDLC_XLT[];
extern const UINT8 CAN_ELEN_XLT[];
#endif
#endif

#define CAN_SDLC_MAX    8
#define CAN_SLEN_MAX    8

#define CAN_EDLC_MAX    15
#define CAN_ELEN_MAX    64

#define CAN_SDLC_TO_LEN(dlc) \
  (((dlc)>=0) && ((dlc)<=CAN_SDLC_MAX))? (UINT8)(dlc) : CAN_SLEN_MAX

#define CAN_EDLC_TO_LEN(dlc) \
  (((dlc)>=0) && ((dlc)<=CAN_EDLC_MAX))? CAN_EDLC_XLT[(dlc)] : CAN_ELEN_MAX

#define CAN_LEN_TO_SDLC(len) \
  (((len)>=0) && ((len)<=CAN_SLEN_MAX))? (UINT8)(len) : CAN_SDLC_MAX

#define CAN_LEN_TO_EDLC(len) \
  (((len)>=0) && ((len)<=CAN_ELEN_MAX))? CAN_ELEN_XLT[(len)] : CAN_EDLC_MAX


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


typedef struct _CANCYCLICTXMSG2
{
  UINT16     wCycleTime; // cycle time for the message in ticks
  UINT8      bIncrMode;  // auto increment mode (see CAN_CTXMSG_INC_ const)
  UINT8      bByteIndex; // index of the byte within abData[] to increment
  UINT32     dwMsgId;    // message identifier (INTEL format)
  CANMSGINFO uMsgInfo;   // message information (bit field)
  UINT8      abData[64]; // message data
} CANCYCLICTXMSG2, *PCANCYCLICTXMSG2;


//
// increment modes for cyclic transmit messages
//

#define CAN_CTXMSG_INC_NO    0x00 // no increment
#define CAN_CTXMSG_INC_ID    0x01 // increment CAN identifier
#define CAN_CTXMSG_INC_8     0x02 // increment 8-bit data field
#define CAN_CTXMSG_INC_16    0x03 // increment 16-bit data field

#include <poppack.h>
#endif //_CANTYPE_H_
