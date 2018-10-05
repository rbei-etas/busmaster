/*****************************************************************************
 HMS Technology Center Ravensburg GmbH
******************************************************************************

 File    : BALTYPE.H
 Summary : Common BAL specific constants, data types and macros.

 Date    : 2004-06-03
 Author  : Hartmut Heim

 Compiler: MSVC

******************************************************************************
 all rights reserved
*****************************************************************************/

#ifndef _BALTYPE_H_
#define _BALTYPE_H_

#include <vcitype.h>
#include <pshpack1.h>

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

#include <poppack.h>
#endif //_BALTYPE_H_
