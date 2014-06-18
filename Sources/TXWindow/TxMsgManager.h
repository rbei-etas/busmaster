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
 * @brief Interface file for CTxMsgManager class
 * \author Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CTxMsgManager class
 */

#pragma once

#include "TxFlags.h"
#include "Utility/Utility_Thread.h"
#include "Utility/XMLUtils.h"

class CTxMsgManager
{
public:
    CTxMsgManager();
    virtual ~CTxMsgManager();

    /**
     * @brief To get singleton Instange
     *
     * This function will return the pointer to the singleton object
     * of CTxMsgManager. In case of memory allocation failure this
     * function will return nullptr
     *
     * @return Pointer to the singleton object
     */
    static CTxMsgManager* s_podGetTxMsgManager();

    /**
     * @brief To clear memory used by the singleton object
     *
     * This function will delete singleton object. If it is null
     * then this function will return FALSE to indicate invalid
     * operation.
     *
     * @return Delete result. FALSE if the object is not valid
     */
    static BOOL s_bDeleteTxMsgManager();

    /**
     * @brief Tx Thread proc for Send Selected Message Transmission
     *
     * This is a thread control function to process
     * transmission of selected message from the dialog.
     *
     * @param[in] pParam Typecasted address of PSTXMSG pointer
     * @return Zero
     */
    static UINT s_unSendSelectedMsg(LPVOID pParam);

    /**
     * @brief Tx Thread proc for Timer Block Transmission
     *
     * This is a thread control function to process
     * message block transmission on time trigger.
     *
     * @param[in] pParam Typecasted address of PSTXMSG pointer
     * @return Zero
     */
    static UINT s_unSendMsgBlockOnTime(LPVOID pParam);

    /**
     * @brief Tx Thread proc for Key Block Transmission
     *
     * This is a thread control function to process
     * message block transmission on key trigger.
     *
     * @param[in] pParam Typecasted address of PSTXMSG pointer
     * @return Zero
     */
    static UINT s_unSendMsgBlockOnKey(LPVOID pParam);

    /**
     * @brief To handle message transmission
     * This function will start transmission on press of a key
     * and on time. This will be called from CMainFrame
     *
     * @param[in] ucKeyVal key pressed
     */
    void vStartTransmission(UCHAR ucKeyVal);

    /**
     * @brief To allocate memory for global Tx Message structure
     *
     * This function allocate memory to global list. Memory
     * will be allocated only if the list is not already
     * having the correct size.Only extra element will be
     * added in case the count is less then m_unMsgBlockCount
     */
    BOOL bAllocateMemoryForGlobalTxList();

    /**
     * @brief To assign message blocks from configuration module
     *
     * This function will assign the message list pointers to
     * global list. The pointers will be obtained from
     * configuration block.
     */
    void vAssignMsgBlockList();

    /**
     * @brief To clear global memory
     *
     * This function will delete the memory allocated for
     * global list.
     */
    void vDeleteTxBlockMemory();

    /** Return the database pointer */
    void vStopTransmission(UINT unMaxWaitTime);

    void vGetTxWndConfigData(BYTE*& pDesBuffer, int& nBuffSize);
    void vGetTxWndConfigData(xmlNodePtr pxmlNodePtr);
    void vSetTxWndConfigData(BYTE* pSrcBuffer, int nBuffSize);
    void vSetTxWndConfigData(xmlDocPtr pDoc);
    void vSetTxStopFlag(BOOL bStartStop);
    BOOL bGetTxStopFlag();

    /**
     * Assigns g_dwClientID to client ID supplied.
     */
    void vSetClientID(DWORD dwClientID);

    /**
     * Assigns g_pouDIL_CAN_Interface variable to DIL interface pointer
     */
    void vSetDILInterfacePtr(void* ptrDILIntrf);

    /**
     * @brief proc for getting CAN DIL Interface pointer
     *
     * Returns DIL interface pointer (g_pouDIL_CAN_Interface)
     */
    static void* pGetDILInterfacePtr();

    static CFlags s_TxFlags;
    static bool s_bDelayBetweenBlocksOnly;
    static UINT s_unTimeDelayBtnMsgBlocks;
    
    /** used in delay btwn msg */
    static PSCOMPLETEMSGINFO m_psCompleteMsgInfo;

    static CRITICAL_SECTION m_csUpdationLock;

    int nGetBlockCount();

    /** To store singleton instance pointer */
    static CTxMsgManager* m_spodInstance;
    static STHREADINFO s_sUtilThread;
    static CEvent s_omState;

private:
    /** Map to store Monoshot blocks */
    CMap<void*, void*, BOOL, BOOL> m_omMonoshotBlocks;

    /** Tx Message List Pointer */
    PSTXMSG m_psTxMsgBlockList;

    /** Flag for breaking the loop of message transmission for message blocks */
    BOOL m_bStopMsgBlockTx;

    int nGetSizeOfTxWndConfigData();
};
