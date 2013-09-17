//////////////////////////////////////////////////////////////////////////
// IXXAT Automation GmbH
//////////////////////////////////////////////////////////////////////////
/**
  Common BAL specific constants, data types and macros.

  @file "baltype.h"

  @note
    This file is shared between user and kernel mode components. Request a
    kernel mode developer before changing the contents of this file.
*/
//////////////////////////////////////////////////////////////////////////
// (C) 2002-2011 IXXAT Automation GmbH, all rights reserved
//////////////////////////////////////////////////////////////////////////

#ifndef BALTYPE_H
#define BALTYPE_H

//////////////////////////////////////////////////////////////////////////
//  include files
//////////////////////////////////////////////////////////////////////////
#include <VCICtrlType.h>
#include <VCIIdType.h>
#include <XATpshpack1.h>

//////////////////////////////////////////////////////////////////////////
//  constants, macros, types
//////////////////////////////////////////////////////////////////////////

/*****************************************************************************
 * BAL firmware features
 ****************************************************************************/

#define BAL_MAX_SOCKETS  VCI_MAX_BUSCTRL // maximum possible bus sockets

typedef struct _BALFEATURES
{
    UINT16 FwMajorVersion;                 // major firmware version number
    UINT16 FwMinorVersion;                 // minor firmware version number
    UINT16 BusSocketCount;                 // number of supported bus sockets
    UINT16 BusSocketType[BAL_MAX_SOCKETS]; // array of supported bus sockets
} BALFEATURES, *PBALFEATURES;


/*****************************************************************************
 * BAL socket information
 ****************************************************************************/

typedef struct _BALSOCKETINFO
{
    VCIID  ObjectId;  // object identifier
    UINT16 BusType;   // type of the bus socket
    UINT16 SocketNo;  // number of the bus socket
} BALSOCKETINFO, *PBALSOCKETINFO;


#include <XATpoppack.h>
#endif //_BALTYPE_H_
