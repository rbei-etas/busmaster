/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  AppServicesJ1939.h
  Description   :  
  $Log:   X:/Archive/Sources/NodeSimEx/AppServicesJ1939.h_v  $
 * 
 *    Rev 1.1   15 Apr 2011 18:59:10   CANMNTTM
 * Added RBEI Copyright information.
 * 
 *    Rev 1.0   01 Mar 2011 17:07:52   CANMNTTM
 *  
  Author(s)     :  
  Date Created  :  
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/
#ifndef APPSERVICESJ1939_H__INCLUDED_
#define APPSERVICESJ1939_H__INCLUDED_

#include "AppServices.h"

UINT g_unGoOnline_J1939(BOOL bEnable, HMODULE hModule);

UINT g_unSendMsg_J1939(STJ1939_MSG* psTxMsg, HMODULE hModule);

UINT g_unRequestPGN_J1939(UINT PGN, BYTE DestAdres, UINT Channel, HMODULE hModule);

UINT g_unSendAckMsg_J1939(UINT PGN, BYTE AckType, BYTE DestAdres, UINT Channel, 
                                                                    HMODULE hModule);

UINT g_unClaimAddress_J1939(BYTE Address, UINT Channel, HMODULE hModule);

UINT g_unRequestAddress_J1939(UINT Channel, HMODULE hModule);

UINT g_unCommandAddress_J1939(UINT64 EcuName, BYTE NewAddress, UINT Channel, 
                                                                HMODULE hModule);

UINT g_unSetTimeout_J1939(BYTE TimeoutType, UINT TimeoutValue, HMODULE hModule);

#endif // APPSERVICESJ1939_H__INCLUDED_
