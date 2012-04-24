/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  NetworkMgmt.cpp
  Description   :  
  $Log:   X:/Archive/Sources/DIL_J1939/NetworkMgmt.cpv  $
   
  Author(s)     :  Pradeep Kadoor
  Date Created  :  23/11/2010
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

#include "DIL_J1939_stdafx.h"
#include "Include/DIL_CommonDefs.h"
#include "J1939_UtilityFuncs.h"
#include "NetworkMgmt.h"
#include "TransferLayer.h"
#include "MonitorNode.h"


#define BITS_IN_BYTE 8

UINT CNetworkMgmt::sg_unTO_BROADCAST    = TO_BROADCAST;
UINT CNetworkMgmt::sg_unTO_RESPONSE     = TO_RESPONSE;
UINT CNetworkMgmt::sg_unTO_HOLDING      = TO_HOLDING;
UINT CNetworkMgmt::sg_unTO_T1           = TO_T1;
UINT CNetworkMgmt::sg_unTO_T2           = TO_T2;
UINT CNetworkMgmt::sg_unTO_T3           = TO_T3;
UINT CNetworkMgmt::sg_unTO_T4           = TO_T4;
/**************************************************************
 ************************************************************** */
CNetworkMgmt::CNetworkMgmt()
{
    m_nConMgrCnt        = 0;
    m_pILog             = NULL;
    m_pIDIL_CAN         = NULL;
    for (int i = 0; i < DEF_MAX_SIMULATED_NODE; i++)
    {
        m_ConMgrArr[i]  = NULL;
    }
}

/**************************************************************
 ************************************************************** */

CNetworkMgmt::~CNetworkMgmt()
{
    for (int i = 0; i < DEF_MAX_SIMULATED_NODE; i++)
    {
        if (NULL != m_ConMgrArr[i])
        {
            delete m_ConMgrArr[i];
            m_ConMgrArr[i] = NULL;
        }
    }
}

void CNetworkMgmt::vDoInit(void)
{
    m_ouReadCANMsg.vDoInit();
}

void CNetworkMgmt::vDoExit(void)
{
    m_ouReadCANMsg.vDoExit();
}

/**************************************************************
 ************************************************************** */
CNetworkMgmt& CNetworkMgmt::ouGetNWManagementObj()
{
    static CNetworkMgmt souNWMangementObj;
    return souNWMangementObj;
}

/**************************************************************
 ************************************************************** */


void CNetworkMgmt::vMInitReq(unsigned short /*BaudIndex*/)
{
}


/**************************************************************
 ************************************************************** */

void CNetworkMgmt::vMInitCon(char /*cInitResult*/)
{
}


/**************************************************************
 ************************************************************** */

void CNetworkMgmt::vMInitInd(char /*cInitResult*/,char /*cInitReason*/)
{
	// not implemented right now.

}


/**************************************************************
 ************************************************************** */
void CNetworkMgmt::vMExitReq(void)
{
}


/**************************************************************
 ************************************************************** */

