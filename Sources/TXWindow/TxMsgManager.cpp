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
 * @brief Implementation file for CTxMsgManager class
 * @author Raja N
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CTxMsgManager class
 */

#include "TxWindow_stdafx.h"             // For standard include
#include "TxMsgManager.h"       // For CTxMsgManager class declaration
// For DIL Interface Class
#include "Include/BaseDefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_Datatypes.h"
#include "DIL_Interface/BaseDIL_CAN.h"
#include "include/CAN_Error_Defs.h"
#include "include/XMLDefines.h"
#include "Utility/XMLUtils.h"
#include "Utility/MultiLanguageSupport.h"

/** CLient Id from the DIL */
static DWORD g_dwClientID = 0;

/** DIL Interface */
static CBaseDIL_CAN* g_pouDIL_CAN_Interface = nullptr;

/**
 * Flag for breaking the loop of message transmission for
 * selected message blocks
 */
BOOL g_bStopSelectedMsgTx = TRUE;

// Init Static member
CTxMsgManager* CTxMsgManager::m_spodInstance = nullptr;
STHREADINFO CTxMsgManager::s_sUtilThread;
CEvent CTxMsgManager::s_omState;
CFlags CTxMsgManager::s_TxFlags;

/** stores the current state of delay between blocks check box */
bool CTxMsgManager::s_bDelayBetweenBlocksOnly;

/** stores the delay between blocks interval value */
UINT CTxMsgManager::s_unTimeDelayBtnMsgBlocks;

/** critical section used while copying PSTXCANMSGLIST from global to local */
CRITICAL_SECTION CTxMsgManager::m_csUpdationLock;

/** Indicates the stop tranmission is invoked */
CEvent g_omInformStopTx = CEvent(FALSE, TRUE, "StopTxEvent"); 

/** Key Handler proc CMap Hash table size. */
#define defKEY_HANDLER_HASH_TABLE_SIZE 17

CTxMsgManager::CTxMsgManager()
{
    // Set some prime number
    m_omMonoshotBlocks.InitHashTable( defKEY_HANDLER_HASH_TABLE_SIZE );
    //m_pouMsgSignal = nullptr;
    m_psTxMsgBlockList = nullptr;
    m_bStopMsgBlockTx = TRUE;
    s_bDelayBetweenBlocksOnly = false;
    InitializeCriticalSection(&m_csUpdationLock);
    g_omInformStopTx.ResetEvent();
}

CTxMsgManager::~CTxMsgManager()
{
    DeleteCriticalSection(&m_csUpdationLock);
}

void CTxMsgManager::vSetClientID(DWORD dwClientID)
{
    g_dwClientID = dwClientID;
}

void CTxMsgManager::vSetDILInterfacePtr(void* ptrDILIntrf)
{
    g_pouDIL_CAN_Interface = (CBaseDIL_CAN*)ptrDILIntrf;
}

void* CTxMsgManager::pGetDILInterfacePtr()
{
    return (void*)g_pouDIL_CAN_Interface;
}

CTxMsgManager* CTxMsgManager::s_podGetTxMsgManager()
{
    if( m_spodInstance == nullptr )
    {
        m_spodInstance = new CTxMsgManager;
        // Handling nullptr condition is caller's duty
        if( m_spodInstance == nullptr )
        {
            // Help debugging
            ASSERT( FALSE );
        }
    }
    // Return the pointer or nullptr in case of failure
    return m_spodInstance;
}

BOOL CTxMsgManager::s_bDeleteTxMsgManager()
{
    BOOL bDelete = FALSE;
    // Check instance pointer
    if( m_spodInstance != nullptr )
    {
        // Delete memory associated with this pointer
        delete m_spodInstance;
        m_spodInstance = nullptr;
        // Set result to success
        bDelete = TRUE;
    }
    // Return the result
    return bDelete;
}

