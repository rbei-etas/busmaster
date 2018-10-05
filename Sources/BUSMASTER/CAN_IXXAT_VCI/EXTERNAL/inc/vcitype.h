/*****************************************************************************
 HMS Technology Center Ravensburg GmbH
******************************************************************************

 File    : VCITYPE.H
 Summary : VCI specific constants, data types and macros.

 Date    : 2003-09-04
 Author  : Hartmut Heim

 Compiler: MSVC

******************************************************************************
 all rights reserved
*****************************************************************************/

#ifndef _VCITYPE_H_
#define _VCITYPE_H_

#include <stdtype.h>
#include <pshpack1.h>

/*****************************************************************************
 * unique VCI object identifier
 ****************************************************************************/

typedef union _VCIID
{
  LUID  AsLuid;
  INT64 AsInt64;
} VCIID, *PVCIID;

#if defined(__cplusplus)
typedef const VCIID& REFVCIID;
#else
typedef const VCIID* const REFVCIID;
#endif

/*****************************************************************************
 * VCI version information
 ****************************************************************************/

typedef struct _VCIVERSIONINFO
{
  UINT32 VciMajorVersion;    // major VCI version number
  UINT32 VciMinorVersion;    // minor VCI version number
  UINT32 VciRevNumber;       // VCI revision number
  UINT32 VciBuildNumber;     // VCI build number
  UINT32 OsMajorVersion;     // major OS version number
  UINT32 OsMinorVersion;     // minor OS version number
  UINT32 OsBuildNumber;      // OS build number
  UINT32 OsPlatformId;       // OS platform id
} VCIVERSIONINFO, *PVCIVERSIONINFO;


/*****************************************************************************
 * VCI license information
 ****************************************************************************/

typedef struct _VCILICINFO
{
  GUID   DeviceClass;        // Class ID of the licensed product
  UINT32 MaxDevices;         // maximum number of allowed devices (0=no limit)
  UINT32 MaxRuntime;         // maximum runtime in seconds (0=no limit)
  UINT32 Restrictions;       // additional restrictions (see VCI_LICX_xxx const)
} VCILICINFO, *PVCILICINFO;

//
// license restriction flags
//

#define VCI_LICX_NORESTRICT  0x00000000 // no additional restrictions
#define VCI_LICX_SINGLEUSE   0x00000001 // single application use only


/*****************************************************************************
 * VCI driver information
 ****************************************************************************/

typedef struct _VCIDRIVERINFO
{
  VCIID  VciObjectId;   // unique VCI object identifier
  GUID   DriverClass;   // driver class identifier
  UINT16 MajorVersion;  // major driver version number
  UINT16 MinorVersion;  // minor driver version number
} VCIDRIVERINFO, *PVCIDRIVERINFO;


/*****************************************************************************
 * VCI device information
 ****************************************************************************/

typedef struct _VCIDEVICEINFO
{
  VCIID  VciObjectId;          // unique VCI object identifier
  GUID   DeviceClass;          // device class identifier

  UINT8  DriverMajorVersion;   // driver major version number
  UINT8  DriverMinorVersion;   // driver minor version number
  UINT16 DriverBuildVersion;   // driver build version number

  UINT8  HardwareBranchVersion;// hardware branch version number
  UINT8  HardwareMajorVersion; // hardware major version number
  UINT8  HardwareMinorVersion; // hardware minor version number
  UINT8  HardwareBuildVersion; // hardware build version number

  union _UniqueHardwareId      // unique hardware identifier
  {
    CHAR AsChar[16];
    GUID AsGuid;
  } UniqueHardwareId;

  CHAR   Description[128];     // device description
  CHAR   Manufacturer[126];    // device manufacturer
  UINT16 DriverReleaseVersion; // driver release version number
} VCIDEVICEINFO, *PVCIDEVICEINFO;


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
#define VCI_BUS_KLN  3 // K-Line
#define VCI_BUS_FXR  4 // FlexRay

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

// Intel 81C90
#define VCI_CAN_81C90        VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_81C90)