void CNetworkMgmt::vMExitCon(char /*cExitResult*/)
{
}
void CNetworkMgmt::vInitializeAllNodes(void)
{
    for (int i = 0; i < m_nConMgrCnt; i++)
    {
        CNodeConManager* pNodConMgr = m_ConMgrArr[i];
        
        if (pNodConMgr != NULL)
        {   
            pNodConMgr->vActivate();
        }
    }
}
HRESULT CNetworkMgmt::GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& unAbsTime)
{
    return m_pIDIL_CAN->DILC_GetTimeModeMapping(CurrSysTime, unAbsTime);
}
void CNetworkMgmt::vGetTimeOut(ETYPE_TIMEOUT eTimeOutType, UINT& unMiliSeconds)
{
    switch (eTimeOutType)
    {
        case TYPE_TO_BROADCAST:
        {
            unMiliSeconds = CNetworkMgmt::sg_unTO_BROADCAST;
        }
        break;
        case TYPE_TO_RESPONSE:
        {
            unMiliSeconds = CNetworkMgmt::sg_unTO_RESPONSE;
        }
        break;
        case TYPE_TO_HOLDING:
        {
            unMiliSeconds = CNetworkMgmt::sg_unTO_HOLDING;
        }
        break;
        case TYPE_TO_T1:
        {
            unMiliSeconds = CNetworkMgmt::sg_unTO_T1;
        }
        break;
        case TYPE_TO_T2:
        {
            unMiliSeconds = CNetworkMgmt::sg_unTO_T2;
        }
        break;
        case TYPE_TO_T3:
        {
            unMiliSeconds = CNetworkMgmt::sg_unTO_T3;
        }
        break;
        case TYPE_TO_T4:
        {
            unMiliSeconds = CNetworkMgmt::sg_unTO_T4;
        }
        break;
    }
}
void CNetworkMgmt::vConfigureTimeOut(ETYPE_TIMEOUT eTimeOutType, UINT unMiliSeconds)
{
    switch (eTimeOutType)
    {
        case TYPE_TO_BROADCAST:
        {
            CNetworkMgmt::sg_unTO_BROADCAST = unMiliSeconds;
        }
        break;
        case TYPE_TO_RESPONSE:
        {
            CNetworkMgmt::sg_unTO_RESPONSE = unMiliSeconds;
        }
        break;
        case TYPE_TO_HOLDING:
        {
            CNetworkMgmt::sg_unTO_HOLDING = unMiliSeconds;
        }
        break;
        case TYPE_TO_T1:
        {
            CNetworkMgmt::sg_unTO_T1 = unMiliSeconds;
        }
        break;
        case TYPE_TO_T2:
        {
            CNetworkMgmt::sg_unTO_T2 = unMiliSeconds;
        }
        break;
        case TYPE_TO_T3:
        {
            CNetworkMgmt::sg_unTO_T3 = unMiliSeconds;
        }
        break;
        case TYPE_TO_T4:
        {
            CNetworkMgmt::sg_unTO_T4 = unMiliSeconds;
        }
        break;
    }
}
void CNetworkMgmt::vUnInitializeAllNodes(void)
{
    for (int i = 0; i < m_nConMgrCnt; i++)
    {
        CNodeConManager* pNodConMgr = m_ConMgrArr[i];
        
        if (pNodConMgr != NULL)
        {   
            pNodConMgr->vDeactivate();
        }
    }
    m_odClaimedAdresMap.RemoveAll();
}
BYTE CNetworkMgmt::byGetNodeAddress(DWORD dwClient)
{
    BYTE byAddress = ADDRESS_NULL;
    for (int i = 0; i < m_nConMgrCnt; i++)
    {
        CNodeConManager* pNodConMgr = m_ConMgrArr[i];
        
        if (pNodConMgr != NULL)
        {   
            if (pNodConMgr->m_dwClientID == dwClient)
            {
                i = m_nConMgrCnt; //break the loop
                byAddress = pNodConMgr->byGetNodeAddress();
            }           
        }
    }
    return byAddress;
}
void CNetworkMgmt::vGetNodeName(BYTE byAddress, TCHAR* acNodeName)
{
    for (int i = 0; i < m_nConMgrCnt; i++)
    {
        CNodeConManager* pNodConMgr = m_ConMgrArr[i];
        
        if (pNodConMgr != NULL)
        {   
            if (pNodConMgr->byGetNodeAddress() == byAddress)
            {
                i = m_nConMgrCnt; //break the loop
                _tcscpy(acNodeName, pNodConMgr->m_acNodeName);
            }
        }
    }
}
BOOL CNetworkMgmt::bIsOnline(void)
{
    return m_bOnline;
}
BOOL CNetworkMgmt::bIsAddressClaimed(BYTE byAddress)
{
    BOOL bResult = FALSE;
    POSITION pos = m_odClaimedAdresMap.GetStartPosition();
    UINT64 unECU_NAME;
    BYTE byNodeAddress;
    while (pos != NULL)
    {
        m_odClaimedAdresMap.GetNextAssoc(pos, unECU_NAME, byNodeAddress);
        if (byNodeAddress == byAddress)
        {
            pos = NULL; //break the loop
            bResult = TRUE;
        }
    }
    return bResult;
}
BYTE CNetworkMgmt::byGetUnclaimedAddress(int nStartFrom)
{
    BYTE byAdres = ADDRESS_NULL;
    
    for (BYTE i = (BYTE)nStartFrom; i < ADDRESS_NULL; i++)
    {
        if (!bIsAddressClaimed(i))
        {
            byAdres = i;
            i = ADDRESS_NULL;
        }
    }
    return byAdres;
}

HRESULT CNetworkMgmt::StartAdresClaimProc(void)
{
    //Give a random delay to start
    HRESULT hResult = S_OK;
    UINT unRandomWait = rand() % sg_unTO_RESPONSE;
    Sleep(unRandomWait);
    static BYTE byPreferedAdres = 0;
    for (int i = 0; i < m_nConMgrCnt; i++)
    {
        CNodeConManager* pNodConMgr = m_ConMgrArr[i];
        
        if (pNodConMgr != NULL)
        {   
            byPreferedAdres = pNodConMgr->m_byPrefAddress;
            if (bIsAddressClaimed(byPreferedAdres) == TRUE)
            {
                byPreferedAdres = byGetUnclaimedAddress(i);
            }
            hResult |= pNodConMgr->StartAdresClaimProc(byPreferedAdres);
        }
    }
    return hResult;
}