void CTxMsgManager::vStartTransmission(UCHAR ucKeyVal)
{
    if(m_psTxMsgBlockList != nullptr )
    {
        PSMSGBLOCKLIST psMsgBlock = nullptr;
        PSTXMSG psTxMsg           = nullptr;
        // Get the message block header pointer from configuration module.
        psMsgBlock = CTxWndDataStore::ouGetTxWndDataStoreObj().psReturnMsgBlockPointer();
        // Get the header pointer from global list
        psTxMsg    = m_psTxMsgBlockList;

        // Get the message block count. No memory allocation is required.
        UINT unMsgBlockCount = 0;
        UINT* punCount = &unMsgBlockCount;
        CTxWndDataStore::ouGetTxWndDataStoreObj().bGetTxData(TX_MSG_BLOCK_COUNT, (void**)&punCount);
        UINT unCount = 0;
        // If there is a key value,  this function is called from
        // CMainFrame::PreTranslateMessage to start transmission on press
        // of a key if key value is zero then the function is called from
        // CMainFrame:: OnSendMessage to start the transmission
        // of all message block with time trigger in a separate thread.
        if(ucKeyVal != 0 )
        {
            // find a matching key value in the list of message block pointer
            // in configuration  module. Also loop through the global list
            // to get the same message block having a matching key value.
            while( psMsgBlock != nullptr && psTxMsg != nullptr &&
                    unCount < unMsgBlockCount)
            {
                // Check if the valid pointers are obtained before it reaches
                // the if pointer is not valid then there is not match of the
                // key pressed.Also check if there is a match and if that block
                // is active
                if( psMsgBlock->m_unMsgCount > 0 &&
                        psMsgBlock->m_bActive == TRUE &&
                        IS_KEY_TRIGGERED( psMsgBlock->m_ucTrigger ) &&
                        psMsgBlock->m_ucKeyValue == ucKeyVal)
                {
                    // Check if the user has selected option to transmit
                    // message one frame at a time on each key press from same
                    // message block.If the user has selected to send all
                    // message in block one after another then create a thread
                    // and start transmission without creating key event.
                    if(psMsgBlock->m_bTxAllFrame != TRUE )
                    {
                        // Check the thread handle to check the thread status
                        // If it is already created then signal the thread
                        // Or create a new thread
                        if(psTxMsg->m_sKeyThreadInfo.m_hThread != nullptr)
                        {
                            psTxMsg->m_omKeyEvent.SetEvent();
                        }
                        else
                        {
                            // For monoshot key block transmission should happen
                            // only once. So check the CMap to see the entry for
                            // Tx Block
                            if( m_omMonoshotBlocks [ psTxMsg ] == FALSE )
                            {
                                // Set the properties of the block
                                psTxMsg->m_bSendAllMessages = FALSE;
                                psTxMsg->m_omKeyEvent.ResetEvent();
                                psTxMsg->m_sKeyThreadInfo.m_hThread =
                                    AfxBeginThread(s_unSendMsgBlockOnKey,psTxMsg);
                                // Save the pointer for monoshot key blocks
                                // Update the CMap to indicate that thread
                                // has been Created
                                if( psTxMsg->m_bType == FALSE )
                                {
                                    m_omMonoshotBlocks [ psTxMsg ] = TRUE;
                                }
                            }
                        }
                    }
                    else
                    {
                        // If the thread is terminated and if it is already
                        // created once, don't create it again since it is
                        // key trigger. This is done for same transmission
                        // start command. If user has selected stop and start
                        // then again this transmission will start.
                        if( psTxMsg->m_sKeyThreadInfo.m_hThread == nullptr )
                        {
                            if( m_omMonoshotBlocks [ psTxMsg ] == FALSE )
                            {
                                psTxMsg->m_bSendAllMessages = TRUE;
                                psTxMsg->m_sKeyThreadInfo.m_hThread =
                                    AfxBeginThread(s_unSendMsgBlockOnKey,psTxMsg);
                                if( psTxMsg->m_bType == FALSE )
                                {
                                    m_omMonoshotBlocks [ psTxMsg ] = TRUE;
                                }
                            }
                        }
                    }
                }
                unCount++;
                psMsgBlock = psMsgBlock->m_psNextMsgBlocksList;
                psTxMsg    = psTxMsg->m_psNextTxMsgInfo;
            } // while
        } // if(ucKeyVal != 0 )
        else
        {
            // Clear the CMap of monoshot blocks
            m_omMonoshotBlocks.RemoveAll();
            // check if the message block with time trigger are active
            // if so then only start tranmission in a seperate thread.
            while(psMsgBlock != nullptr && psTxMsg != nullptr &&
                    unCount < unMsgBlockCount)
            {
                // Set the flag to FALSE to indicate that user has started the
                // transmission and it will be set to true for key trigger
                // as key trigger thread should be created only once even thread
                // has existed.
                //  m_bTransmittedOnce = FALSE;
                if( psMsgBlock->m_unMsgCount > 0 &&
                        psMsgBlock->m_bActive == TRUE &&
                        (IS_TIME_TRIGGERED(psMsgBlock->m_ucTrigger)|| s_bDelayBetweenBlocksOnly)&&
                        psTxMsg->m_sTimerThreadInfo.m_hThread == nullptr )
                {
                    psTxMsg->m_sTimerThreadInfo.m_hThread =
                        AfxBeginThread(s_unSendMsgBlockOnTime,psTxMsg);
                }
                psMsgBlock = psMsgBlock->m_psNextMsgBlocksList;
                psTxMsg    = psTxMsg->m_psNextTxMsgInfo;
                unCount++;
            }
        }
    }
}

