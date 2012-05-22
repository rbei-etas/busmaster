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
 * \file      DIL_J1939/DIL_J1939.cpp
 * \brief     Defines the initialization routines for the DLL.
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CAN BUS
 */
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

BEGIN_MESSAGE_MAP(CJ1939TranslatorApp, CWinApp)
END_MESSAGE_MAP()

CJ1939TranslatorApp::CJ1939TranslatorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CJ1939TranslatorApp theApp;

BOOL CJ1939TranslatorApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

/**
 * \brief Initializes J1939 network
 * \req RSI_26_001 - DILJ1939_Initialise
 * \param[in] pILog pointer to wrapper_error object.
 * \param[in] pouIDIL_CAN Interface to DIL CAN.
 * \return S_OK for success, S_FALSE for failure.
 *
 * Initializes J1939 network
 */
USAGEMODE HRESULT DILJ_Initialise(Base_WrapperErrorLogger* pILog, CBaseDIL_CAN* pouIDIL_CAN)
{
    HRESULT hResult = S_OK;
    CNetworkMgmt::ouGetNWManagementObj().vSetLogAndICANPtr(pILog, pouIDIL_CAN);
    CNetworkMgmt::ouGetNWManagementObj().vDoInit();
    return hResult;
}

/**
 * \brief Uninitializes J1939 network
 * \req RSI_26_002 - DILJ1939_Uninitialise
 * \return S_OK for success, S_FALSE for failure.
 *
 * Performs all uninitialisation / closure operations
 */
USAGEMODE HRESULT DILJ_Uninitialise(void)
{
    HRESULT hResult = S_OK;
    CNetworkMgmt::ouGetNWManagementObj().vRemoveAllNodes();
    CNetworkMgmt::ouGetNWManagementObj().vDoExit();
    return hResult;
}

/**
 * \brief Registers / unregisters a client.
 * \req RSI_26_003 - DILJ1939_RegisterClient
 * \param[in] bRegister TRUE to register, else FALSE.
 * \param[in] pacNodeName Client node name.
 * \param[in] un64ECUName 64 bit ECU name.
 * \param[out] dwClientId Client's Id rendered.
 * \return 1. ERR_CLIENT_EXISTS, 2. ERR_NO_CLIENT_EXIST, 3. ERR_NO_MORE_CLIENT_ALLOWED & 4. S_OK
 * \note Explanation: 1. Client already registered, 2. No such client with this id exists. 3. No more clients is allowed to register. 4. Success.
 *
 * Registers / unregisters a client. This is necessary to simulate a node
 * and to receive messages. Only registered client's buffer will be updated
 * on receive of a msg in the bus.
 */
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

/**
 * \brief Manages the target client buffer list. Call this function to open a data channel to receive messages.
 * \req RSI_26_004 - DILJ1939_ManageMsgBuf
 * \param[in] bAction When MSGBUF_ADD, adds pBufObj to the target message buffer list. Removes when MSGBUF_CLEAR.
 * \param[in] ClientID Client ID
 * \param[in] pBufObj Interface to message buffer object.
 * \return S_OK if successful, else S_FALSE.
 * \note At present maximum number of entries in the list is kept as 8.
 *
 * Manages the target client buffer list. Call this function to open a data channel to receive messages.
 */
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

/**
 * \brief Get version information.
 * \return S_OK if successful, else S_FALSE.
 *
 * To get the version information.
 */
USAGEMODE void DILJ_GetVersionInfo (VERSIONINFO& /*sVerInfo*/)
{
}

/**
 * \brief Sends a J1939 message.
 * \req RSI_26_005 - DILJ_SendJ1939Msg
 * \param[in] dwClient Client Id
 * \param[in] unChannel Channel number
 * \param[in] eMsgType COMMAND, BROADCAST, REQUEST, RESPONSE
 * \param[in] unPGN Parameter group number
 * \param[in] pbyData Data bytes
 * \param[in] unDLC Data length in number of bytes.
 * \param[in] byPriority Priority (0-7) byDesrAdress = Destination address.
 * \return S_OK if successful, else S_FALSE.
 *
 * Sends a J1939 message.
 */
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

/**
 * \brief Get node name.
 * \req RSI_26_010 - DILJ_NM_GetNodeName
 * \param[in] byAddress 8 bit node address (0 - 253)
 * \param[out] acNodeName Nodes name.
 * \return S_OK if successful, else S_FALSE.
 *
 * Gets the node name from 8 bit address from J1939 network.
 */
USAGEMODE HRESULT DILJ_NM_GetNodeName(BYTE byAddress, TCHAR* acNodeName)
{
    CNetworkMgmt::ouGetNWManagementObj().vGetNodeName(byAddress, acNodeName);
    return S_OK;
}

/**
 * \brief Get node address.
 * \req RSI_26_011 - DILJ_NM_GetNodeAddress
 * \param[out] byAddress Nodes 8 bit address
 * \param[in] dwClient Client Id.
 * \return S_OK if successful, else S_FALSE.
 *
 * Gets the node address from Client Id from J1939 network.
 */
USAGEMODE HRESULT DILJ_NM_GetByteAddres(BYTE& byAddress, DWORD dwClient)
{
    byAddress = CNetworkMgmt::ouGetNWManagementObj().byGetNodeAddress(dwClient);
    return S_OK;
}

/**
 * \brief Requests address from the node.
 * \req RSI_26_014 - DILJ_NM_RequestAddress
 * \param[in] dwClientId Already register node's client Id
 * \param[in] unChannel Channel number
 * \param[in] byDestAddress Destination Address
 * \param[in] byPriority Priority (0 - 7).
 * \return S_OK if successful, else S_FALSE.
 *
 * A node requests address from an another node.
 */
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

