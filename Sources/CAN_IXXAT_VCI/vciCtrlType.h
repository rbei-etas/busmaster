//////////////////////////////////////////////////////////////////////////
// IXXAT Automation GmbH
//////////////////////////////////////////////////////////////////////////
/**
  VCI specific constants, data types and macros.

  @file "vciCtrlType.h"

  @note
  This file is shared between user and kernel mode components.
  Request a kernel mode developer before changing the contents
  of this file

*/
//////////////////////////////////////////////////////////////////////////
// (C) 2002-2011 IXXAT Automation GmbH, all rights reserved
//////////////////////////////////////////////////////////////////////////

#ifndef VCICTRLTYPE_H
#define VCICTRLTYPE_H

//////////////////////////////////////////////////////////////////////////
//  include files
//////////////////////////////////////////////////////////////////////////
#include <CANtype.h>
#include <LINtype.h>
#include <KLNtype.h>
#include <pshpack1.h>

//////////////////////////////////////////////////////////////////////////
//  constants and macros
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// data types
//////////////////////////////////////////////////////////////////////////

/*****************************************************************************
 * VCI bus controller types
 *
 * +-------+--------+
 * | 15..8 | 7 .. 0 |
 * +-------+--------+
 * Bit 15..8 : bus type (0 is reserved)
 * Bit  7..0 : controller type
 ****************************************************************************/

#define VCI_BUS_RES  0 // reserved
#define VCI_BUS_CAN  1 // CAN
#define VCI_BUS_LIN  2 // LIN
#define VCI_BUS_FLX  3 // FlexRay
#define VCI_BUS_KLN  4 // K-Line

#define VCI_BUS_CTRL(Bus,Ctrl) (UINT16) ( ((Bus)<<8) | (Ctrl) )
#define VCI_BUS_TYPE(BusCtrl)  (UINT8)  ( ((BusCtrl) >> 8) & 0x00FF )
#define VCI_CTL_TYPE(BusCtrl)  (UINT8)  ( ((BusCtrl) >> 0) & 0x00FF )

//------------------------------------------------------------------------
// CAN controller types
//------------------------------------------------------------------------

// unknown CAN controller
#define VCI_CAN_UNKNOWN      VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_UNKNOWN)

// Intel 82527
#define VCI_CAN_82527        VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_82527)

// Intel 82C200
#define VCI_CAN_82C200       VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_82C200)

// Intel 82C90
#define VCI_CAN_81C90        VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_81C90)
// backward compatibility to older wrong definition
#define VCI_CAN_82C90        VCI_CAN_81C90 

// Intel 82C92
#define VCI_CAN_81C92        VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_81C92)
// backward compatibility to older wrong definition
#define VCI_CAN_82C92        VCI_CAN_81C92

// Philips SJA 1000
#define VCI_CAN_SJA1000      VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_SJA1000)

// Infinion 82C900 (TwinCAN)
#define VCI_CAN_82C900       VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_82C900)

// Motorola TOUCAN
#define VCI_CAN_TOUCAN       VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_TOUCAN)

// IFI-CAN
#define VCI_CAN_IFI          VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_IFI)

// CCAN    ( Bosch C_CAN )
#define VCI_CAN_CCAN         VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_CCAN)

// BXCAN   ( ST BX_CAN )
#define VCI_CAN_BXCAN        VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_BXCAN)


//------------------------------------------------------------------------
// LIN controller types
//------------------------------------------------------------------------

// unknown LIN controller
#define VCI_LIN_UNKNOWN      VCI_BUS_CTRL(VCI_BUS_LIN, LIN_CTRL_UNKNOWN)


//------------------------------------------------------------------------
// FlexRay controller types
//------------------------------------------------------------------------

// unknown FlexRay controller
#define VCI_FLX_UNKNOWN      VCI_BUS_CTRL(VCI_BUS_FLX, 0)

//------------------------------------------------------------------------
// K-Line controller types
//------------------------------------------------------------------------

// unknown K-Line controller
#define VCI_KLI_UNKNOWN    VCI_BUS_CTRL(VCI_BUS_KLN, 0)

/*****************************************************************************
 * VCI device capabilities
 ****************************************************************************/

#define VCI_MAX_BUSCTRL 32   // maximum number of supported bus controllers

typedef struct _VCIDEVICECAPS
{
  UINT16 BusCtrlCount;       // number of supported bus controllers
  UINT16 BusCtrlTypes[32];   // array of supported bus controllers
} VCIDEVICECAPS, *PVCIDEVICECAPS;

#include <poppack.h>

#endif //VCICTRLTYPE_H