void CTxMsgManager::vStopTransmission(UINT unMaxWaitTime)
{
    vSetTxStopFlag(TRUE);
    PSTXMSG psTxMsg = m_psTxMsgBlockList;
    // Store wait return value of timer and key threads seprately
    DWORD   dwTimerThreadStatus = WAIT_ABANDONED;
    DWORD   dwKeyThreadStatus = WAIT_ABANDONED;

    if (psTxMsg != nullptr)
    {
        g_omInformStopTx.SetEvent();
    }

    while(psTxMsg != nullptr )
    {
        // Set the event for thread waiting for key event to set.
        // This is only if a block is trigger on key and so
        // the m_pomKeyEvent will not be null.
        if( psTxMsg->m_sKeyThreadInfo.m_hThread != nullptr)
        {
            // Set the Key Event so that it will terminate by iteself
            psTxMsg->m_omKeyEvent.SetEvent();
            Sleep(0);
        }
        // If timer tharead is active then wait for a while
        if(psTxMsg->m_sTimerThreadInfo.m_hThread != nullptr )
        {
            // Wait for thread to exit.
            dwTimerThreadStatus =
                WaitForSingleObject( psTxMsg->m_omTxBlockTimerEvent,
                                     unMaxWaitTime );

        }
        // If key thread is active then wait for key thread termination
        if(psTxMsg->m_sKeyThreadInfo.m_hThread != nullptr )
        {
            // Wait for thread to exit.
            dwKeyThreadStatus =
                WaitForSingleObject( psTxMsg->m_omTxBlockKeyEvent,
                                     unMaxWaitTime );
        }
        // If time out, terminate the thread and delete the memory
        // if it is allocated inside the thread function and not
        // deleted
        if( dwTimerThreadStatus == WAIT_TIMEOUT &&
                psTxMsg->m_sTimerThreadInfo.m_hThread != nullptr)
        {
            TerminateThread(psTxMsg->m_sTimerThreadInfo.m_hThread, 0);
            // Invalidate the handle
            psTxMsg->m_sTimerThreadInfo.m_hThread = nullptr;
            // Delete if any memory is allocated for this thread.
            // Currently there are all global data so not required
            // to be deleted here.
            /***********************************************************/
            // Right now Tx timer thread isn't using any dynamic memory
            // So this is commented
            /*if( psTxMsg->m_sTimerThreadInfo.m_pvThread !=nullptr )
            {
                m_asUtilThread[defTX_SEL_MSG_THREAD].m_pvThread = nullptr;
            }*/
            /***********************************************************/
        }
        // If the key handler is not terminated yet then kill the thread
        if( dwKeyThreadStatus == WAIT_TIMEOUT &&
                psTxMsg->m_sKeyThreadInfo.m_hThread != nullptr)
        {
            TerminateThread(psTxMsg->m_sKeyThreadInfo.m_hThread, 0);
            // Invalidate the handle
            psTxMsg->m_sKeyThreadInfo.m_hThread = nullptr;
            /***********************************************************/
            // Delete if any memory is allocated for this thread.
            // Currently there are all global data so not required
            // to be deleted here.
            /*if( psTxMsg->m_sThreadInfo1.m_pvThread !=nullptr )
            {
                m_asUtilThread[defTX_SEL_MSG_THREAD].m_pvThread = nullptr;
            }*/
            /***********************************************************/
        }
        // Go to the next thread node
        psTxMsg = psTxMsg->m_psNextTxMsgInfo;
    }
    g_omInformStopTx.ResetEvent();
}

BOOL CTxMsgManager::bAllocateMemoryForGlobalTxList()
{
    BOOL bReturn = TRUE;
    PSTXMSG psTxMsg    = m_psTxMsgBlockList;
    PSTXMSG psTxMsgMem = nullptr;
    UINT unCount = 0;
    UINT unMsgBlockCount = 0;
    // Get the message block count. No memory allocation is required.
    UINT* punCount = &unMsgBlockCount;
    CTxWndDataStore::ouGetTxWndDataStoreObj().bGetTxData(TX_MSG_BLOCK_COUNT, (void**)&punCount);
    if(unMsgBlockCount <= 0 )
    {
        bReturn = FALSE;
    }
    if(psTxMsg != nullptr && bReturn != FALSE)
    {
        unCount++;
        // Go to end of the list
        while(psTxMsg->m_psNextTxMsgInfo != nullptr )
        {
            psTxMsg = psTxMsg->m_psNextTxMsgInfo;
            unCount++;
        }

    }
    // if count  is less then total message block
    // allocate extra memory for the extra node in the
    // list

    while(unCount< unMsgBlockCount && bReturn == TRUE)
    {
        psTxMsgMem = new STXMSG;
        if(psTxMsgMem == nullptr )
        {
            bReturn = FALSE;
        }
        else
        {
            //           InitializeCriticalSection(&psTxMsgMem->m_sMsgBlocksCriticalSection);
            // Set the maximum count to two
            psTxMsgMem->m_hSemaphore = CreateSemaphore( nullptr,
                                       defTX_BLOCK_SEM_MAX_COUNT,
                                       defTX_BLOCK_SEM_MAX_COUNT,
                                       "");
            psTxMsgMem->m_psTxCANMsgList         = nullptr;
            psTxMsgMem->m_unTimeInterval         = 0;
            psTxMsgMem->m_sTimerThreadInfo.m_hThread  = nullptr;
            psTxMsgMem->m_sTimerThreadInfo.m_pvThread = nullptr;
            psTxMsgMem->m_sKeyThreadInfo.m_hThread  = nullptr;
            psTxMsgMem->m_sKeyThreadInfo.m_pvThread = nullptr;
            psTxMsgMem->m_unIndex = -1;

            psTxMsgMem->m_psNextTxMsgInfo        = nullptr;
            psTxMsgMem->m_bType                  = FALSE;
            if(psTxMsg != nullptr )
            {
                psTxMsg->m_psNextTxMsgInfo = psTxMsgMem;
                psTxMsg = psTxMsg->m_psNextTxMsgInfo;
            }
            else
            {
                psTxMsg = psTxMsgMem;
                m_psTxMsgBlockList = psTxMsg;
            }
        }
        unCount++;
    }
    return bReturn;
}

