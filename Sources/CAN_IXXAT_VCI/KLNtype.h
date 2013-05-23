/*****************************************************************************
 IXXAT Automation GmbH
******************************************************************************

 File    : KLNTYPE.H
 Summary : Common K-Line specific constants, data types and macros.

 Date    : 2011-04-20
 Author  : Hartmut Heim

 Compiler: MSVC

 Remarks : This file is shared between user and kernel mode components.
           Request a kernel mode developer before changing the contents
           of this file.

******************************************************************************
 all rights reserved
*****************************************************************************/

#ifndef _KLNTYPE_H_
#define _KLNTYPE_H_

#include <pshpack1.h>

/*****************************************************************************
 * controller types
 ****************************************************************************/

#define KLN_CTRL_UNKNOWN     0    // unknown
#define KLN_CTRL_RS232       1    // RS232 converter
#define KLN_CTRL_MAXVAL      255  // maximum value for controller type


/*****************************************************************************
 * supported features
 ****************************************************************************/

#define KLN_FEATURE_AUTORATE 0x0001 // automatic bitrate detection
#define KLN_FEATURE_TXERRDET 0x0002 // supports transmit error detection
#define KLN_FEATURE_ASCNTRLR 0x0004 // supports ECU mode


/*****************************************************************************
 * controller operating modes
 ****************************************************************************/
//
// initialization modes
//
#define KLN_OPMODE_NOPINIT   0x00000000 // no special initialization
#define KLN_OPMODE_BAUDINIT  0x00000001 // 5 baud initialization
#define KLN_OPMODE_FASTINIT  0x00000002 // ISO14230 fast initialization
#define KLN_OPMODE_18L0INIT  0x00000003 // 1.8 second logic 0 initialization
#define KLN_OPMODE_2GNDINIT  0x00000004 // 2 second wire short to ground init
#define KLN_OPMODE_INITMASK  0x00FFFFFF // mask for initialization mode

//
// initialization flags
//
#define KLN_OPMODE_LISTONLY  0x01000000 // listen only mode (TX passive)
#define KLN_OPMODE_ASCNTRLR  0x80000000 // operate in ECU mode


/*****************************************************************************
 * frame format
 ****************************************************************************/

#define KLN_FRMFMT_DATA_0         0x00000000 // no data bits
#define KLN_FRMFMT_DATA_1         0x00000001 // 1 data bit
#define KLN_FRMFMT_DATA_2         0x00000001 // 2 data bits
#define KLN_FRMFMT_DATA_3         0x00000003 // 3 data bits
#define KLN_FRMFMT_DATA_4         0x00000004 // 4 data bits
#define KLN_FRMFMT_DATA_5         0x00000005 // 5 data bits
#define KLN_FRMFMT_DATA_6         0x00000006 // 6 data bits
#define KLN_FRMFMT_DATA_7         0x00000007 // 6 data bits
#define KLN_FRMFMT_DATA_8         0x00000008 // 8 data bits
#define KLN_FRMFMT_DATA_9         0x00000009 // 9 data bits
#define KLN_FRMFMT_DATA_MASK      0x000000FF // mask for data bits

#define KLN_FRMFMT_STOP_10        0x00000100 // 1 stop bit
#define KLN_FRMFMT_STOP_15        0x00000200 // 1.5 stop bits
#define KLN_FRMFMT_STOP_20        0x00000400 // 2 stop bits
#define KLN_FRMFMT_STOP_MASK      0x0000FF00 // mask for stop bits

#define KLN_FRMFMT_PARITY_N       0x00000000 // no parity
#define KLN_FRMFMT_PARITY_O       0x00010000 // odd parity
#define KLN_FRMFMT_PARITY_E       0x00020000 // even parity
#define KLN_FRMFMT_PARITY_M       0x00040000 // mark parity
#define KLN_FRMFMT_PARITY_S       0x00080000 // space parity
#define KLN_FRMFMT_PARITY_MASK    0x00FF0000 // mask for parity bits


/*****************************************************************************
 * address information (required for StartLine request in ISO14230 mode)
 ****************************************************************************/

#define KLN_ADDRINF_NONE     0x00 // no address information
#define KLN_ADDRINF_EXCT     0x40 // exception mode (CARB)
#define KLN_ADDRINF_CARB     0x40 // exception mode (CARB)
#define KLN_ADDRINF_PHYS     0x80 // physical addressing
#define KLN_ADDRINF_FUNC     0xC0 // functional addressing
#define KLN_ADDRINF_MASK     0xC0 // mask for address bits within format byte


/*****************************************************************************
 * predefined bitrates
 ****************************************************************************/

#define KLN_BITRATE_UNDEF    ((UINT32)-1) // undefined bitrate
#define KLN_BITRATE_AUTO     0            // auto detect bitrate

