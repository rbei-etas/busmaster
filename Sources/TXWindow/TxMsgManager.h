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
 * \file      TxMsgManager.h
 * \brief     Interface file for CTxMsgManager class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CTxMsgManager class
 */

#pragma once

#include "TxFlags.h"
#include "Utility/Utility_Thread.h"

class CTxMsgManager
{
public:
    // To get singleton Instange
    static CTxMsgManager* s_podGetTxMsgManager();
    // To clear memory used by the singleton object
    static BOOL s_bDeleteTxMsgManager();
    // Static Tx Thread Functions
    // Thread proc for Send Selected Message Transmission
    static UINT s_unSendSelectedMsg(LPVOID pParam );
    // Thread proc for Timer Block Transmission
    static UINT s_unSendMsgBlockOnTime(LPVOID pParam );
    // Thread proc for Key Block Transmission
    static UINT s_unSendMsgBlockOnKey(LPVOID pParam );
    // To handle message transmission
    VOID vStartTransmission(UCHAR ucKeyVal);
    // To allocate memory for global Tx Message structure
    BOOL bAllocateMemoryForGlobalTxList();
    // To assign message blocks from configuration module
    VOID vAssignMsgBlockList();
    // To clear global memory
    VOID vDeleteTxBlockMemory();
    //Return the database pointer
    VOID vStopTransmission(UINT unMaxWaitTime);
    void vGetTxWndConfigData(BYTE*& pDesBuffer, int& nBuffSize);
    void vSetTxWndConfigData(BYTE* pSrcBuffer, int nBuffSize);
    BOOL bIsTxWndConfigChanged();
    void vSetTxStopFlag(BOOL bStartStop);
    BOOL bGetTxStopFlag();
    void vSetClientID(DWORD dwClientID);
    void vSetDILInterfacePtr(void* ptrDILIntrf);

    // proc for getting CAN DIL Interface pointer
    static void* pGetDILInterfacePtr();

public:
    static CFlags         s_TxFlags;
    // Standard Constructor and Destructor are protected to avoid
    // multiple instances
    CTxMsgManager();
    virtual ~CTxMsgManager();
    int nGetBlockCount();
public:
    // Members
    // To store singleton instance pointer
    static CTxMsgManager* m_spodInstance;
    static STHREADINFO    s_sUtilThread;
    static CEvent         s_omState;

private:
    // Map to store Monoshot blocks
    CMap<void*, void*, BOOL, BOOL> m_omMonoshotBlocks;
    //Tx Message List Pointer
    PSTXMSG m_psTxMsgBlockList;
    // Flag for breaking the loop of message transmission for message blocks
    BOOL m_bStopMsgBlockTx;
    int nGetSizeOfTxWndConfigData();
};
