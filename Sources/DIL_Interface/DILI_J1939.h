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
 * \file      DILI_J1939.h
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * DIL for J1939
 */

#pragma once

/* Project includes */
#include "BaseDIL_J1939.h"

class CDILI_J1939 : public CBaseDILI_J1939
{
public:
    CDILI_J1939();
    ~CDILI_J1939();

    BOOL InitInstance(void);
    int ExitInstance(void);
    /*Initializes J1939 network
    Parameters: 
    1.pouSAE_J1939 - Pointer to the object which contains list of all SAE defined PGNS and SPNs. 
    2.pILog - pointer to wrapper_error object. 
    Return value:
    1.S_OK for success or
    2.S_FALSE for failure.
    */
    HRESULT DILIJ_Initialise(Base_WrapperErrorLogger* pILog, CBaseDIL_CAN* pouIDIL_CAN);

    /*Uninitializes J1939 network 
    Return value:
    1.S_OK for success or
    2.S_FALSE for failure.
    */
    HRESULT DILIJ_Uninitialise ( void);

    /*Registers / unregisters a client. This is necessary to simulate a node
    and to receive messages. Only registered client's buffer will be updated
    on receive of a msg in the bus.

    Parameters: 
    1. bRegister - TRUE to register, else FALSE.
    2. pacNodeName - Client node name. 
    3. byAddress - 8 bit node address (0 - 253).
    4. un64ECUName - 64 bit ECU name.
    5. dwClientId - [Out Parm] Client's Id rendered.

    Return value:
    1. ERR_CLIENT_ EXISTS or
    2. ERR_ NO_CLIENT_EXIST or  
    3. ERR_NO_ MORE_CLIENT_ALLOWED  or
    4. S_OK.
    */
    HRESULT DILIJ_RegisterClient(BOOL bRegister, TCHAR* pacNodeName, 
                                    UINT64 un64ECUName, BYTE byPrefAdres, DWORD& dwClientId);

    /* Manages the target client buffer list. Call this function to open a 
    data channel to receive messages.
    Parameters: 
    1. bAction - When MSGBUF_ADD, adds pBufObj to the target message buffer list. 
                Removes when MSGBUF_CLEAR.
    2. ClientID - Client ID
    3. pBufObj - Interface to message buffer object.


    Return value:
    1.S_OK for success or
    2.S_FALSE for failure.
    */
    HRESULT DILIJ_ManageMsgBuf(BYTE bAction, DWORD ClientID, 
                                            CBaseMsgBufVSE* pBufObj);

    /*To get the version information.*/
    void DILIJ_GetVersionInfo (VERSIONINFO& sVerInfo);

    /* Sends a J1939 message.
    Parameters:
    1. dwClient - Id of registered client
    2. unChannel - Channel number
    3. eMsgType - COMMAND, BROADCAST, DATA, REQUEST
    4. unPGN - PGN to be sent or received
    5. pbyData - Data
    6. unDLC   - Data length in number of bytes
    7. byPriority - Priority of the message (0-7)
    8. bySrc - From address, This is ignored if client is not a monitor client.
    9. byDest - Destination address.

    Return value:
    1.S_OK for success or
    2.S_FALSE for failure.
    */
    HRESULT DILIJ_SendJ1939Msg (DWORD dwClient, UINT unChannel,
                                        EJ1939_MSG_TYPE eMsgType, UINT32 unPGN,
                                        BYTE* pbyData, UINT unDLC,                                     
                                        BYTE byPriority = DEFAULT_PRIORITY,
                                        BYTE bySrc = ADDRESS_NULL,
                                        BYTE byDestAdress = ADDRESS_ALL);

    /* Sends Positive/Negative acknowledgement msg.
    Parameters: 
    1. dwClient - Client Id of the sender node
    2. eAckType - ACK_POS, ACK_NEG
    3. unPGN - PGN to be acknowledged.
    4. bySrc - From address, This is ignored if client is not a monitor node.
    5. byAddresAck - Address to be acknowledged.

    Return value:
    1.S_OK for success or
    2.S_FALSE for failure.
    */
    HRESULT DILIJ_SendAckMsg(DWORD dwClient, UINT unChannel,
                                    ETYPE_ACK eAckType, UINT32 unPGN, 
                                    BYTE bySrc,
                                    BYTE byAddresAck);

    /* Requests a PGN from the node.
    Parameters: 
    1. dwClient - Client Id of the sender node
    2. unPGN - PGN to be requested.
    3. bySrc - From address, This is ignored if the client is not monitor node.
    4. byDestAddress - From Node address.

    Return value:
    1.S_OK for success or
    2.S_FALSE for failure.
    */
    HRESULT DILIJ_RequestPGN(DWORD dwClient, 
                                    UINT unChannel, UINT32 unPGN,
                                    BYTE byPriority = DEFAULT_PRIORITY,
                                    BYTE bySrc = ADDRESS_NULL,
                                    BYTE byDestAddress = ADDRESS_ALL);

    /* Starts J1939 network. All nodes start sending according to the configuration.
    Return value:
    1.S_OK for success or
    2.S_FALSE for failure.
    */
    HRESULT DILIJ_GoOnline();

    /* Stops J1939 network. All nodes stop sending msgs.
    Return value:
    1.S_OK for success or
    2.S_FALSE for failure.
    */
    HRESULT DILIJ_GoOffline();