void CTxMsgManager::vAssignMsgBlockList()
{
    PSMSGBLOCKLIST psMsgBlock = CTxWndDataStore::ouGetTxWndDataStoreObj().psReturnMsgBlockPointer();
    PSTXMSG psTxMsg           = m_psTxMsgBlockList;
    while(psTxMsg != nullptr && psMsgBlock != nullptr )
    {
        psTxMsg->m_unTimeInterval = psMsgBlock->m_unTimeInterval;
        psTxMsg-> m_unIndex =  psMsgBlock->m_unIndex;
        PSTXCANMSGLIST ppsTxCANMsgList = psTxMsg->m_psTxCANMsgList;
        if(psTxMsg->m_psTxCANMsgList == nullptr)
        {
            psTxMsg->m_psTxCANMsgList = new STXCANMSGLIST;
        }
        if(psMsgBlock->m_unMsgCount > 0 )
        {
            psTxMsg->m_psTxCANMsgList = psMsgBlock->m_psTxCANMsgList;
            psTxMsg->m_omTxBlockAutoUpdateEvent.SetEvent();

            if( IS_KEY_TRIGGERED( psMsgBlock->m_ucTrigger ) )
            {
                psTxMsg->m_omTxBlockAutoUpdateEventForKey.SetEvent();
            }
        }
        else
        {
            psTxMsg->m_psTxCANMsgList = nullptr;
        }
        psTxMsg->m_bType          = psMsgBlock->m_bType;
        psTxMsg    = psTxMsg->m_psNextTxMsgInfo;
        psMsgBlock = psMsgBlock->m_psNextMsgBlocksList;
    }
}

void CTxMsgManager::vDeleteTxBlockMemory()
{
    if(m_psTxMsgBlockList != nullptr )
    {
        s_bDelayBetweenBlocksOnly = false;
        CTxWndDataStore::ouGetTxWndDataStoreObj().m_bDelayBetweenMsgBlocks = false;
        s_unTimeDelayBtnMsgBlocks = 100;

        PSTXMSG psTxMsg = m_psTxMsgBlockList; // Get the head pointer
        PSTXMSG psNextTxMsg = nullptr;
        // loop through till last list is reached and delete it one by one
        // from begining. It is a singly link list.
        while(psTxMsg != nullptr )
        {
            ///            DeleteCriticalSection(&psTxMsg->m_sMsgBlocksCriticalSection);
            if(psTxMsg->m_hSemaphore != nullptr )
            {
                CloseHandle(psTxMsg->m_hSemaphore);
            }
            psNextTxMsg = psTxMsg->m_psNextTxMsgInfo;
            delete psTxMsg;
            psTxMsg = nullptr;
            psTxMsg = psNextTxMsg;
        }
        // initialise the pointer to nullptr
        m_psTxMsgBlockList = nullptr;
    }
}

const int SIZE_STCAN_MSG = sizeof(STCAN_MSG);

UINT CTxMsgManager::s_unSendSelectedMsg(LPVOID pParam )
{
    s_omState.ResetEvent();
    PSTXSELMSGDATA psTxCanMsg = static_cast <PSTXSELMSGDATA> (pParam);
    if(psTxCanMsg != nullptr)
    {
        s_sUtilThread.m_pvThread = (void*)psTxCanMsg;
        UINT unIndex = 0;
        while(unIndex < psTxCanMsg->m_unCount && g_bStopSelectedMsgTx == FALSE)
        {
            if (psTxCanMsg->m_psTxMsg[unIndex].m_unMsgID != -1)
            {
                // Use HIL function to send CAN message
                int nReturn = g_pouDIL_CAN_Interface->DILC_SendMsg(g_dwClientID, psTxCanMsg->m_psTxMsg[unIndex]);
            }
            unIndex++;
        }
        delete [](psTxCanMsg->m_psTxMsg);
        psTxCanMsg->m_psTxMsg = nullptr;
        delete psTxCanMsg;
        psTxCanMsg = nullptr;
    }
    s_sUtilThread.m_pvThread = nullptr;
    g_bStopSelectedMsgTx = TRUE;
    s_omState.SetEvent();
    return 0;
}