void CNetworkMgmt::vUpdateAddressMap(UINT64 un64EcuName, BYTE byNodeAddress)
{
    m_odClaimedAdresMap[un64EcuName] = byNodeAddress;
}

void CNetworkMgmt::vReinitAdresClaimProc(DWORD dwClient)
{
    CNodeConManager* pNodConMgr = pouGetConMagrObj(dwClient);

    if (pNodConMgr != NULL)
    {   
        BYTE byPreferedAdres = byGetUnclaimedAddress();
        pNodConMgr->StartAdresClaimProc(byPreferedAdres);
    }
}
HRESULT CNetworkMgmt::GoOnline(BOOL bStart)
{
    HRESULT hResult = S_OK;
    if ((bStart == TRUE) && (m_bOnline != TRUE))
    {
        vInitializeAllNodes();
        if ((hResult = StartAdresClaimProc()) == S_OK)
        {
            m_bOnline = TRUE;
        }
    }
    else if ((bStart == FALSE) && (m_bOnline == TRUE))
    {
        vUnInitializeAllNodes();
        m_bOnline = FALSE;
    }
    return hResult;
}
/**************************************************************
 ************************************************************** */
void CNetworkMgmt::vMDisconnectInd(short /*sLocalLc*/, short /*sRemoteLc*/, eREASON /*eReason*/)
{
    /*if (m_pfDisconInd != NULL)
    {
        (*m_pfDisconInd)(sLocalLc, sRemoteLc, eReason);
    }kadoor*/
}
/**************************************************************
 ************************************************************** */

void CNetworkMgmt::vMConnectionCon(short /*sLocalLc*/, short /*sRemoteLc*/,
                                   eCON_STATUS eConStatus)
{
    if (eConStatus == T_DISCONNECTED)
    {
        //SHORT shConNo = shGetConNo(sLocalLc, sRemoteLc);
        //bRemoveConnectionFromConMap(shConNo);
    }
    /*if (m_pfConConf != NULL)
    {
        
        (*m_pfConConf)(sLocalLc, sRemoteLc, eWdStatus, eConStatus);        
    }kadoor*/
}

/**************************************************************
 **************************************************************/
void CNetworkMgmt::vMConnectionInd(short /*sLocalLc*/, short /*sRemoteLc*/,
                                   eCON_STATUS /*eConStatus*/)
{
    /*if (m_pfConInd != NULL)
    {
        (*m_pfConInd)(sLocalLc, sRemoteLc, eWdStatus, eConStatus);
    }kadoor*/
}
/**************************************************************
 ************************************************************** */

void CNetworkMgmt::vMErrorInd(short /*sLocalLc*/,short /*sRemoteLc*/,char /*cError*/)
{
	// to be clarified with handling of physical layer.	
	//as on now not used.

}


/**************************************************************
 ************************************************************** */

void CNetworkMgmt::vMBaudRateChange(unsigned short /*BaudTemp*/)
{
    //May not be required
}



/**************************************************************
 ************************************************************** */
BOOL CNetworkMgmt::bRemoveConnectionFromConMap(short shConnectionNo)
{
    BOOL bReturn = FALSE;
    UINT unLc;
    short shCon;
    POSITION pos = m_LCsToConMap.GetStartPosition();
    while (pos)
    {
        m_LCsToConMap.GetNextAssoc(pos, unLc, shCon );
        if (shCon == shConnectionNo)
        {
            m_LCsToConMap.RemoveKey(unLc);
            bReturn = TRUE;
            break;
        }
    }
    return bReturn;
}

CNodeConManager* CNetworkMgmt::pouGetConMagrObj(TCHAR* pacClientName)
{
    CNodeConManager* pNodeConMgr = NULL;
    for(int i = 0; i < DEF_MAX_SIMULATED_NODE; i++)
    {
        if (NULL != m_ConMgrArr[i])
        {
            if (_tcscmp(pacClientName, m_ConMgrArr[i]->m_acNodeName) == 0)
            {
                pNodeConMgr = m_ConMgrArr[i];
                break;
            }
        }
    }
    return pNodeConMgr;
}
/**************************************************************
 ************************************************************** */