    /*Gets the node name from 8 bit address from J1939 network.
    Parameters: 
    1. byAddress - 8 bit node address (0 - 253).
    2. acNodeName - [OUT_PARAM] Nodes name.

    Return value:
    1.S_OK for success or
    2.S_FALSE for failure.
    */
    HRESULT DILIJ_NM_GetNodeName(BYTE byAddress, TCHAR* acNodeName);

    /*Gets the 8 bit address from node name from J1939 network.
    Parameters: 
    1. byAddress - [OUT_PARAM]8 bit node address (0 - 253).
    2. dwClient -  Client Id

    Return value:
    1.S_OK for success or
    2.S_FALSE for failure.
    */
    HRESULT DILIJ_NM_GetByteAddres(BYTE& byAddress, DWORD dwClient);

    /*Returns whether the address is already claimed by another node.
    Return value:
    1.TRUE for already claimed or
    2.FALSE for not claimed.
    */
    BOOL DILIJ_NM_bIsAddressClaimed(BYTE byAddress);

    /*Tries to claim 8 bit address to a corresponding node.
    Parameters: 
    1. dwClientId - Already register node's client Id
    2. byAddress - New address (0 - 253).
    3. byPriority - Priority

    Return value:
    1.S_OK for success or
    2.S_FALSE for failure.
    */
    HRESULT DILIJ_NM_ClaimAddress(DWORD dwClientId, UINT unChannel, BYTE byAddress, BYTE byPriority = DEFAULT_PRIORITY);

    /* Requests address from the node.
    Parameters: 
    1. dwClientId - Senders's client Id
    2. byPriority - Priority
    3. bySrc = Src Address for monitor node. 
                This is ignored if client is not a monitor node.
    4. byDestAddress - Destination address (global by default).

    Return value:
    1.S_OK for success or
    2.S_FALSE for failure.
    */
    HRESULT DILIJ_NM_RequestAddress(DWORD dwClient, UINT unChannel,
                                            BYTE byPriority = DEFAULT_PRIORITY,
                                            BYTE bySrc = ADDRESS_NULL,
                                            BYTE byDestAddress = ADDRESS_ALL);

    /*Commands a node to assume a address.
    Parameters: 
    1. dwClientId - Senders's client Id
    2. unECU_NAME - 64 bit NAME of the ECU to which command has to be sent.
    3. byNewAddress - The new address it has to assume.
    4. byPriority - Priority
    5. bySrc - This is ignored if client is not monitor node.
    6. byDestAddress - Destination address (global by default).

    Return value:
    1.S_OK for success or
    2.S_FALSE for failure.
    */
    HRESULT DILIJ_NM_CommandAddress(DWORD dwClient, UINT unChannel,
                                            UINT64 unECU_NAME, 
                                            BYTE byNewAddress, 
                                            BYTE byPriority = DEFAULT_PRIORITY,
                                            BYTE bySrc = ADDRESS_NULL,
                                            BYTE byDestAddress = ADDRESS_ALL);
    
    /*Get J1939 timeouts.
    Parameters: 
    1. eTimeOutType - Type of the timeout 
                    1. TYPE_TO_BROADCAST  
                    2. TYPE_TO_RESPONSE
                    3. TYPE_TO_HOLDING
                    4. TYPE_TO_T1
                    5. TYPE_TO_T2
                    6. TYPE_TO_T3
                    7. TYPE_TO_T4
    2. unMiliSeconds - [OUT PARAM]Value of the timeout

    Return value:
    1.S_OK for success or
    2.S_FALSE for failure.
    */
    HRESULT DILIJ_GetTimeOut(ETYPE_TIMEOUT eTimeOutType, UINT& unMiliSeconds);

    /*Configure J1939 timeouts.
    Parameters: 
    1. eTimeOutType - Type of the timeout 
                    1. TYPE_TO_BROADCAST  
                    2. TYPE_TO_RESPONSE
                    3. TYPE_TO_HOLDING
                    4. TYPE_TO_T1
                    5. TYPE_TO_T2
                    6. TYPE_TO_T3
                    7. TYPE_TO_T4
    2. unMiliSeconds - Value of the timeout

    Return value:
    1.S_OK for success or
    2.S_FALSE for failure.
    */
    HRESULT DILIJ_ConfigureTimeOut(ETYPE_TIMEOUT eTimeOutType, UINT unMiliSeconds);

    /*Gets time mode mapping
    Parameters: 
    CurrSysTime - System time
    unAbsTime   - Absolute time stamp after controller started

    Return value:
    1.S_OK for success or
    2.S_FALSE for failure.
    */
    HRESULT DILIJ_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& unAbsTime);

    /*Returns J1939 network status.

    Return value:
    1.TRUE if online
    2.FALSE if offline
    */
    BOOL DILIJ_bIsOnline(void);

    /*Set call back function pointer
    Parameters: 
    eClBckFnType - Call back function type. 
                (Long data & broadcast data indication & type).
    pvClBckFn   - Call back function pointer.

    Return value:
    1.S_OK for success or
    2.S_FALSE for failure.
    */
    HRESULT DILIJ_SetCallBckFuncPtr(DWORD dwClientId, ETYPE_CLBCK_FN eClBckFnType, 
                                            void* pvClBckFn);

};
