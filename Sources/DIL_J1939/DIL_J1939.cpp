/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  J1939Translator.cpp
  Description   :  Defines the initialization routines for the DLL.
  $Log:   X:/Archive/Sources/DIL_J1939/DIL_J1939.cpv  $
   
      Rev 1.10   07 Jun 2011 11:11:16   CANMNTTM
   All W4 level Warnings Are removed
   
   
      Rev 1.9   15 Apr 2011 18:48:34   CANMNTTM
   Added RBEI Copyright information.
   
      Rev 1.8   08 Apr 2011 17:30:00   CANMNTTM
   Clear Message buffer function modified.
   
      Rev 1.7   02 Mar 2011 11:36:44   CANMNTTM
   SetCallBackFuncPtr function is added.
   
      Rev 1.6   13 Jan 2011 14:47:00   CANMNTTM
   GoOnline() return value is used.
   
      Rev 1.5   23 Dec 2010 16:52:20   CANMNTTM
   Macro MAX_MSG_LEN_J1939
    instead of MAX_DATA_LEN_J1939 wherever applicable.
   
      Rev 1.4   22 Dec 2010 19:23:42   CANMNTTM
   1. Implemented Call back mechanism.
   2. Exported function SetClBckFnPtrs and GetTimeOut added.
   
      Rev 1.3   15 Dec 2010 16:22:58   CANMNTTM
   Added new function to remove all the register nodes.
   
      Rev 1.2   13 Dec 2010 18:46:54   CANMNTTM
   New API DILJ_bIsOnline(void) added
   
      Rev 1.1   13 Dec 2010 16:37:06   CANMNTTM
   Nodes are made independent of channel.
   Now nodes can send message in any channel.
   
      Rev 1.0   06 Dec 2010 18:47:20   rac2kor
    
  Author(s)     :  Pradeep Kadoor
  Date Created  :  26/10/2010
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

// MCNetTranslator.cpp : 
//

#include "DIL_J1939_stdafx.h"
#include "J1939_UtilityFuncs.h"
#include "DIL_Interface/BaseDIL_CAN.h"

#define USAGE_EXPORT

#include "Datatypes/J1939_DataTypes.h"
#include "DIL_J1939_Extern.h"
#include "DIL_J1939.h"
#include "NetworkMgmt.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CMCNetTranslatorApp

BEGIN_MESSAGE_MAP(CJ1939TranslatorApp, CWinApp)
END_MESSAGE_MAP()


// CMCNetTranslatorApp construction

CJ1939TranslatorApp::CJ1939TranslatorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMCNetTranslatorApp object

CJ1939TranslatorApp theApp;


// CMCNetTranslatorApp initialization

BOOL CJ1939TranslatorApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}



/*pouNetwork – The object containing assembly information. 
If null, then only monitor mode of node simulation is allowed.
pILog – Error logging interface. Can be null.*/
USAGEMODE HRESULT DILJ_Initialise(Base_WrapperErrorLogger* pILog, CBaseDIL_CAN* pouIDIL_CAN)
{
    HRESULT hResult = S_OK;
    CNetworkMgmt::ouGetNWManagementObj().vSetLogAndICANPtr(pILog, pouIDIL_CAN);
    CNetworkMgmt::ouGetNWManagementObj().vDoInit();
    return hResult;
}

/*Performs all uninitialisation / closure operations*/
USAGEMODE HRESULT DILJ_Uninitialise(void)
{
    HRESULT hResult = S_OK;
    CNetworkMgmt::ouGetNWManagementObj().vRemoveAllNodes();
    CNetworkMgmt::ouGetNWManagementObj().vDoExit();
    return hResult;
}

/*Registers / unregisters a client. This is necessary to simulate a node
and to receive messages. Only registered client's buffer will be updated
on receive of a msg in the bus.*/
USAGEMODE HRESULT DILJ_RegisterClient(BOOL bRegister, TCHAR* pacNodeName, 
                                      UINT64 un64ECUName, BYTE byPrefAdres,
                                      DWORD& dwClientId)
{
    HRESULT hResult = S_OK;
    if (bRegister == TRUE)
    {
        hResult = CNetworkMgmt::ouGetNWManagementObj().lCreateNodeConManager(pacNodeName, un64ECUName, byPrefAdres, dwClientId);
    }
    else
    {
        hResult = CNetworkMgmt::ouGetNWManagementObj().lRemoveNodeConManager(dwClientId);
    }

    return hResult;
}

/* Manages the target client buffer list. Call this function to open a 
data channel to receive messages.*/
USAGEMODE HRESULT DILJ_ManageMsgBuf(BYTE bAction, DWORD dwClientID, 
                                        CBaseMsgBufVSE* pBufObj)
{
    HRESULT hResult = INVALID_PARAM;
    if (pBufObj != NULL)
    {
        CNodeConManager* pConManager = 
            CNetworkMgmt::ouGetNWManagementObj().pouGetConMagrObj(dwClientID);
        if (pConManager != NULL)
        {
            if (bAction == MSGBUF_ADD)
            {
                pConManager->lAddMsgBuffer(pBufObj);
            }
            else if (bAction == MSGBUF_CLEAR)
            {
                pConManager->vClearMsgBuffer(pBufObj);
            }
            hResult = S_OK;
        }
        else
        {
            hResult = ERROR_NOCLIENT;
        }
    }
    return hResult;
}