CNodeConManager* CNetworkMgmt::pouGetConMagrObj(DWORD dwClientId)
{
    CNodeConManager* pNodeConMgr = NULL;
    for(int i = 0; i < DEF_MAX_SIMULATED_NODE; i++)
    {
        if (NULL != m_ConMgrArr[i])
        {
            if (dwClientId == m_ConMgrArr[i]->m_dwClientID)
            {
                pNodeConMgr = m_ConMgrArr[i];
                break;
            }
        }
    }
    return pNodeConMgr;
}
/**************************************************************
 ************************************************************** */
CNodeConManager* CNetworkMgmt::pouGetConMagrObj(BYTE byIndex)
{
    CNodeConManager* pNodeConMgr = NULL;
    if (byIndex < DEF_MAX_SIMULATED_NODE)
    {
        pNodeConMgr = m_ConMgrArr[byIndex];
    }
    return pNodeConMgr;
}
/**************************************************************
 ************************************************************** */
BYTE CNetworkMgmt::byGetConMagrNumber(short shConNumber)
{
    //Second byte is connectionmgr's no.
    SHORT shConMgrNo = (shConNumber & 0xFF00);
    shConMgrNo = shConMgrNo >> BITS_IN_BYTE;
    return (BYTE)shConMgrNo;
}
/**************************************************************
 ************************************************************** */
 void CNetworkMgmt::vSetLogAndICANPtr(Base_WrapperErrorLogger* pILog, CBaseDIL_CAN* pIDIL_CAN)
 {
    m_pILog = pILog;
    m_pIDIL_CAN = pIDIL_CAN;
    CTransferLayer::ouGetTransLayerObj().vSetIDIL_CAN(pIDIL_CAN);
 }

/**************************************************************
************************************************************** */
LONG CNetworkMgmt::lCreateNodeConManager(TCHAR* pacNodeName, 
                                         UINT64 un64ECUName,
                                         BYTE   byPrefAdres,
                                         DWORD& dwClientId)
{
    VALIDATE_POINTER_RETURN_VAL(m_pIDIL_CAN, S_FALSE);
    LONG lResult = S_OK;
    CNodeConManager* pNodeConMgr = NULL;
    //Search the array if node is already present
    pNodeConMgr = pouGetConMagrObj(pacNodeName);
    if (pNodeConMgr != NULL)
    {
        lResult = ERR_CLIENT_EXISTS;
        dwClientId = pNodeConMgr->m_dwClientID;
    }
    //If no. of node reached limit
    else if(m_nConMgrCnt == DEF_MAX_SIMULATED_NODE)
    {
        lResult = ALLOWED_NOMORE;
    }    
    //Create new node
    if (lResult == S_OK)
    {
        //Find the first available position in the Array. There will
        //be empty places in beween due to deletion 
        int nEmptyPos = 0;
        for (int i = 0; i < DEF_MAX_SIMULATED_NODE; i++)
        {
            if (m_ConMgrArr[i] == NULL)
            {
                nEmptyPos = i;
                break;
            }
        }
        if ( _tcscmp(pacNodeName, J1939_MONITOR_NODE) == 0 )
        {
            pNodeConMgr = new CMonitorNode(nEmptyPos, pacNodeName, un64ECUName, byPrefAdres);
        }
        else
        {
            //Provide its position in array and its PGN
            pNodeConMgr = new CNodeConManager(nEmptyPos, pacNodeName, un64ECUName, byPrefAdres);
        }
        if (pNodeConMgr != NULL)
        {
            m_ConMgrArr[nEmptyPos] = pNodeConMgr;
            m_nConMgrCnt++; 
            HRESULT hResult = S_FALSE;
            // Connection manager is created. Proceed with rest of the procedures
            if ( _tcscmp(pacNodeName, J1939_MONITOR_NODE) == 0 )
            {
                hResult = m_pIDIL_CAN->DILC_RegisterClient(TRUE, dwClientId, 
                                                                    CAN_MONITOR_NODE);
                if (hResult == ERR_CLIENT_EXISTS)
                {
                    hResult = S_OK;
                }
            }
            else
            {
                hResult = m_pIDIL_CAN->DILC_RegisterClient(TRUE, dwClientId, 
                                                                    pacNodeName);
            }
            //ASSERT(hResult == S_OK);
            if ((hResult == S_OK) || (hResult == ERR_CLIENT_EXISTS))
            {
                CBaseCANBufFSE* pouBuffer = pNodeConMgr->pouGetBuf();
                hResult = m_pIDIL_CAN->DILC_ManageMsgBuf(MSGBUF_ADD, dwClientId, pouBuffer);
                ASSERT(hResult == S_OK);
                if (hResult == S_OK)
                {
                    pNodeConMgr->m_dwClientID = dwClientId;
                    m_ouReadCANMsg.AddEventHandle(pouBuffer->hGetNotifyingEvent(), (BYTE)nEmptyPos);
                }
                //Join this node to network if started.
                if (m_bOnline == TRUE)
                {
                    pNodeConMgr->vActivate();
                    BYTE byPrefAddress = pNodeConMgr->m_byPrefAddress;
                    
                    if ((byPrefAddress >= ADDRESS_NULL) || (bIsAddressClaimed(byPrefAddress) == TRUE))
                    {
                        byPrefAddress = byGetUnclaimedAddress();
                    }
                    pNodeConMgr->StartAdresClaimProc(byPrefAddress);
                }
            }
        }
        else
        {
            ASSERT(FALSE);
        }
    }
    return lResult;
}