// Intel 81C92
#define VCI_CAN_81C92        VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_81C92)

// Philips SJA 1000
#define VCI_CAN_SJA1000      VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_SJA1000)

// Infineon 82C900 (TwinCAN)
#define VCI_CAN_82C900       VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_82C900)

// Motorola TOUCAN
#define VCI_CAN_TOUCAN       VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_TOUCAN)

// Freescale Star12 MSCAN
#define VCI_CAN_MSCAN        VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_MSCAN)

// Freescale FlexCAN
#define VCI_CAN_FLEXCAN      VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_FLEXCAN)

// IFI CAN Controller
#define VCI_CAN_IFICAN       VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_IFICAN)

// IFI CAN FD Controller
#define VCI_CAN_IFICFD       VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_IFICFD)

// Bosch C_CAN
#define VCI_CAN_BCCAN        VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_BCCAN)

// Bosch M_CAN
#define VCI_CAN_BMCAN       VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_BMCAN)

// ST BX_CAN
#define VCI_CAN_BXCAN       VCI_BUS_CTRL(VCI_BUS_CAN, CAN_CTRL_BXCAN)


//------------------------------------------------------------------------
// LIN controller types
//------------------------------------------------------------------------

// unknown LIN controller
#define VCI_LIN_UNKNOWN      VCI_BUS_CTRL(VCI_BUS_LIN, LIN_CTRL_UNKNOWN)


//------------------------------------------------------------------------
// K-Line controller types
//------------------------------------------------------------------------

// unknown K-Line controller
#define VCI_KLN_UNKNOWN      VCI_BUS_CTRL(VCI_BUS_KLN, KLN_CTRL_UNKNOWN)

// RS232 to K-Line converter
#define VCI_KLN_RS232        VCI_BUS_CTRL(VCI_BUS_KLN, KLN_CTRL_RS232)


//------------------------------------------------------------------------
// FlexRay controller types
//------------------------------------------------------------------------

// generic FlexRay controller
#define VCI_FXR_GENERIC      VCI_BUS_CTRL(VCI_BUS_FXR, FXR_CTRL_GENERIC)

// Freescale MFR4200, MFR4300, MFR4310
#define VCI_FXR_MFR4200      VCI_BUS_CTRL(VCI_BUS_FXR, FXR_CTRL_MFR4200)
#define VCI_FXR_MFR4300      VCI_BUS_CTRL(VCI_BUS_FXR, FXR_CTRL_MFR4300)
#define VCI_FXR_MFR4310      VCI_BUS_CTRL(VCI_BUS_FXR, FXR_CTRL_MFR4310)


/*****************************************************************************
 * VCI device capabilities
 ****************************************************************************/

typedef struct _VCIDEVICECAPS
{
  UINT16 BusCtrlCount;       // number of supported bus controllers
  UINT16 BusCtrlTypes[32];   // array of supported bus controllers
} VCIDEVICECAPS, *PVCIDEVICECAPS;

#define VCI_MAX_BUSCTRL 32   // maximum number of supported bus controllers


/*****************************************************************************
 * VCI device run-time status information
 ****************************************************************************/

typedef struct _VCIDEVRTINFO
{
  UINT32 dwCommId;  // ID of currently running communication layer
  UINT32 dwStatus;  // run-time status flags (see DEV_RTI_STAT_ constants)
  //tbd
} VCIDEVRTINFO, *PVCIDEVRTINFO;


//
// communication layer IDs
//
#define DEV_RTI_COID_NIL     0x00000000  // not used
#define DEV_RTI_COID_DAL     0x004C4144  // 'DAL'
#define DEV_RTI_COID_FLD     0x00444C46  // 'FLD'
#define DEV_RTI_COID_CCL     0x004C4343  // 'CCL'
#define DEV_RTI_COID_BAL     0x004C4142  // 'BAL'

//
// run-time status flags
//
#define DEV_RTI_STAT_LICEXP  0x00000001 // license run-time expired


#include <poppack.h>
#endif //_VCITYPE_H_