UINT CTxMsgManager::s_unSendMsgBlockOnTime(LPVOID pParam )
{
    PSTXMSG psTxMsg  =  static_cast<PSTXMSG> (pParam);
    PSTXCANMSGLIST psIntialTxMsgList = nullptr;

    static  int snThreadCount   = -1;           //its incremented as soon as it enters the if condition
    if (psTxMsg != nullptr)
    {
        snThreadCount++;
        UINT unTimeInterval = 0;
        psTxMsg->m_omTxBlockTimerEvent.ResetEvent();
        PSTXCANMSGLIST psTxMsgList = new STXCANMSGLIST;
        psTxMsg->m_psTxCANMsgList->m_bModified = true;      //set this to override the default;
        CTxWndDataStore::ouGetTxWndDataStoreObj().bCopyMsgList(psTxMsgList, & psTxMsg->m_psTxCANMsgList);
        psIntialTxMsgList = psTxMsgList;
        TIMECAPS time;
        MMRESULT mmResult = TIMERR_NOERROR;
        HANDLE hEventWait = nullptr, hEventWaitForBlockTx = nullptr;
        MMRESULT Result = 0;

        //getting the message count
        UINT unMsgBlockCount = 0;
        UINT* punCount = &unMsgBlockCount;
        CTxWndDataStore::ouGetTxWndDataStoreObj().bGetTxData(TX_MSG_BLOCK_COUNT, (void**)&punCount);
        if (timeGetDevCaps(&time, sizeof(TIMECAPS)) == TIMERR_NOERROR)
        {
            mmResult = timeBeginPeriod(time.wPeriodMin);
        }
        if(s_bDelayBetweenBlocksOnly)
        {
            //delay calculated for the first time,
            //thread count * delay betrween blocks
            unTimeInterval = snThreadCount * s_unTimeDelayBtnMsgBlocks;
            hEventWaitForBlockTx = CreateEvent(nullptr, FALSE, FALSE, nullptr);
            if(unTimeInterval == 0)
            {
                unTimeInterval = 1;
            }
            Result = timeSetEvent(unTimeInterval, time.wPeriodMin,
                                  (LPTIMECALLBACK) hEventWaitForBlockTx, 0,
                                  TIME_CALLBACK_EVENT_SET | TIME_ONESHOT);
            WaitForSingleObject(hEventWaitForBlockTx, INFINITE);
            if (mmResult == TIMERR_NOERROR)
            {
                timeEndPeriod(time.wPeriodMin);
            }
            if(Result != 0)
            {
                Result = timeKillEvent(Result);
                Result = 0;
            }
        }
        //delay  is decided based on the type
        if(s_bDelayBetweenBlocksOnly)
        {
            //constnt delay should be equal to (no. of blocks * delay between blocks)
            unTimeInterval = unMsgBlockCount * s_unTimeDelayBtnMsgBlocks;
        }
        else
        {
            unTimeInterval = psTxMsg->m_unTimeInterval;
        }

        if (timeGetDevCaps(&time, sizeof(TIMECAPS)) == TIMERR_NOERROR)
        {
            mmResult = timeBeginPeriod(time.wPeriodMin);
        }
        // Create the event object to wait for
        hEventWait = CreateEvent(nullptr, FALSE, FALSE, nullptr);

        Result = timeSetEvent(unTimeInterval, time.wPeriodMin,
                              (LPTIMECALLBACK) hEventWait, 0,
                              TIME_CALLBACK_EVENT_SET | TIME_PERIODIC);

        HANDLE hEvents[] = {hEventWait, g_omInformStopTx};
        HANDLE hEventsTimeBlocks[] = {nullptr, g_omInformStopTx};

        LPLONG lpPreviousCount = nullptr;
        BOOL bStopMsgBlockTx = CTxMsgManager::s_podGetTxMsgManager()->bGetTxStopFlag();

        CString             csData;
        BOOL bFileOpen               = FALSE;
        DWORD dwIndex = 0; // Consider timer event as default
        while ((psTxMsgList != nullptr) && (bStopMsgBlockTx == FALSE))
        {
            // Wait for the event
            if(!s_bDelayBetweenBlocksOnly)
            {
                //WaitForSingleObject(hEventWait, INFINITE);
                dwIndex = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);
            }

            //Transmit only if the global flag is set to go

            // Get the first enabled message
            // This block will select next available enabled message
            // If there is no message in the list that is enabled this will
            // return. A check has been introduced before transmission to
            // find this condition
            // Check whether currently available message is selected or not
            DWORD  dwWaitResult;

            dwWaitResult  = WaitForSingleObject(psTxMsg->m_omTxBlockAutoUpdateEvent, 0);
            if( dwWaitResult == WAIT_OBJECT_0 )
            {
                EnterCriticalSection(&m_csUpdationLock);
                CTxWndDataStore::ouGetTxWndDataStoreObj().bCopyMsgList(psIntialTxMsgList, & psTxMsg->m_psTxCANMsgList); //reset the values
                LeaveCriticalSection(&m_csUpdationLock);
                psTxMsg->m_omTxBlockAutoUpdateEvent.ResetEvent ();
            }

            // Exit thread if transmission is stopped
            if (dwIndex == 1)
            {
                break;
            }
            // The following code can be enabled if to stop the thread at runtime
            // when the "Update" button is clicked if the block is empty
            /*// Check if the message list is empty. If empty, exit the thread
            if (nullptr != psTxMsg)
            {
                if (nullptr == psTxMsg->m_psTxCANMsgList)
                {
                    break;
                }
            }*/

            //if the message is not enabled then search for the next enabled message
            if (psTxMsgList->m_sTxMsgDetails.m_bEnabled == FALSE)
            {
                // Save current position
                PSTXCANMSGLIST psCurrentPosition = psTxMsgList;
                // Get Next Node in the list
                psTxMsgList = psTxMsgList->m_psNextMsgDetails;
                // If this is the last node and it is a cyclic block
                // Set the first node pointer
                //delay between block check is been added in "IF condition" so that if one mesage is unchecked,
                //it should not go to a continous loop
                if (psTxMsgList == nullptr && psTxMsg->m_bType == TRUE && !s_bDelayBetweenBlocksOnly)
                {
                    psTxMsgList = psIntialTxMsgList;
                }
                int nMsgCnt =           psTxMsg->nGetMsgCount();
                int nVarMsgCnt = 0;
                // Go through the list of nodes
                while( psTxMsgList != nullptr &&
                        psTxMsgList != psCurrentPosition &&
                        psTxMsgList->m_sTxMsgDetails.m_bEnabled == FALSE &&
                        bStopMsgBlockTx == FALSE &&
                        nVarMsgCnt <= nMsgCnt)
                {
                    // Get Next Node in the list
                    psTxMsgList = psTxMsgList->m_psNextMsgDetails;
                    // If this is the last node, then select first node as
                    // next only if it is of Cyclic type.
                    nVarMsgCnt++;
                    //delay between block check is been added in "IF condition" so that if one mesage is unchecked,
                    //it should not go to a continous loop
                    if( psTxMsgList == nullptr && psTxMsg->m_bType == TRUE && !s_bDelayBetweenBlocksOnly)
                    {
                        psTxMsgList = psIntialTxMsgList;
                    }
                }
            }
            // After search
            // Possible results:
            // psTxMsgList = nullptr  -> if it is a monoshot block and there no
            // message from got enabled from current position then return

            // psTxMsgList msg is enabled -> Transmit selected message
            // psTxMsgList msg is disabled -> There is no message in the message
            // block which has a enabled message. Skip this iteration
            bStopMsgBlockTx = CTxMsgManager::s_podGetTxMsgManager()->bGetTxStopFlag();

            if (bStopMsgBlockTx == FALSE && psTxMsgList!= nullptr &&
                    (psTxMsgList->m_sTxMsgDetails.m_bEnabled == TRUE))
            {

                hEventsTimeBlocks[0] = psTxMsg->m_hSemaphore;
                hEventsTimeBlocks[1] = g_omInformStopTx;
                DWORD dwIdx = WaitForMultipleObjects(2, hEventsTimeBlocks, FALSE, INFINITE);
                if (dwIdx == 1)
                {
                    break;
                }

                //WaitForSingleObject(psTxMsg->m_hSemaphore, INFINITE);
                // Use HIL Function to send CAN Message
                int nRet = g_pouDIL_CAN_Interface->DILC_SendMsg(g_dwClientID,
                           psTxMsgList->m_sTxMsgDetails.m_sTxMsg);
                if (nRet != S_OK)
                {
                    //::PostMessage(GUI_hDisplayWindow, WM_ERROR,
                    //            ERROR_DRIVER_API_FAIL, 0);
                }
                ReleaseSemaphore(psTxMsg->m_hSemaphore,1,lpPreviousCount);
            }
            else
            {
                TRACE("Tx block stopped\n");
            }

            // Select Next Message
            if( psTxMsgList != nullptr )
            {
                psTxMsgList = psTxMsgList->m_psNextMsgDetails;
            }
            // Set the pointer to the begining if it is the last node and
            // this is cyclic block
            if( psTxMsgList == nullptr && psTxMsg->m_bType == TRUE )
            {
                psTxMsgList = psIntialTxMsgList;
                if(s_bDelayBetweenBlocksOnly)
                {
                    //WaitForSingleObject(hEventWait, INFINITE);
                    dwIndex = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);
                    if (dwIndex == 1)
                    {
                        break;
                    }
                }
            }

        }// while
        if (mmResult == TIMERR_NOERROR)
        {
            timeEndPeriod(time.wPeriodMin);
        }
        if(Result != 0)
        {
            Result = timeKillEvent(Result);
        }
        CloseHandle(hEventWait);
    }
    //thread will reach this point only if the transmission is stopped, so stop the thread.
    if(psTxMsg != nullptr)
    {
        psTxMsg->m_omTxBlockTimerEvent.SetEvent();
        if(psTxMsg->m_sTimerThreadInfo.m_pvThread != nullptr )
        {
            // Delete the pointer in case there is any memory allocated and used
            // inside the thread function.
            psTxMsg->m_sTimerThreadInfo.m_pvThread = nullptr;
        }
        psTxMsg->m_sTimerThreadInfo.m_hThread = nullptr;

        //Clear the message list
        if(psIntialTxMsgList)
        {
            CTxWndDataStore::ouGetTxWndDataStoreObj().bDeleteMsgList(psIntialTxMsgList);
        }
        snThreadCount--; //since the thread has stopped reduce the count
    }
    return 0;
}