/**
 * \brief Check if address claimed
 * \req RSI_26_012 - DILJ_NM_bIsAddressClaimed
 * \param[in] byAddress 8 bit node address (0 - 253).
 * \return TRUE if claimed, else FALSE.
 *
 * Returns whether the address is already claimed by another node.
 */
USAGEMODE BOOL DILJ_NM_bIsAddressClaimed(BYTE byAddress)
{
    return CNetworkMgmt::ouGetNWManagementObj().bIsAddressClaimed(byAddress);
}

/**
 * \brief Claim address
 * \req RSI_26_013 - DILJ_NM_ClaimAddress
 * \param[in] dwClientId Already register node's client Id
 * \param[in] unChannel Channel number
 * \param[in] byAddress New address to be claimed
 * \param[in] byPriority Priority (0 - 7)
 * \return S_OK if successful, else S_FALSE.
 *
 * Node tries to claim a new address by sending Address Claim message into the network.
 */
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

/**
 * \brief Commands a node with perticular NAME to assume a address.
 * \req RSI_26_015 - DILJ_NM_CommandAddress
 * \param[in] dwClientId Already register node's client Id
 * \param[in] unChannel Channel number
 * \param[in] unECU_NAME 64 bit ECU NAME of the destination node
 * \param[in] byDestAddress Destination Address
 * \param[in] byPriority Priority (0 - 7)
 * \return S_OK if successful, else S_FALSE.
 *
 * A node commands another node to assume an address.
 */
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

/**
 * \brief Requests a PGN from a node
 * \req RSI_26_007 - DILJ_RequestPGN
 * \param[in] dwClient Client Id
 * \param[in] unChannel Channel number
 * \param[in] unPGN Parameter group number to be requested
 * \param[in] byPriority Priority (0-7)
 * \param[in] byDesrAdress Destination address.
 * \return S_OK if successful, else S_FALSE.
 *
 * Requests a PGN from the node.
 */
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

/**
 * \brief Sends a acknowledgement message.
 * \req RSI_26_006 - DILJ_SendAckMsg
 * \param[in] dwClientId Already register node's client Id
 * \param[in] unChannel Channel number
 * \param[in] eAckType Acknowledge type (ACK_POS, ACK_NEG)
 * \param[in] unPGN PGN to be sent.
 * \param[in] pbyData PGN data.
 * \param[in] byAddresAck Destination address.
 * \return S_OK if successful, else S_FALSE.
 *
 * Sends Positive/Negative acknowledgement msg.
 */
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

/**
 * \brief Starts J1939 network.
 * \req RSI_26_008 - DILJ1939_GoOnline
 * \return S_OK if successful, else S_FALSE.
 *
 * Starts J1939 network. All nodes start sending according to the configuration.
 */
USAGEMODE HRESULT DILJ_GoOnline()
{
    return CNetworkMgmt::ouGetNWManagementObj().GoOnline(TRUE);
}

/**
 * \brief Stops J1939 network.
 * \req RSI_26_009 - DILJ1939_GoOffline
 * \return S_OK if successful, else S_FALSE.
 *
 * Stops J1939 network. All nodes stop sending msgs.
 */
USAGEMODE HRESULT DILJ_GoOffline()
{
    //HRESULT hResult = S_OK;
    return CNetworkMgmt::ouGetNWManagementObj().GoOnline(FALSE);
}

/**
 * \brief Configure J1939 timeouts.
 * \req RSI_26_016 - DILJ_ConfigureTimeOut
 * \param[in] eTimeOutType Time out type (TO_BROADCAST, TO_RESPONSE, TO_HOLDING, TO_T1, TO_T2, TO_T3, TO_T4).
 * \param[in] unMiliSeconds Timeout value in mili seconds.
 * \return S_OK if successful, else S_FALSE.
 *
 * Configure timeouts for flow control packets.
 */
USAGEMODE HRESULT DILJ_ConfigureTimeOut(ETYPE_TIMEOUT eTimeOutType, UINT unMiliSeconds)
{
    HRESULT hResult = S_OK;
    CNetworkMgmt::ouGetNWManagementObj().vConfigureTimeOut(eTimeOutType, unMiliSeconds);
    return hResult;
}

/**
 * \brief Get J1939 timeouts
 * \return S_OK if successful, else S_FALSE.
 *
 * Get J1939 timeouts.
 */
USAGEMODE HRESULT DILJ_GetTimeOut(ETYPE_TIMEOUT eTimeOutType, UINT& unMiliSeconds)
{
    HRESULT hResult = S_OK;
    CNetworkMgmt::ouGetNWManagementObj().vGetTimeOut(eTimeOutType, unMiliSeconds);
    return hResult;
}

/**
 * \brief Get time mode mapping
 * \req RSI_26_017 - DILJ_GetTimeModeMapping
 * \param[out] CurrSysTime Reference system time
 * \param[out] unAbsTime Absolute time.
 * \return S_OK if successful, else S_FALSE.
 *
 * Get time mode mapping.
 */
USAGEMODE HRESULT DILJ_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& unAbsTime)
{
    HRESULT hResult = S_OK;
    CNetworkMgmt::ouGetNWManagementObj().GetTimeModeMapping(CurrSysTime, unAbsTime);
    return hResult;
}

/**
 * \brief Get J1939 network status
 * \req RSI_26_018 - DILJ_bIsOnline
 * \return TRUE if Online, else FALSE.
 *
 * Get the J1939 network status.
 */
USAGEMODE BOOL DILJ_bIsOnline(void)
{
    return CNetworkMgmt::ouGetNWManagementObj().bIsOnline();
}

/**
 * Set call back function pointer
 */
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