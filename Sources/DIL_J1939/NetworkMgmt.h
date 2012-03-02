
/******************************************************************************
  Project       :  Frame_McNet
  FileName      :  NetworkMgmt.h
  Description   :  Network management layer services
  $Log:   X:/Archive/Sources/DIL_J1939/NetworkMgmt.h_v  $
 * 
 *    Rev 1.7   15 Apr 2011 18:48:38   CANMNTTM
 * Added RBEI Copyright information.
 * 
 *    Rev 1.6   02 Mar 2011 11:36:44   CANMNTTM
 * SetCallBackFuncPtr function is added.
 * 
 *    Rev 1.5   13 Jan 2011 14:47:00   CANMNTTM
 * GoOnline() return value is used.
 * 
 *    Rev 1.4   22 Dec 2010 19:23:42   CANMNTTM
 * 1. Implemented Call back mechanism.
 * 2. Exported function SetClBckFnPtrs and GetTimeOut added.
 * 
 *    Rev 1.3   15 Dec 2010 16:22:58   CANMNTTM
 * Added new function to remove all the register nodes.
 * 
 *    Rev 1.2   13 Dec 2010 18:46:54   CANMNTTM
 * New API DILJ_bIsOnline(void) added
 * 
 *    Rev 1.1   13 Dec 2010 16:37:06   CANMNTTM
 * Nodes are made independent of channel.
 * Now nodes can send message in any channel.
 * 
 *    Rev 1.0   06 Dec 2010 18:47:22   rac2kor
 *  
 * 
 *    Rev 1.4   12 Jan 2010 14:22:46   mcnetpl
 * Bugfixing
 * 
 *    Rev 1.3   04 Jan 2010 14:46:12   mcnetpl
 *  
 * 
 *    Rev 1.1   07 Dec 2009 13:02:00   mcnetpl
 * Bugfix
 * 
 *    Rev 1.0   02 Dec 2009 17:53:04   mcnetpl
 * Initial version

  Author(s)     :  Anish Kumar
  Date Created  :  15/09/2009
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

#if !defined NETWORKMGMT_H__INCLUDED
#define NETWORKMGMT_H__INCLUDED

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

#endif //NETWORKMGMT_H__INCLUDED