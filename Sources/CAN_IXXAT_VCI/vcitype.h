//////////////////////////////////////////////////////////////////////////
// IXXAT Automation GmbH
//////////////////////////////////////////////////////////////////////////
/**
  VCI specific constants, data types and macros.

  @file "vciType.h"

  @note
  This file is shared between user and kernel mode components.
  Request a kernel mode developer before changing the contents
  of this file

*/
//////////////////////////////////////////////////////////////////////////
// (C) 2002-2011 IXXAT Automation GmbH, all rights reserved
//////////////////////////////////////////////////////////////////////////

#ifndef VCITYPE_H
#define VCITYPE_H

//////////////////////////////////////////////////////////////////////////
//  include files
//////////////////////////////////////////////////////////////////////////
#include <pshpack1.h>  


//////////////////////////////////////////////////////////////////////////
//  constants and macros
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// data types
//////////////////////////////////////////////////////////////////////////

/*****************************************************************************
 * VCI version information
 ****************************************************************************/

typedef struct _VCIVERSIONINFO
{
  UINT32 VciMajorVersion;    // major VCI server version number
  UINT32 VciMinorVersion;    // minor VCI server version number
  UINT32 OsMajorVersion;     // major os version number
  UINT32 OsMinorVersion;     // minor os version number
  UINT32 OsBuildNumber;      // os build number
  UINT32 OsPlatformId;       // os platform id
} VCIVERSIONINFO, *PVCIVERSIONINFO;

typedef struct _VCIVERSIONINFO2
{
  UINT32 VciMajorVersion;    // major VCI server version number
  UINT32 VciMinorVersion;    // minor VCI server version number
  UINT32 VciReleaseVersion;  // release VCI server version number
  UINT32 VciBuildVersion;    // build VCI server version number
  UINT32 OsMajorVersion;     // major os version number
  UINT32 OsMinorVersion;     // minor os version number
  UINT32 OsBuildNumber;      // os build number
  UINT32 OsPlatformId;       // os platform id
} VCIVERSIONINFO2, *PVCIVERSIONINFO2;

/*****************************************************************************
 * VCI driver information
 ****************************************************************************/
typedef struct _VCIDRIVERINFO
{
  VCIID  VciObjectId;    // unique VCI object identifier
  GUID   DriverClass;    // driver class identifier
  UINT16 MajorVersion;   // minor driver version number
  UINT16 MinorVersion;   // minor driver version number
  UINT16 ReleaseVersion; // build driver version number
  UINT16 BuildVersion;   // build driver version number
} VCIDRIVERINFO, *PVCIDRIVERINFO;


/*****************************************************************************
 * VCI device information
 ****************************************************************************/

typedef struct _VCIDEVICEINFO
{
  VCIID  VciObjectId;          // unique VCI object identifier
  GUID   DeviceClass;          // device class identifier

  UINT8  DriverMajorVersion;   // major driver version number
  UINT8  DriverMinorVersion;   // minor driver version number 
  UINT16 DriverBuildVersion;   // build driver version number

  UINT8  HardwareBranchVersion;// branch hardware version number
  UINT8  HardwareMajorVersion; // major hardware version number
  UINT8  HardwareMinorVersion; // minor hardware version number
  UINT8  HardwareBuildVersion; // build hardware version number

  union _UniqueHardwareId      // unique hardware identifier
  {
    CHAR AsChar[16];
    GUID AsGuid;
  } UniqueHardwareId;

  CHAR Description [128];       // device description (e.g: "PC-I04-PCI")
  CHAR Manufacturer[126];       // device manufacturer (e.g: "IXXAT Automation")

  UINT16 DriverReleaseVersion; // minor driver version number
} VCIDEVICEINFO, *PVCIDEVICEINFO;

#include <poppack.h>

#endif //VCITYPE_H
