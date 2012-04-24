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
 * \file      DILI_J1939.cpp
 * \brief     Interface file for J1939 BUS
 * \author    Pradeep Kadoor, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for J1939 BUS
 */

/* Project includes */
#include "DIL_Interface_stdafx.h"
#include "Include/BaseDefs.h"
#include "Datatypes/MsgBufAll_Datatypes.h"
#include "DIL_J1939/DIL_J1939_Extern.h"
#include "DILI_J1939.h"

/**
 * \brief Constructor
 *
 * Constructor of CDILI_J1939
 */
CDILI_J1939::CDILI_J1939()
{
}

/**
 * \brief Destructor
 *
 * Destructor of CDILI_J1939
 */
CDILI_J1939::~CDILI_J1939()
{
}

/**
 * \brief Init Instance
 *
 * Initializes the J1939 instance.
 */
BOOL CDILI_J1939::InitInstance(void)
{
    return TRUE;
}

/**
 * \brief Exit Instance
 *
 * Finalizes the J1939 instance.
 */
int CDILI_J1939::ExitInstance(void)
{
    return 0;
}

/**
 * \brief     Initialise
 * \req       RS_26_01 - Initialise
 * \param[in] pILog Error logging interface. Can be null.
 *
 * Performs all initialisation / opening operations
 */
HRESULT CDILI_J1939::DILIJ_Initialise(Base_WrapperErrorLogger* pILog, CBaseDIL_CAN* pIDIL_CAN)
{
    if (pIDIL_CAN != NULL)
    {
        return DILJ_Initialise(pILog, pIDIL_CAN);
    }
    else
    {
        return S_FALSE;
    }
}

/**
 * \brief Uninitialise
 * \req RS_26_02 - Uninitialise
 *
 * Performs all uninitialisation / closure operations
 */
HRESULT CDILI_J1939::DILIJ_Uninitialise(void)
{
    return DILJ_Uninitialise();
}

/**
 * \brief Registers / unregisters a client
 *
 * This is necessary to simulate a node
 * and to receive messages. Only registered client's buffer will be updated
 * on receive of a msg in the bus.
 */
HRESULT CDILI_J1939::DILIJ_RegisterClient(BOOL bRegister, TCHAR* pacNodeName,
        UINT64 un64ECUName, BYTE byPrefAdres,
        DWORD& dwClientId)
{
    return DILJ_RegisterClient(bRegister, pacNodeName, un64ECUName,
                               byPrefAdres, dwClientId);
}

/**
 * \brief Manage Message Buffers
 *
 * Manages the target client buffer list. Call this function to open a
 * data channel to receive messages.
 */
HRESULT CDILI_J1939::DILIJ_ManageMsgBuf(BYTE bAction, DWORD dwClientID,
                                        CBaseMsgBufVSE* pBufObj)
{
    return DILJ_ManageMsgBuf(bAction, dwClientID, pBufObj);
}

/**
 * \brief Send J1939 Message
 *
 * Sends a J1939 message
 */
HRESULT CDILI_J1939::DILIJ_SendJ1939Msg (DWORD dwClientId, UINT unChannel, EJ1939_MSG_TYPE eMsgType, UINT32 unPGN,
        BYTE* pbyData, UINT unDLC, BYTE byPriority, BYTE bySrc, BYTE byDestAdres)
{
    return DILJ_SendJ1939Msg (dwClientId, unChannel, eMsgType, unPGN, pbyData,
                              unDLC, byPriority, bySrc, byDestAdres);
}

/**
 * \brief Get Node Name
 *
 * Gets the node name from 8 bit address from J1939 network.
 */
HRESULT CDILI_J1939::DILIJ_NM_GetNodeName(BYTE byAddress, TCHAR* acNodeName)
{
    return DILJ_NM_GetNodeName(byAddress, acNodeName);
}

/**
 * \brief Get Byte Address
 *
 * Gets the byte address of the client.
 */
HRESULT CDILI_J1939::DILIJ_NM_GetByteAddres(BYTE& byAddress, DWORD dwClient)
{
    return DILJ_NM_GetByteAddres(byAddress, dwClient);
}

/**
 * \brief Requests address
 *
 * Requests address from the node.
 */
HRESULT CDILI_J1939::DILIJ_NM_RequestAddress(DWORD dwClient, UINT unChannel, BYTE byPriority,
        BYTE bySrc, BYTE byDestAddress)
{
    return DILJ_NM_RequestAddress(dwClient, unChannel, byPriority, bySrc, byDestAddress);
}

