//////////////////////////////////////////////////////////////////////////
// IXXAT Automation GmbH
//////////////////////////////////////////////////////////////////////////
/**
  Common LIN constants, data types and macros.

  @file "lintype.h"

  @note 
    This file is shared between user and kernel mode components. Request a 
    kernel mode developer before changing the contents of this file.
*/
//////////////////////////////////////////////////////////////////////////
// (C) 2002-2011 IXXAT Automation GmbH, all rights reserved
//////////////////////////////////////////////////////////////////////////

#ifndef LINTYPE_H
#define LINTYPE_H

//////////////////////////////////////////////////////////////////////////
//  include files
//////////////////////////////////////////////////////////////////////////
#include <pshpack1.h>

//////////////////////////////////////////////////////////////////////////
//  constants, macros, types
//////////////////////////////////////////////////////////////////////////

/*****************************************************************************
 * controller types
 ****************************************************************************/

#define LIN_CTRL_UNKNOWN     0    // unknown
#define LIN_CTRL_MAXVAL      255  // maximum value for controller type


/*****************************************************************************
 * supported features
 ****************************************************************************/

#define LIN_FEATURE_MASTER   0x0001 // master mode
#define LIN_FEATURE_AUTORATE 0x0002 // automatic bitrate detection
#define LIN_FEATURE_ERRFRAME 0x0004 // reception of error frames
#define LIN_FEATURE_BUSLOAD  0x0008 // bus load measurement
#define LIN_FEATURE_SLEEP    0x0010 // support the Sleep message ( Master )
#define LIN_FEATURE_WAKEUP   0x0020 // support the WakeUp message ( Slave / Master )


/*****************************************************************************
 * controller operating modes
 ****************************************************************************/

#define LIN_OPMODE_SLAVE     0x00 // slave
#define LIN_OPMODE_MASTER    0x01 // enable master mode
#define LIN_OPMODE_ERRORS    0x02 // enable reception of error frames


/*****************************************************************************
 * predefined bitrates
 ****************************************************************************/

#define LIN_BITRATE_UNDEF    65535 // undefined bit-rate
#define LIN_BITRATE_AUTO     0     // automatic bit-rate detection
#define LIN_BITRATE_MIN      1000  // lowest specified bit-rate
#define LIN_BITRATE_MAX      20000 // highest specified bit-rate

#define LIN_BITRATE_1000     1000  //  1000 baud
#define LIN_BITRATE_1200     1200  //  1200 baud
#define LIN_BITRATE_2400     2400  //  2400 baud
#define LIN_BITRATE_4800     4800  //  4800 baud
#define LIN_BITRATE_9600     9600  //  9600 baud
#define LIN_BITRATE_10400    10400 // 10400 baud
#define LIN_BITRATE_19200    19200 // 19200 baud
#define LIN_BITRATE_20000    20000 // 20000 baud


/*****************************************************************************
 * controller status
 ****************************************************************************/
#define LIN_STATUS_OVRRUN    0x01 // data overrun occurred
#define LIN_STATUS_ININIT    0x10 // init mode active


/*****************************************************************************
 * controller initialization structure
 ****************************************************************************/

typedef struct _LININITLINE
{
  UINT8  bOpMode;   // operating mode (see LIN_OPMODE_ constants)
  UINT8  bReserved; // reserved
  UINT16 wBitrate;  // bit rate (see LIN_BITRATE_ constants)
} LININITLINE, *PLININITLINE;


/*****************************************************************************
 * LIN capabilities
 ****************************************************************************/

typedef struct _LINCAPABILITIES
{
  UINT32 dwFeatures;   // supported features (see LIN_FEATURE_ constants)
  UINT32 dwClockFreq;  // clock frequency of the primary counter in Hz 
  UINT32 dwTscDivisor; // divisor for the message time stamp counter
} LINCAPABILITIES, *PLINCAPABILITIES;


/*****************************************************************************
 * controller status information structure
 ****************************************************************************/