UINT CTxMsgManager::s_unSendMsgBlockOnKey(LPVOID pParam )
{
    PSTXMSG psTxMsg  =  static_cast<PSTXMSG> (pParam);
    PSTXCANMSGLIST psIntialTxMsgList = nullptr;
    if(psTxMsg != nullptr )
    {
        psTxMsg->m_omTxBlockKeyEvent.ResetEvent();
        PSTXCANMSGLIST psTxMsgList = new STXCANMSGLIST;

        psTxMsg->m_psTxCANMsgList->m_bModified = true;      //set this to override the default;
        //get the MsgList from global
        CTxWndDataStore::ouGetTxWndDataStoreObj().bCopyMsgList(psTxMsgList, & psTxMsg->m_psTxCANMsgList);
        psIntialTxMsgList = psTxMsgList;
        // Take the first node of Tx block
        //PSTXCANMSGLIST psTxMsgList = psTxMsg->m_psTxCANMsgList;
        // If All Messages option is enabled search for atleast one message
        // with send option enabled. If not then quit
        BOOL bMsgFound = FALSE;
        if( psTxMsg->m_bSendAllMessages == TRUE )
        {
            while( psTxMsgList != nullptr && bMsgFound == FALSE )
            {
                if( psTxMsgList->m_sTxMsgDetails.m_bEnabled == TRUE )
                {
                    bMsgFound = TRUE;
                }
                else
                {
                    psTxMsgList = psTxMsgList->m_psNextMsgDetails;
                }
            }
        }
        else
        {
            bMsgFound = TRUE;
        }

        if( bMsgFound == TRUE )
        {
            // Search through the list
            LPLONG lpPreviousCount = nullptr;
            while(psTxMsgList != nullptr  && CTxMsgManager::s_podGetTxMsgManager()->bGetTxStopFlag() == FALSE)
            {
                // Get the first enabled message
                // This block will select next available enabled message
                // If there is no message in the list that is enabled this will
                // return. A check has been introduced before transmission to
                // find this condition
                // Check whether currently available message is selected or not
                DWORD  dwWaitResult;
                dwWaitResult  = WaitForSingleObject(psTxMsg->m_omTxBlockAutoUpdateEventForKey, 0);
                if( dwWaitResult == WAIT_OBJECT_0 )
                {
                    EnterCriticalSection(&m_csUpdationLock);
                    CTxWndDataStore::ouGetTxWndDataStoreObj().bCopyMsgList(psIntialTxMsgList, & psTxMsg->m_psTxCANMsgList); //reset the values
                    LeaveCriticalSection(&m_csUpdationLock);
                    psTxMsg->m_omTxBlockAutoUpdateEventForKey.ResetEvent ();
                }
                if(  psTxMsgList == nullptr && psTxMsgList->m_sTxMsgDetails.m_bEnabled == FALSE )
                {
                    // Save current position
                    PSTXCANMSGLIST psCurrentPosition = psTxMsgList;
                    // Get Next Node in the list
                    psTxMsgList = psTxMsgList->m_psNextMsgDetails;
                    // If this is the last node then select first node as
                    // next if it is of Cyclic type
                    if( psTxMsgList == nullptr && psTxMsg->m_bType == TRUE )
                    {
                        // Set the first node
                        psTxMsgList = psIntialTxMsgList;
                    }

                    // Go through the list of nodes if current one is not enabled
                    while( psTxMsgList != nullptr &&
                            psTxMsgList != psCurrentPosition &&
                            psTxMsgList->m_sTxMsgDetails.m_bEnabled == FALSE )
                    {
                        // Get Next Node in the list
                        psTxMsgList = psTxMsgList->m_psNextMsgDetails;
                        // If this is the last node then select first node as
                        // next if it is of Cyclic type
                        if( psTxMsgList == nullptr && psTxMsg->m_bType == TRUE )
                        {
                            psTxMsgList = psIntialTxMsgList;
                        }
                    }
                }
                // After search
                // Possible results:
                // psTxMsgList = nullptr
                // if it is a monoshot block and there no message
                // from got enabled from current position then return

                // psTxMsgList msg is enabled -> Transmit selected message
                // psTxMsgList msg is disabled -> There is no message
                //                                  in the message
                // block which has a enabled message. Skip this iteration
                if( psTxMsgList != nullptr &&
                        psTxMsgList->m_sTxMsgDetails.m_bEnabled == TRUE )
                {
                    //                    EnterCriticalSection(&psTxMsg->m_sMsgBlocksCriticalSection);
                    WaitForSingleObject(psTxMsg->m_hSemaphore, INFINITE);
                    int nRet = g_pouDIL_CAN_Interface->DILC_SendMsg(g_dwClientID,
                               psTxMsgList->m_sTxMsgDetails.m_sTxMsg);
                    if(CTxWndDataStore::ouGetTxWndDataStoreObj().m_bDelayBetweenMsgBlocks)
                    {
                        //if delay between is selected then it should sleep for the time of same
                        Sleep(CTxWndDataStore::ouGetTxWndDataStoreObj().m_unTimeDelayBtwnMsgBlocks);
                    }
                    else
                    {
                        Sleep(psTxMsg->m_unTimeInterval);
                    }
                    ReleaseSemaphore(psTxMsg->m_hSemaphore,1,lpPreviousCount);
                }

                // Go to the next node
                if( psTxMsgList != nullptr )
                {
                    psTxMsgList = psTxMsgList->m_psNextMsgDetails;
                }
                // Check for the last node if not
                if( psTxMsgList == nullptr && psTxMsg->m_bType == TRUE )
                {
                    // Move to the begining
                    psTxMsgList = psIntialTxMsgList;
                }
                // If there are messages to be transmitted then only wait
                // and All Messages is unckecked
                if( psTxMsgList != nullptr && psTxMsg->m_bSendAllMessages == FALSE)
                {
                    WaitForSingleObject(psTxMsg->m_omKeyEvent,INFINITE);
                }
            }// while
        }
    }
    if(psTxMsg != nullptr)
    {
        // if thread is about to close, set the event.
        psTxMsg->m_omKeyEvent.SetEvent();
        psTxMsg->m_omTxBlockKeyEvent.SetEvent();
        // Clear any memory used
        if(psTxMsg->m_sKeyThreadInfo.m_pvThread != nullptr )
        {
            // Delete the pointer in case there is any memory allocated and used
            // inside the thread function.
            psTxMsg->m_sKeyThreadInfo.m_pvThread = nullptr;
        }
        //Clear the message list
        if(psIntialTxMsgList)
        {
            CTxWndDataStore::ouGetTxWndDataStoreObj().bDeleteMsgList(psIntialTxMsgList);
        }
        // Set the thread handle to nullptr
        psTxMsg->m_sKeyThreadInfo.m_hThread = nullptr;
    }
    return 0;
}

