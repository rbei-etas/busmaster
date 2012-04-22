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
 * \file      NetworkMgmt.h
 * \brief     Definition file for NetworkMgmt
 * \author    Anish Kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for NetworkMgmt
 */

#pragma once

#include "NodeConManager.h"
#include "DIL_J1939_Extern.h"
#include "DIL_Interface/BaseDIL_CAN.h"
#include "ConnectionDet.h"
#include "ReadCanMsg.h"



typedef enum 
{
    CB_CON_CON,
    CB_CON_IND,
    CB_CONTEST_CON,
    CB_CONTEST_IND
}E_NW_CALLBK_TYPE;

typedef CMap<UINT, UINT, short, short> CCombineLCsToConNoMap;
typedef CMap<UINT64, UINT64, BYTE, BYTE> CNameAddressMap;

class CNetworkMgmt
{
private:
    CNetworkMgmt(void);
    CReadCanMsg m_ouReadCANMsg;
    Base_WrapperErrorLogger* m_pILog;
    CBaseDIL_CAN* m_pIDIL_CAN;
    CNodeConManager* m_ConMgrArr[DEF_MAX_SIMULATED_NODE];
    CCombineLCsToConNoMap m_LCsToConMap;
    int m_nConMgrCnt;
    CNameAddressMap m_odClaimedAdresMap;
    BOOL m_bOnline;
    
public:
    static UINT sg_unTO_BROADCAST;
    static UINT sg_unTO_RESPONSE;
    static UINT sg_unTO_HOLDING;
    static UINT sg_unTO_T1;
    static UINT sg_unTO_T2;
    static UINT sg_unTO_T3;
    static UINT sg_unTO_T4;

    ~CNetworkMgmt(void);

    void vDoInit(void);
    void vDoExit(void);
    // chnaged for incorporrating Buad Rate change.
    void vMInitReq(unsigned short BaudIndex);
    void vMInitCon(char cInitResult);
    void vMInitInd(char cInitResult,char cInitReason);
    void vMExitReq(void);
    void vMExitCon(char cExitResult);
    void vMDisconnectInd(short sLocalLc, short sRemoteLc, eREASON eReason);
    void vMErrorInd(short sLocalLc,short sRemoteLc,char cError);
    void vMBaudRateChange(unsigned short BaudTemp);
    void vMConnectionCon(short sLocalLc, short sRemoteLc, eCON_STATUS eConStatus);
    void vMConnectionInd(short sLocalLc, short sRemoteLc, eCON_STATUS eConStatus);
    ////////////////////////////////////////////////////////////////////
    CNodeConManager* pouGetConMagrObj(DWORD dwClientId);
    CNodeConManager* pouGetConMagrObj(TCHAR* pacClientName);
    CNodeConManager* pouGetConMagrObj(BYTE byIndex);
    BYTE byGetConMagrNumber(short shConNumber);
    //Singleton Class 
    static CNetworkMgmt& ouGetNWManagementObj();
    //
    void vSetLogAndICANPtr(Base_WrapperErrorLogger* pILog, CBaseDIL_CAN* pouIDIL_CAN);
    LONG lCreateNodeConManager(TCHAR* pacNodeName, 
                                UINT64 un64ECUName, 
                                BYTE byPrefAdres,
                                DWORD& dwClientId);
    void vRemoveAllNodes(void);
    LONG lRemoveNodeConManager(DWORD dwClientId);
    BOOL bRemoveConnectionFromConMap(short shConnectionNo);
    HRESULT StartAdresClaimProc(void);
    BYTE byGetUnclaimedAddress(int nStartFrom = 0);
    void vReinitAdresClaimProc(DWORD dwClient);
    void vUpdateAddressMap(UINT64 un64EcuName, BYTE byNodeAddress);
    BOOL bIsAddressClaimed(BYTE byAddress);
    void vGetNodeName(BYTE byAddress, TCHAR* acNodeName);
    BYTE byGetNodeAddress(DWORD dwClient);
    void vInitializeAllNodes(void);
    void vUnInitializeAllNodes(void);
    void vConfigureTimeOut(ETYPE_TIMEOUT eTimeOutType, UINT unMiliSeconds);
    void vGetTimeOut(ETYPE_TIMEOUT eTimeOutType, UINT& unMiliSeconds);
    HRESULT GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& unAbsTime);
    CBaseDIL_CAN* GetICANDIL(void);
    HRESULT GoOnline(BOOL bStart);
    BOOL bIsOnline(void);
};