typedef struct _LINLINESTATUS
{
  UINT8  bOpMode;   // current operating mode
  UINT8  bReserved; // reserved
  UINT16 wBitrate;  // current bit rate
  UINT32 dwStatus;  // status of the LIN controller (see LIN_STATUS_)
} LINLINESTATUS, *PLINLINESTATUS;


/*****************************************************************************
 * message monitor status information structure
 ****************************************************************************/

typedef struct _LINMONITORSTATUS
{
  LINLINESTATUS sLineStatus; // current LIN line status
  UINT32        fActivated;  // TRUE if the monitor is activated
  UINT32        fRxOverrun;  // TRUE if receive FIFO overrun occurs
  UINT8         bRxFifoLoad; // receive FIFO load in percent (0..100)
} LINMONITORSTATUS, *PLINMONITORSTATUS;


/*****************************************************************************
 * LIN message information
 ****************************************************************************/

/*lint -save -e46 (field type should be int) */
typedef union _LINMSGINFO
{
  struct
  {
    UINT8  bPid;     // protected id
    UINT8  bType;    // message type (see LIN_MSGTYPE_ constants)
    UINT8  bDlen;    // data length
    UINT8  bFlags;   // flags (see LIN_MSGFLAGS_ constants)
  } Bytes;

  struct
  {
    UINT32 pid  : 8; // protected identifier
    UINT32 type : 8; // message type
    UINT32 dlen : 8; // data length
    UINT32 ecs  : 1; // enhanced checksum
    UINT32 sor  : 1; // sender of response
    UINT32 ovr  : 1; // possible data overrun
    UINT32 ido  : 1; // ID only
    UINT32 res  : 4; // reserved
  } Bits;

} LINMSGINFO, *PLINMSGINFO;
/*lint -restore */

//
// message types (see <LINMSGINFO.Bytes.bType>)
//

#define LIN_MSGTYPE_DATA     0x00 // data frame
#define LIN_MSGTYPE_INFO     0x01 // info frame
#define LIN_MSGTYPE_ERROR    0x02 // error frame
#define LIN_MSGTYPE_STATUS   0x03 // status frame
#define LIN_MSGTYPE_WAKEUP   0x04 // wakeup frame
#define LIN_MSGTYPE_SLEEP    0x05 // goto sleep frame
#define LIN_MSGTYPE_TMOVR    0x06 // timer overrun

//
// message flags (used by <LINMSGINFO.Bytes.bFlags>)
//

#define LIN_MSGFLAGS_ECS     0x01 // enhanced checksum (LIN 2.0)
#define LIN_MSGFLAGS_SOR     0x02 // sender of response
#define LIN_MSGFLAGS_OVR     0x04 // possible data overrun
#define LIN_MSGFLAGS_IDO     0x08 // ID only

//
// Information supplied in the abData[0] field of info frames
// (LINMSGINFO.Bytes.bType = LIN_MSGTYPE_INFO).
//

#define LIN_INFO_START       1 // start of LIN controller
#define LIN_INFO_STOP        2 // stop of LIN controller
#define LIN_INFO_RESET       3 // reset of LIN controller

//
// Error information supplied in the abData[0] field of error frames
// (LINMSGINFO.Bytes.bType = LIN_MSGTYPE_ERROR).
//

#define LIN_ERROR_BIT        1 // bit error
#define LIN_ERROR_CHKSUM     2 // checksum error
#define LIN_ERROR_PARITY     3 // identifier parity error
#define LIN_ERROR_SLNORE     4 // slave not responding error
#define LIN_ERROR_SYNC       5 // inconsistent sync field error
#define LIN_ERROR_NOBUS      6 // no bus activity error
#define LIN_ERROR_OTHER      7 // other (unspecified) error


/*****************************************************************************
 * LIN message structure
 ****************************************************************************/

typedef struct _LINMSG
{
  UINT32     dwTime;    // time stamp for receive message [ms]
  LINMSGINFO uMsgInfo;  // message information (bit field)
  UINT8      abData[8]; // message data
} LINMSG, *PLINMSG;     // size is 20 bytes

#include <poppack.h>

#endif //_LINTYPE_H_