/*To get the version information.*/
USAGEMODE void DILJ_GetVersionInfo (VERSIONINFO& /*sVerInfo*/)
{
}

/* Sends a J1939 message*/
USAGEMODE HRESULT DILJ_SendJ1939Msg (DWORD dwClientId, UINT unChannel, EJ1939_MSG_TYPE eMsgType, UINT32 unPGN,
                                     BYTE* pbyData, UINT unDLC, BYTE byPriority, BYTE bySrc, BYTE byDestAdres)
{
    HRESULT hResult = S_FALSE;
    CNodeConManager* pManager = NULL;
    pManager = CNetworkMgmt::ouGetNWManagementObj().pouGetConMagrObj(dwClientId);
    if (NULL != pManager && pManager->bIsActive())
    {
        hResult = S_OK;
        if (pManager->bIsMonitorNode())
        {
            pManager->vSetNodeAddress(bySrc);
        }
        pManager->vSendMessage(unChannel, eMsgType, unPGN, pbyData, unDLC, byPriority, byDestAdres);
        
    }
    return hResult;
}
/* Gets the node name from 8 bit address from J1939 network.*/
USAGEMODE HRESULT DILJ_NM_GetNodeName(BYTE byAddress, TCHAR* acNodeName)
{
    CNetworkMgmt::ouGetNWManagementObj().vGetNodeName(byAddress, acNodeName);
    return S_OK;
}
USAGEMODE HRESULT DILJ_NM_GetByteAddres(BYTE& byAddress, DWORD dwClient)
{
    byAddress = CNetworkMgmt::ouGetNWManagementObj().byGetNodeAddress(dwClient);
    return S_OK;
}
/* Requests address from the node.*/
USAGEMODE HRESULT DILJ_NM_RequestAddress(DWORD dwClient, UINT unChannel, BYTE byPriority,
                                         BYTE bySrc, BYTE byDestAddress)
{
    HRESULT hResult = S_FALSE; 
    CNodeConManager* pManager = NULL;
    pManager = CNetworkMgmt::ouGetNWManagementObj().pouGetConMagrObj(dwClient);
    if (NULL != pManager && pManager->bIsActive())
    {
        hResult = S_OK;
        if (pManager->bIsMonitorNode())
        {
            pManager->vSetNodeAddress(bySrc);
        }
        pManager->vSendMessage(unChannel, MSG_TYPE_NM_RQST_ACL, 0, NULL, 0, byPriority, byDestAddress);
    }
    return hResult;
}
/* Returns whether the address is already claimed by another node.*/
USAGEMODE BOOL DILJ_NM_bIsAddressClaimed(BYTE byAddress)
{
    return CNetworkMgmt::ouGetNWManagementObj().bIsAddressClaimed(byAddress);
}
/* Tries to claim 8 bit address to a corresponding node.*/
USAGEMODE HRESULT DILJ_NM_ClaimAddress (DWORD dwClientId, UINT unChannel, BYTE byAddress, BYTE byPriority)
{
    HRESULT hResult = S_FALSE; 
    CNodeConManager* pManager = NULL;
    pManager = CNetworkMgmt::ouGetNWManagementObj().pouGetConMagrObj(dwClientId);
    if (NULL != pManager && pManager->bIsActive())
    {
        hResult = S_OK;
        if (pManager->bIsMonitorNode())
        {
            pManager->vSetNodeAddress(byAddress);
        }
        pManager->vSendMessage(unChannel, MSG_TYPE_NM_ACL, byAddress, 0, NULL, byPriority, ADDRESS_ALL);
    }
    return hResult;
}