#define KLN_BITRATE_MIN      1200
#define KLN_BITRATE_1200     1200    //  1200 baud
#define KLN_BITRATE_2400     2400    //  2400 baud
#define KLN_BITRATE_4800     4800    //  4800 baud
#define KLN_BITRATE_9600     9600    //  9600 baud
#define KLN_BITRATE_10400    10400   // 10400 baud
#define KLN_BITRATE_19200    19200   // 19200 baud
#define KLN_BITRATE_38400    38400   // 38400 baud
#define KLN_BITRATE_57600    57600   // 57600 baud
#define KLN_BITRATE_115200   115200  // 115200 baud
#define KLN_BITRATE_MAX      115200


/*****************************************************************************
 * controller status
 ****************************************************************************/

#define KLN_STATUS_ININIT    0x00000001 // initialization mode
#define KLN_STATUS_OVRRUN    0x00000002 // receive buffer overrun occurred
#define KLN_STATUS_RXTOUT    0x00000004 // receive data timing error
#define KLN_STATUS_TXTOUT    0x00000008 // transmit data timing error


/*****************************************************************************
 * controller initialization structure
 ****************************************************************************/

typedef struct _KLNINITLINE
{
  UINT32 dwOpMode;  // operating mode (see KLN_OPMODE_ constants)
  UINT16 wTxSize;   // minimum size of the transmit FIFO in bytes
  UINT16 wCtlBits;  // additional control bits (reserved set to 0)
  UINT32 dwFrmFmt;  // frame format (see KLN_FRMFMT_ constants)
  UINT32 dwBitrate; // transfer rate in bits/sec (see KLN_BITRATE_ constants)
} KLNINITLINE, *PKLNINITLINE;


/*****************************************************************************
 * communication strart configuration structure
 ****************************************************************************/

typedef struct _KLNSTARTLINE
{
  UINT8 bAddrInf;   // addressing info (see KLN_ADDRINF_ constants)
  UINT8 bSrcAddr;   // source address
  UINT8 bTgtAddr;   // target address
  UINT8 bNumKeys;   // number of key bytes (in parameter if ECU mode)
  UINT8 abKeys[8];  // array of key bytes (in parameter if ECU mode)
} KLNSTARTLINE, *PKLNSTARTLINE;

#define KLN_MAX_KEYS NUMELEM( ((PKLNSTARTLINE)NULL)->abKeys )


/*****************************************************************************
 * controller timing parameter structure
 ****************************************************************************/

typedef struct _KLNTIMINGS
{
  UINT32 dwP1min;  // minimum ECU inter byte gap time in microseconds
  UINT32 dwP1max;  // maximum ECU inter byte gap time in microseconds
  UINT32 dwP2min;  // minimum ECU response time in microseconds
  UINT32 dwP2max;  // maximum ECU response time in microseconds
  UINT32 dwP3min;  // minimum tester request wait time in microseconds
  UINT32 dwP3max;  // maximum tester request wait time in microseconds
  UINT32 dwP4min;  // minimum tester inter byte gap time in microseconds
  UINT32 dwP4max;  // maximum tester inter byte gap time in microseconds
} KLNTIMINGS, *PKLNTIMINGS;


/*****************************************************************************
 * K-Line capabilities
 ****************************************************************************/

typedef struct _KLNCAPABILITIES
{
  UINT32 dwFeatures;   // supported features (see LIN_FEATURE_ constants)
  UINT32 dwClockFreq;  // clock frequency of the bit rate generator in HZ
} KLNCAPABILITIES, *PKLNCAPABILITIES;


/*****************************************************************************
 * controller status information structure
 ****************************************************************************/

typedef struct _KLNLINESTATUS
{
  UINT32 dwOpMode;   // current operating mode (see KLN_OPMODE_ constants)
  UINT32 dwFrmFmt;   // current frame format (see KLN_FRMFMT_ constants)
  UINT32 dwBitrate;  // current bit rate (bits/s)
  UINT32 dwStatus;   // status of the K-Line controller (see KLN_STATUS_)
} KLNLINESTATUS, *PKLNLINESTATUS;


/*****************************************************************************
 * message monitor status information structure
 ****************************************************************************/

typedef struct _KLNMONITORSTATUS
{
  KLNLINESTATUS sLineStatus; // current K-Line line status
  UINT32        fActivated;  // TRUE if the monitor is activated
  UINT32        fRxOverrun;  // TRUE if receive FIFO overrun occurs
  UINT8         bRxFifoLoad; // receive FIFO load in percent (0..100)
} KLNMONITORSTATUS, *PKLNMONITORSTATUS;


#include <poppack.h>

#endif //_KLNTYPE_H_