int CTxMsgManager::nGetBlockCount()
{
    int nBlockCount = 0;
    int* pnBlockCount = &nBlockCount;
    CTxWndDataStore::ouGetTxWndDataStoreObj().bGetTxData( TX_MSG_BLOCK_COUNT,
            (LPVOID*)&pnBlockCount);
    return nBlockCount;
}

int CTxMsgManager::nGetSizeOfTxWndConfigData()
{
    PSTXMSG psTxMsg     = m_psTxMsgBlockList;
    int nTotalByteCount = 0;
    //To store msg block count as integer
    nTotalByteCount += sizeof(int);
    while (nullptr != psTxMsg)
    {
        nTotalByteCount += psTxMsg->nGetConfigSize();
        psTxMsg = psTxMsg->m_psNextTxMsgInfo;
    }
    return nTotalByteCount;
}

void CTxMsgManager::vGetTxWndConfigData(BYTE*& /* pDesBuffer */, int& /* nBuffSize */)
{
}

void CTxMsgManager::vGetTxWndConfigData(xmlNodePtr pxmlNodePtr)
{
    CTxWndDataStore::ouGetTxWndDataStoreObj().pbyGetConfigData(pxmlNodePtr);
}

void CTxMsgManager::vSetTxWndConfigData(BYTE* pSrcBuffer, int nBuffSize)
{
    //Stop threads and clear everything
    vStopTransmission(500);
    vDeleteTxBlockMemory();
    CTxWndDataStore::ouGetTxWndDataStoreObj().pbySetConfigData(pSrcBuffer, nBuffSize);
    BOOL bAllocateMemory =
        CTxMsgManager::s_podGetTxMsgManager()->bAllocateMemoryForGlobalTxList();
    // Update Message List
    if(bAllocateMemory == TRUE )
    {
        CTxMsgManager::s_podGetTxMsgManager()->vAssignMsgBlockList();
    }
}

void  CTxMsgManager::vSetTxWndConfigData(xmlDocPtr pDoc)
{
    //Stop threads and clear everything
    vStopTransmission(500);
    vDeleteTxBlockMemory();
    CTxWndDataStore::ouGetTxWndDataStoreObj().pbySetConfigData(pDoc);
    BOOL bAllocateMemory =
        CTxMsgManager::s_podGetTxMsgManager()->bAllocateMemoryForGlobalTxList();
    // Update Message List
    if(bAllocateMemory == TRUE )
    {
        CTxMsgManager::s_podGetTxMsgManager()->vAssignMsgBlockList();
    }
}

void CTxMsgManager::vSetTxStopFlag(BOOL bStartStop)
{
    m_bStopMsgBlockTx = bStartStop;
    PSTXMSG psTxMsg           = nullptr;
    psTxMsg    = m_psTxMsgBlockList;
    while(psTxMsg != nullptr)
    {
        psTxMsg->m_omKeyEvent.SetEvent();       //set this event to quit the waiting state
        psTxMsg = psTxMsg->m_psNextTxMsgInfo;
    }
}

BOOL CTxMsgManager::bGetTxStopFlag()
{
    return m_bStopMsgBlockTx;
}