/* Commands a node with perticular NAME to assume a address.*/
USAGEMODE HRESULT DILJ_NM_CommandAddress(DWORD dwClient, UINT unChannel, UINT64 unECU_NAME, 
                                         BYTE byNewAddress, BYTE byPriority, BYTE bySrc, BYTE byDestAddress)
{
    HRESULT hResult = S_FALSE; 
    CNodeConManager* pManager = NULL;
    pManager = CNetworkMgmt::ouGetNWManagementObj().pouGetConMagrObj(dwClient);
    if (NULL != pManager && pManager->bIsActive())
    {
        hResult = S_OK;
        if (pManager->bIsMonitorNode())
        {
            pManager->vSetNodeAddress(bySrc);
        }
        BYTE abyFrame[DATA_LEN_CMD_ADDRESS]  = {0xFF};
        memcpy(abyFrame, &unECU_NAME, MAX_FRAME_DATA_SIZE);
        abyFrame[DATA_LEN_CMD_ADDRESS - 1] = byNewAddress;
        pManager->vSendMessage(unChannel, MSG_TYPE_NM_CMD_ADDRESS, 0, abyFrame, DATA_LEN_CMD_ADDRESS, byPriority, byDestAddress);
    }
    return hResult;
}
/* Requests a PGN from the node.*/
USAGEMODE HRESULT DILJ_RequestPGN(DWORD dwClient, UINT unChannel, UINT32 unPGN, 
                                  BYTE byPriority, BYTE bySrc, BYTE byDestAddress)
{
    HRESULT hResult = S_FALSE; 
    CNodeConManager* pManager = NULL;
    pManager = CNetworkMgmt::ouGetNWManagementObj().pouGetConMagrObj(dwClient);
    if (NULL != pManager && pManager->bIsActive())
    {
        hResult = S_OK;
        if (pManager->bIsMonitorNode())
        {
            pManager->vSetNodeAddress(bySrc);
        }
        pManager->vSendMessage(unChannel, MSG_TYPE_REQUEST, unPGN, 0, 0, byPriority, byDestAddress);
    }
    return hResult;
}
/* Sends Positive/Negative acknowledgement msg.*/
USAGEMODE HRESULT DILJ_SendAckMsg(DWORD dwClient, UINT unChannel, ETYPE_ACK eAckType, UINT32 unPGN, BYTE bySrc, BYTE byAddresAck)
{
    HRESULT hResult = S_FALSE; 
    CNodeConManager* pManager = NULL;
    pManager = CNetworkMgmt::ouGetNWManagementObj().pouGetConMagrObj(dwClient);
    if (NULL != pManager && pManager->bIsActive())
    {
        hResult = S_OK;
        if (pManager->bIsMonitorNode())
        {
            pManager->vSetNodeAddress(bySrc);
        }
        BYTE abyFrame[MAX_FRAME_DATA_SIZE];
        switch (eAckType)
        {
            case ACK_POS:
            {
                abyFrame[0] = CB_ACK_POS;
                abyFrame[4] = byAddresAck;
            }
            break;
            case ACK_NEG:
            {
                abyFrame[0] = CB_ACK_NEG;
                abyFrame[4] = ADDRESS_NULL;
            }
            break;
        }
        abyFrame[1] = 0xFF;
        abyFrame[2] = 0xFF;
        abyFrame[3] = 0xFF;
        
        abyFrame[5] = (BYTE)unPGN;
        abyFrame[6] = (BYTE)(unPGN >> 8);
        abyFrame[7] = (BYTE)(unPGN >> 16);
        pManager->vSendMessage(unChannel, MSG_TYPE_ACKNOWLEDGEMENT, 
            unPGN, abyFrame, 0, DEFAULT_PRIORITY, byAddresAck);
    }
    return hResult;
}
/* Starts J1939 network. All nodes start sending according to the configuration.*/
USAGEMODE HRESULT DILJ_GoOnline()
{
    return CNetworkMgmt::ouGetNWManagementObj().GoOnline(TRUE);
}

/* Stops J1939 network. All nodes stop sending msgs.*/
USAGEMODE HRESULT DILJ_GoOffline()
{
    //HRESULT hResult = S_OK;
    return CNetworkMgmt::ouGetNWManagementObj().GoOnline(FALSE);
}

/* Configure J1939 timeouts. */
USAGEMODE HRESULT DILJ_ConfigureTimeOut(ETYPE_TIMEOUT eTimeOutType, UINT unMiliSeconds)
{
    HRESULT hResult = S_OK;
    CNetworkMgmt::ouGetNWManagementObj().vConfigureTimeOut(eTimeOutType, unMiliSeconds);
    return hResult;
}

/* Get J1939 timeouts.*/
USAGEMODE HRESULT DILJ_GetTimeOut(ETYPE_TIMEOUT eTimeOutType, UINT& unMiliSeconds)
{
    HRESULT hResult = S_OK;
    CNetworkMgmt::ouGetNWManagementObj().vGetTimeOut(eTimeOutType, unMiliSeconds);
    return hResult;
}

/* Get time mode mapping */
USAGEMODE HRESULT DILJ_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& unAbsTime)
{
    HRESULT hResult = S_OK;
    CNetworkMgmt::ouGetNWManagementObj().GetTimeModeMapping(CurrSysTime, unAbsTime);
    return hResult;
}

/* Get J1939 network status */
USAGEMODE BOOL DILJ_bIsOnline(void)
{
    return CNetworkMgmt::ouGetNWManagementObj().bIsOnline();
}
/* Set call back function pointer*/
USAGEMODE HRESULT DILJ_SetCallBckFuncPtr(DWORD dwClient, ETYPE_CLBCK_FN eClBckFnType, 
                                         void* pvClBckFn)
{
    HRESULT hResult = S_FALSE; 
    CNodeConManager* pManager = NULL;
    pManager = CNetworkMgmt::ouGetNWManagementObj().pouGetConMagrObj(dwClient);
    if (NULL != pManager)
    {
        hResult = pManager->SetCallBackFuncPtr(eClBckFnType, pvClBckFn);
    }
    return hResult;
}