/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      DIL_Datatypes.h
 * \brief     Defines DIL related data types
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Defines DIL related data types
 */


#if !defined DIL_DATATYPES_H__INCLUDED_
#define DIL_DATATYPES_H__INCLUDED_

#define NO_SELECTION_HI 0xCDCD

#define CAN_MONITOR_NODE _T("CAN_MONITOR")
const int CAN_MONITOR_NODE_INDEX = 0;
const int CAN_MONITOR_CLIENT_ID  = 1;

#define J1939_MONITOR_NODE _T("J1939_MONITOR")
const UINT64 J1939_ECU_NAME     = 0x8000000000000001;

const BYTE MSGBUF_ADD = 0x1;
const BYTE MSGBUF_CLEAR = 0x0;

enum
{
    RESET_STATE = 0, INITIALISED, WAITING, NORMAL_ACTIVE, NORMAL_PASSIVE, NOT_DEFINED
};

enum ECONTR_PARAM
{
    NUMBER_HW = 0, NUMBER_CONNECTED_HW, PEAK_ERR_CNT, DRIVER_STATUS, ERR_CNT, HW_MODE, CNTR_STATUS, CON_TEST
};

//----------------------------------------------------------------------------
// declaration of Board Info structure
//----------------------------------------------------------------------------
typedef struct tagBOARDINFO
{
  WORD      m_wHWVersion;         // Hardware version e.g.: 01.00 as 0x100
  WORD      m_wFWVersion;         // Firmware version (0xFFFF if not available)
  WORD      m_wDDVersion;         // device driver version 
                                  // (0xFFFF if not available)
  WORD      m_wUCIVersion;        // UCI software version
  BYTE      m_bCanNum;            // Number of supported CAN controllers
  BYTE      m_bBftNum;            // Number of supported byteflight controllers 
  BYTE      m_bFlxNum;            // Number of supported FlexRay controllers  
  char      m_acSerialNum[16];    // hw serial number as string, e.g "12345678"
  char      m_acHardwareType[40]; // hw type as string e.g: "PCI04-ISA"
} s_BOARDINFO;
//----------------------------------------------------------------------------
// declaration of FlexRay status message
//----------------------------------------------------------------------------

typedef struct struct_STATUSMSG
{
  WORD  wControllerStatus;                  // Current controller state 
                                            // 0 := reset
                                            // 1 := stopped / initialized
                                            // 2 := started / waiting for startup completion
                                            // 3 := started / normal active (running)
                                            // 4 := started / normal passiv
                                            // 5 := started / halt mode
  DWORD dwStatusInfoFlags;                  // Flagfield of status information (UCI_FLXSTSINFO_???)
} s_STATUSMSG, *ps_STATUSMSG;

enum
{
    DRIVER_MCNET_PEAK_USB = 0,
    DIL_MCNET_TOTAL,
    DAL_MCNET_NONE = ~0x0
};

enum 
{
    DRIVER_CAN_STUB = 0,
    DRIVER_CAN_PEAK_USB,
    DRIVER_CAN_ICS_NEOVI,
    DRIVER_CAN_ETAS_BOA,
    DRIVER_CAN_ETAS_ES581,
    DRIVER_CAN_VECTOR_XL,
    DRIVER_CAN_KVASER_CAN,
    DIL_TOTAL,          // Its value must be <= MAX_DILS
    DAL_NONE            = ~0x0
};

typedef enum FILTER_TYPE
{
    PASS_FILTER,
    STOP_FILTER,  
}FILTER_TYPE;

typedef enum TXMODE
{
    SINGLE_SHOT,
    CONTINOUS,  
}TXMODE;

#define  MAX_CHAR_SHORT       128
#define  MAX_CHAR_LONG        512
#define  MAX_CHAR			  1024

typedef struct tagHwInterface
{
   DWORD    m_dwIdInterface;
   DWORD    m_dwVendor;
   BYTE     m_bytNetworkID;   
   TCHAR    m_acNameInterface[MAX_CHAR_SHORT];
   TCHAR    m_acDescription[MAX_CHAR_LONG];
   TCHAR    m_acDeviceName[MAX_CHAR_SHORT];
} INTERFACE_HW;

const int MAX_HW = 32;
typedef INTERFACE_HW INTERFACE_HW_LIST[MAX_HW];

typedef struct tagVersionInfo
{
   char     m_acDIL[MAX_CHAR_LONG];
   char     m_acController[MAX_CHAR_LONG];
   char     m_acDriver[MAX_CHAR_LONG];
} VERSIONINFO;


#define     MAX_DILS                16
#define     MAX_DILNAME             32

typedef struct 
{
    TCHAR   m_acName[MAX_DILNAME];
    DWORD   m_dwDriverID;
    UINT    m_ResourceID;
} DILINFO;

typedef DILINFO DILLIST[MAX_DILS];


#endif // DIL_DATATYPES_H__INCLUDED_