/**************************************************************
************************************************************** */
LONG CNetworkMgmt::lRemoveNodeConManager(DWORD dwClientId)
{
    LONG lResult = ERROR_NOCLIENT;
    for(int i = 0; i < DEF_MAX_SIMULATED_NODE; i++)
    {
        if (NULL != m_ConMgrArr[i])
        {
            if (dwClientId == m_ConMgrArr[i]->m_dwClientID)
            {
                CBaseCANBufFSE* pouBuffer = m_ConMgrArr[i]->pouGetBuf();
                if (NULL != pouBuffer)
                {
                    HANDLE handle = pouBuffer->hGetNotifyingEvent();
                    TRACE("Going to call bDeleteEventHandle\n");
                    m_ouReadCANMsg.bDeleteEventHandle(handle);
                    TRACE("Called bDeleteEventHandle\n");
                }
                CString omClientName = _T("");
                if (m_ConMgrArr[i]->m_dwClientID != CAN_MONITOR_CLIENT_ID) //Do not remove client from CAN if monitor
                {
					m_pIDIL_CAN->DILC_RegisterClient(FALSE, m_ConMgrArr[i]->m_dwClientID, omClientName.GetBuffer(MAX_CHAR));
                }
                else //Do not remove client from CAN if monitor, remove only the buffer
                {
                    CBaseCANBufFSE* pouBuffer = m_ConMgrArr[i]->pouGetBuf();
                    m_pIDIL_CAN->DILC_ManageMsgBuf(MSGBUF_CLEAR, m_ConMgrArr[i]->m_dwClientID, pouBuffer);
                }
                m_ConMgrArr[i]->vRemoveAllConnections();
                delete m_ConMgrArr[i];
                m_ConMgrArr[i] = NULL;
                m_nConMgrCnt--;
                lResult = S_OK;
            }
        }
    }
    return lResult;
}

CBaseDIL_CAN* CNetworkMgmt::GetICANDIL(void)
{
    return m_pIDIL_CAN;
}

void CNetworkMgmt::vRemoveAllNodes(void)
{
    for(int i = 0; i < DEF_MAX_SIMULATED_NODE; i++)
    {
        if (NULL != m_ConMgrArr[i])
        {
            CBaseCANBufFSE* pouBuffer = m_ConMgrArr[i]->pouGetBuf();
            if (NULL != pouBuffer)
            {
                HANDLE handle = pouBuffer->hGetNotifyingEvent();
                TRACE("Going to call bDeleteEventHandle\n");
                m_ouReadCANMsg.bDeleteEventHandle(handle);
                TRACE("Called bDeleteEventHandle\n");
            }
            CString omClientName = _T("");
            
            if (m_ConMgrArr[i]->m_dwClientID != CAN_MONITOR_CLIENT_ID) 
            {
				m_pIDIL_CAN->DILC_RegisterClient(FALSE, m_ConMgrArr[i]->m_dwClientID, omClientName.GetBuffer(MAX_CHAR));
            }
            else //Do not remove client from CAN if monitor, remove only the buffer
            {
                CBaseCANBufFSE* pouBuffer = m_ConMgrArr[i]->pouGetBuf();
                m_pIDIL_CAN->DILC_ManageMsgBuf(MSGBUF_CLEAR, m_ConMgrArr[i]->m_dwClientID, pouBuffer);
            }
            m_ConMgrArr[i]->vDeactivate();
            m_ConMgrArr[i]->vRemoveAllConnections();
            delete m_ConMgrArr[i];
            m_ConMgrArr[i] = NULL;
            m_nConMgrCnt--;
        }
    }
}

