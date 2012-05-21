/*****************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  Export_UserDllJ1939.h
  Description   :  This header file contains name and prototype of the 
                   exported function from user-defined DLL which should be 
                   called by the application to initialise a function pointer
                   in that DLL to enable it to call a function from the 
                   application.
  $Log:   X:/Archive/Sources/NodeSimEx/Export_UserDllJ1939.h_v  $
 * 
 *    Rev 1.1   15 Apr 2011 18:59:12   CANMNTTM
 * Added RBEI Copyright information.
 * 
 *    Rev 1.0   01 Mar 2011 17:07:52   CANMNTTM
 *  
  Author(s)     :  
  Date Created  :  28.11.2002
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*****************************************************************************/
#include "Export_UserDll.h"

typedef UINT (*APPSENDPROC_J) (STJ1939_MSG*, HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLSENDPROC_J)(APPSENDPROC_J);

typedef UINT (*APPONLINEOFFLINE_J) (BOOL, HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLONLINEOFFLINE_J)(APPONLINEOFFLINE_J);

typedef UINT (*APPREQUESTPGN_J) (UINT, BYTE, UINT, HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLREQUESTPGN_J)(APPREQUESTPGN_J);

typedef UINT (*APPACKMSG_J) (UINT, BYTE, BYTE, UINT, HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLACKMSG_J)(APPACKMSG_J);

typedef UINT (*APPCLAIMADDRESS_J) (BYTE, UINT, HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLCLAIMADDRESS_J)(APPCLAIMADDRESS_J);

typedef UINT (*APPRQSTADDRESS_J) (UINT, HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLRQSTADDRESS_J)(APPRQSTADDRESS_J);

typedef UINT (*APPCMDADDRESS_J) (UINT64, BYTE, UINT, HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLCMDADDRESS_J)(APPCMDADDRESS_J);

typedef UINT (*APPSETTIMEOUT_J) (BYTE, UINT, HMODULE);
// Prototype of the exported function from user-defined DLL
typedef void (*DLLSETTIMEOUT_J)(APPSETTIMEOUT_J);

// Name of the exported function from user-defined DLL
#define NAME_FUNC_SENGMSG_J      "vSetSendMsgProcAddress"
#define NAME_FUNC_ONLINE_J       "vSetGoOnlineProcAddress"
#define NAME_FUNC_OFFLINE_J      "vSetGoOfflineProcAddress"
#define NAME_FUNC_REQUESTPGN_J   "vSetRequestPGNProcAddress"
#define NAME_FUNC_ACKMSG_J       "vSetSendAckMsgProcAddress"
#define NAME_FUNC_CLAIMADDRESS_J "vSetClaimAddressProcAddress"
#define NAME_FUNC_RQSTADDRESS_J  "vSetRequestAddressProcAddress"
#define NAME_FUNC_CMDADDRESS_J   "vSetCommandAddressProcAddress"
#define NAME_FUNC_TIMEOUT_J      "vSetSetTimeoutProcAddress"