/**
 * \brief Checks if address is claimed
 *
 * Returns whether the address is already claimed by another node.
 */
BOOL CDILI_J1939::DILIJ_NM_bIsAddressClaimed(BYTE byAddress)
{
    return DILJ_NM_bIsAddressClaimed(byAddress);
}

/**
 * \brief Claims an address
 *
 * Tries to claim 8 bit address to a corresponding node.
 */
HRESULT CDILI_J1939::DILIJ_NM_ClaimAddress(DWORD dwClientId, UINT unChannel, BYTE byAddress, BYTE byPriority)
{
    return DILJ_NM_ClaimAddress(dwClientId, unChannel, byAddress, byPriority);
}

/**
 * \brief Command an address
 *
 * Commands a node with perticular NAME to assume a address.
 */
HRESULT CDILI_J1939::DILIJ_NM_CommandAddress(DWORD dwClient, UINT unChannel, UINT64 unECU_NAME,
        BYTE byNewAddress, BYTE byPriority, BYTE bySrc, BYTE byDestAddress)
{
    return DILJ_NM_CommandAddress(dwClient, unChannel, unECU_NAME, byNewAddress,
                                  byPriority, bySrc, byDestAddress);
}

/**
 * \brief Request PGN
 *
 * Requests a PGN from the node.
 */
HRESULT CDILI_J1939::DILIJ_RequestPGN(DWORD dwClient, UINT unChannel, UINT32 unPGN,
                                      BYTE byPriority, BYTE bySrc, BYTE byDestAddress)
{
    return DILJ_RequestPGN(dwClient, unChannel, unPGN, byPriority, bySrc, byDestAddress);
}

/**
 * \brief Send Acknowledgement Message
 *
 * Sends Positive/Negative acknowledgement msg.
 */
HRESULT CDILI_J1939::DILIJ_SendAckMsg(DWORD dwClient, UINT unChannel, ETYPE_ACK eAckType, UINT32 unPGN, BYTE bySrc, BYTE byAddresAck)
{
    return DILJ_SendAckMsg(dwClient, unChannel, eAckType, unPGN, bySrc, byAddresAck);
}

/**
 * \brief Go Online
 *
 * Starts J1939 network. All nodes start sending according to the configuration.
 */
HRESULT CDILI_J1939::DILIJ_GoOnline()
{
    return DILJ_GoOnline();
}

/**
 * \brief Go Offline
 *
 * Stops J1939 network. All nodes stop sending msgs.
 */
HRESULT CDILI_J1939::DILIJ_GoOffline()
{
    return DILJ_GoOffline();
}

/**
 * \brief Get Timeout
 *
 * Get J1939 timeouts.
 */
HRESULT CDILI_J1939::DILIJ_GetTimeOut(ETYPE_TIMEOUT eTimeOutType, UINT& unMiliSeconds)
{
    return DILJ_GetTimeOut(eTimeOutType, unMiliSeconds);
}

/**
 * \brief Configure Time Out
 *
 * Configure J1939 timeouts.
 */
HRESULT CDILI_J1939::DILIJ_ConfigureTimeOut(ETYPE_TIMEOUT eTimeOutType, UINT unMiliSeconds)
{
    return DILJ_ConfigureTimeOut(eTimeOutType, unMiliSeconds);
}

/**
 * \brief Get Time Mode Mapping
 *
 * Get time mode mapping
 */
HRESULT CDILI_J1939::DILIJ_GetTimeModeMapping(SYSTEMTIME& CurrSysTime,
        UINT64& unAbsTime)
{
    return DILJ_GetTimeModeMapping(CurrSysTime, unAbsTime);
}

/**
 * \brief Check if online
 *
 * Returns J1939 network status.
 */
BOOL CDILI_J1939::DILIJ_bIsOnline(void)
{
    return DILJ_bIsOnline();
}

/**
 * \brief Set Callback Function
 *
 * Set call back function pointer.
 */
HRESULT CDILI_J1939::DILIJ_SetCallBckFuncPtr(DWORD dwClientId, ETYPE_CLBCK_FN eClBckFnType,
        void* pvClBckFn)
{
    return DILJ_SetCallBckFuncPtr(dwClientId, eClBckFnType, pvClBckFn);
}
