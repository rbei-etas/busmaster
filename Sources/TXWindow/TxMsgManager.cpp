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
 * \file      TxMsgManager.cpp
 * \brief     Implementation file for CTxMsgManager class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
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

//CLient Id from the DIL
static DWORD g_dwClientID = 0;
//DIL Interface
static CBaseDIL_CAN* g_pouDIL_CAN_Interface = NULL;
// Application object declaration
//extern CCANMonitorApp theApp;

//extern HWND GUI_hDisplayWindow;
// Flag for breaking the loop of message transmission for
// selected message blocks
BOOL g_bStopSelectedMsgTx = TRUE;

// Init Static member
CTxMsgManager* CTxMsgManager::m_spodInstance = NULL;
STHREADINFO CTxMsgManager::s_sUtilThread;
CEvent CTxMsgManager::s_omState;
CFlags CTxMsgManager::s_TxFlags;
//extern CRITICAL_SECTION g_CritSectDllBufferRead;


// Key Handler proc CMap Hash table size.
#define defKEY_HANDLER_HASH_TABLE_SIZE      17
/*******************************************************************************
  Function Name  : CTxMsgManager
  Description    : Standard default constructor
  Member of      : CTxMsgManager
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 19.4.2005
  Modifications  :
*******************************************************************************/
CTxMsgManager::CTxMsgManager()
{
    // Set some prime number
    m_omMonoshotBlocks.InitHashTable( defKEY_HANDLER_HASH_TABLE_SIZE );
    //m_pouMsgSignal = NULL;
    m_psTxMsgBlockList = NULL;
    m_bStopMsgBlockTx = TRUE;
}

/*******************************************************************************
  Function Name  : ~CTxMsgManager
  Description    : Standard Destructor
  Member of      : CTxMsgManager
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 19.4.2005
  Modifications  :
*******************************************************************************/
CTxMsgManager::~CTxMsgManager()
{
}

/*******************************************************************************
  Function Name  : vSetClientID
  Description    : Assigns g_dwClientID to client ID supplied.
  Member of      : CTxMsgManager
  Functionality  : -
  Author(s)      : ArunKumat K
  Date Created   : 03.08.2010
  Modifications  :
*******************************************************************************/
void CTxMsgManager::vSetClientID(DWORD dwClientID)
{
    g_dwClientID = dwClientID;
}

/*******************************************************************************
  Function Name  : vSetDILInterfacePtr
  Description    : Assigns g_pouDIL_CAN_Interface variable to DIL interface pointer
  Member of      : CTxMsgManager
  Functionality  : -
  Author(s)      : ArunKumat K
  Date Created   : 03.08.2010
  Modifications  :
*******************************************************************************/
void CTxMsgManager::vSetDILInterfacePtr(void* ptrDILIntrf)
{
    g_pouDIL_CAN_Interface = (CBaseDIL_CAN*)ptrDILIntrf;
}

/*******************************************************************************
  Function Name  : pGetDILInterfacePtr
  Description    : Returns DIL interface pointer (g_pouDIL_CAN_Interface)
  Member of      : CTxMsgManager
  Functionality  : -
  Author(s)      : ArunKumat K
  Date Created   : 24.03.2011
  Modifications  :
*******************************************************************************/
void* CTxMsgManager::pGetDILInterfacePtr()
{
    return (void*)g_pouDIL_CAN_Interface;
}

/*******************************************************************************
  Function Name  : s_podGetTxMsgManager
  Input(s)       : -
  Output         : CTxMsgManager * - Pointer to the singleton object
  Functionality  : This function will return the pointer to the singleton object
                   of CTxMsgManager. In case of memory allocation failure this
                   function will return NULL
  Member of      : CTxMsgManager
  Author(s)      : Raja N
  Date Created   : 19.4.2005
  Modifications  :
*******************************************************************************/
CTxMsgManager* CTxMsgManager::s_podGetTxMsgManager()
{
    if( m_spodInstance == NULL )
    {
        m_spodInstance = new CTxMsgManager;

        // Handling NULL condition is caller's duty
        if( m_spodInstance == NULL )
        {
            // Help debugging
            ASSERT( FALSE );
        }
    }

    // Return the pointer or NULL in case of failure
    return m_spodInstance;
}

/*******************************************************************************
  Function Name  : s_bDeleteTxMsgManager
  Input(s)       : -
  Output         : BOOL - Delete result. FALSE if the object is not valid
  Functionality  : This function will delete singleton object. If it is null
                   then this function will return FALSE to indicate invalid
                   operation.
  Member of      : CTxMsgManager
  Author(s)      : Raja N
  Date Created   : 20.4.2005
  Modifications  :
*******************************************************************************/
BOOL CTxMsgManager::s_bDeleteTxMsgManager()
{
    BOOL bDelete = FALSE;

    // Check instance pointer
    if( m_spodInstance != NULL )
    {
        // Delete memory associated with this pointer
        delete m_spodInstance;
        m_spodInstance = NULL;
        // Set result to success
        bDelete = TRUE;
    }

    // Return the result
    return bDelete;
}

/******************************************************************************/
/*  Function Name    :  vStartTransmission                                    */
/*  Input(s)         :  UCHAR ucKeyVal  : key pressed                         */
/*  Output           :                                                        */
/*  Functionality    :  This function will start transmission on press ofa key*/
/*                      and on time. This will be called from CMainFrame      */
/*                                                                            */
/*  Member of        :  CTxMsgManager                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  22.07.2004, Modified thread creation logic to create  */
/*                      both time and key threads if the type is set for both */
/*                      and added a CMap search to avoid thread creation again*/
/*                      for monoshot key block                                */
/******************************************************************************/
VOID CTxMsgManager::vStartTransmission(UCHAR ucKeyVal)
{
    if(m_psTxMsgBlockList != NULL )
    {
        PSMSGBLOCKLIST psMsgBlock = NULL;
        PSTXMSG psTxMsg           = NULL;
        // Get the message block header pointer from configuration module.
        psMsgBlock = CTxWndDataStore::ouGetTxWndDataStoreObj().psReturnMsgBlockPointer();
        // Get the header pointer from global list
        psTxMsg    = m_psTxMsgBlockList;
        // Get the message block count. No memory allocation is required.
        UINT unMsgBlockCount = NULL;
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
            while( psMsgBlock != NULL && psTxMsg != NULL &&
                    unCount < unMsgBlockCount)
            {
                // Check if the valid pointers are obtained before it reaches
                // the if pointer is not valid then there is not match of the
                // key pressed.Also check if there is a match and if that block
                // is active
                if( psMsgBlock->m_unMsgCount >0 &&
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
                        if(psTxMsg->m_sKeyThreadInfo.m_hThread != NULL)
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
                        if( psTxMsg->m_sKeyThreadInfo.m_hThread == NULL )
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
            while(psMsgBlock != NULL && psTxMsg != NULL &&
                    unCount < unMsgBlockCount)
            {
                // Set the flag to FALSE to indicate that user has started the
                // transmission and it will be set to true for key trigger
                // as key trigger thread should be created only once even thread
                // has existed.
                //  m_bTransmittedOnce = FALSE;
                if( psMsgBlock->m_unMsgCount > 0 &&
                        psMsgBlock->m_bActive == TRUE &&
                        IS_TIME_TRIGGERED(psMsgBlock->m_ucTrigger) &&
                        psTxMsg->m_sTimerThreadInfo.m_hThread == NULL )
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

VOID CTxMsgManager::vStopTransmission(UINT unMaxWaitTime)
{
    vSetTxStopFlag(TRUE);
    PSTXMSG psTxMsg = m_psTxMsgBlockList;
    // Store wait return value of timer and key threads seprately
    DWORD   dwTimerThreadStatus = WAIT_ABANDONED;
    DWORD   dwKeyThreadStatus = WAIT_ABANDONED;

    while(psTxMsg != NULL )
    {
        // Set the event for thread waiting for key event to set.
        // This is only if a block is trigger on key and so
        // the m_pomKeyEvent will not be null.
        if( psTxMsg->m_sKeyThreadInfo.m_hThread != NULL)
        {
            // Set the Key Event so that it will terminate by iteself
            psTxMsg->m_omKeyEvent.SetEvent();
            Sleep(0);
        }

        // If timer tharead is active then wait for a while
        if(psTxMsg->m_sTimerThreadInfo.m_hThread != NULL )
        {
            // Wait for thread to exit.
            dwTimerThreadStatus =
                WaitForSingleObject( psTxMsg->m_omTxBlockTimerEvent,
                                     unMaxWaitTime );
        }

        // If key thread is active then wait for key thread termination
        if(psTxMsg->m_sKeyThreadInfo.m_hThread != NULL )
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
                psTxMsg->m_sTimerThreadInfo.m_hThread != NULL)
        {
            TerminateThread(psTxMsg->m_sTimerThreadInfo.m_hThread, 0);
            // Invalidate the handle
            psTxMsg->m_sTimerThreadInfo.m_hThread = NULL;
            // Delete if any memory is allocated for this thread.
            // Currently there are all global data so not required
            // to be deleted here.
            /***********************************************************/
            // Right now Tx timer thread isn't using any dynamic memory
            // So this is commented
            /*if( psTxMsg->m_sTimerThreadInfo.m_pvThread !=NULL )
            {
                m_asUtilThread[defTX_SEL_MSG_THREAD].m_pvThread = NULL;
            }*/
            /***********************************************************/
        }

        // If the key handler is not terminated yet then kill the thread
        if( dwKeyThreadStatus == WAIT_TIMEOUT &&
                psTxMsg->m_sKeyThreadInfo.m_hThread != NULL)
        {
            TerminateThread(psTxMsg->m_sKeyThreadInfo.m_hThread, 0);
            // Invalidate the handle
            psTxMsg->m_sKeyThreadInfo.m_hThread = NULL;
            /***********************************************************/
            // Delete if any memory is allocated for this thread.
            // Currently there are all global data so not required
            // to be deleted here.
            /*if( psTxMsg->m_sThreadInfo1.m_pvThread !=NULL )
            {
                m_asUtilThread[defTX_SEL_MSG_THREAD].m_pvThread = NULL;
            }*/
            /***********************************************************/
        }

        // Go to the next thread node
        psTxMsg = psTxMsg->m_psNextTxMsgInfo;
    }
}
/******************************************************************************/
/*  Function Name    :  bAllocateMemoryForGlobalTxList                        */
/*  Input(s)         :  TRUE or FALSE                                         */
/*  Output           :                                                        */
/*  Functionality    :  This function allocate memory to global list. Memory  */
/*                      will be allocated only if the list is not already     */
/*                      having the correct size. Only extra element will be   */
/*                      added in case the count is less then m_unMsgBlockCount*/
/*  Member of        :  CTxMsgManager                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  21.01.2004, Instead of critical section, semaphore is */
/*                                  used.                                     */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  22.07.2004, Modified the semophare maximun count to 2 */
/*                      as key and time threads can access it simultaniously  */
/******************************************************************************/
BOOL CTxMsgManager::bAllocateMemoryForGlobalTxList()
{
    BOOL bReturn = TRUE;
    PSTXMSG psTxMsg    = m_psTxMsgBlockList;
    PSTXMSG psTxMsgMem = NULL;
    UINT unCount = 0;
    UINT unMsgBlockCount = 0;
    // Get the message block count. No memory allocation is required.
    UINT* punCount = &unMsgBlockCount;
    CTxWndDataStore::ouGetTxWndDataStoreObj().bGetTxData(TX_MSG_BLOCK_COUNT, (void**)&punCount);

    if(unMsgBlockCount <= 0 )
    {
        bReturn = FALSE;
    }

    if(psTxMsg != NULL && bReturn != FALSE)
    {
        unCount++;

        // Go to end of the list
        while(psTxMsg->m_psNextTxMsgInfo != NULL )
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

        if(psTxMsgMem == NULL )
        {
            bReturn = FALSE;
        }
        else
        {
            //           InitializeCriticalSection(&psTxMsgMem->m_sMsgBlocksCriticalSection);
            // Set the maximum count to two
            psTxMsgMem->m_hSemaphore = CreateSemaphore( NULL,
                                       defTX_BLOCK_SEM_MAX_COUNT,
                                       defTX_BLOCK_SEM_MAX_COUNT,
                                       "");
            psTxMsgMem->m_psTxCANMsgList         = NULL;
            psTxMsgMem->m_unTimeInterval         = 0;
            psTxMsgMem->m_sTimerThreadInfo.m_hThread  = NULL;
            psTxMsgMem->m_sTimerThreadInfo.m_pvThread = NULL;
            psTxMsgMem->m_sKeyThreadInfo.m_hThread  = NULL;
            psTxMsgMem->m_sKeyThreadInfo.m_pvThread = NULL;
            psTxMsgMem->m_psNextTxMsgInfo        = NULL;
            psTxMsgMem->m_bType                  = FALSE;

            if(psTxMsg != NULL )
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


/******************************************************************************/
/*  Function Name    :  vAssignMsgBlockList                                   */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will assign the message list pointers to*/
/*                      global list. The pointers will be obtained from       */
/*                      configuration block.                                  */
/*                                                                            */
/*  Member of        :  CTxMsgManager                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
VOID CTxMsgManager::vAssignMsgBlockList()
{
    PSMSGBLOCKLIST psMsgBlock = CTxWndDataStore::ouGetTxWndDataStoreObj().psReturnMsgBlockPointer();
    PSTXMSG psTxMsg           = m_psTxMsgBlockList;

    while(psTxMsg != NULL && psMsgBlock != NULL )
    {
        psTxMsg->m_unTimeInterval = psMsgBlock->m_unTimeInterval;

        if(psMsgBlock->m_unMsgCount > 0 )
        {
            psTxMsg->m_psTxCANMsgList = psMsgBlock->m_psTxCANMsgList;
        }
        else
        {
            psTxMsg->m_psTxCANMsgList = NULL;
        }

        psTxMsg->m_bType          = psMsgBlock->m_bType;
        psTxMsg    = psTxMsg->m_psNextTxMsgInfo;
        psMsgBlock = psMsgBlock->m_psNextMsgBlocksList;
    }
}

/******************************************************************************/
/*  Function Name    :  vDeleteTxBlockMemory                                 */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will delete the memory allocated for    */
/*                      global list.                                          */
/*                                                                            */
/*  Member of        :  CSendMultiMsgDlg                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  21.01.2004, Instead of critical section, semaphore is */
/*                                  used.                                     */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  22.07.2004, Removed deletion of CEvent object pointer */
/*                      as it is changed as member object                     */
/******************************************************************************/
VOID CTxMsgManager::vDeleteTxBlockMemory()
{
    if(m_psTxMsgBlockList != NULL )
    {
        PSTXMSG psTxMsg = m_psTxMsgBlockList; // Get the head pointer
        PSTXMSG psNextTxMsg = NULL;

        // loop through till last list is reached and delete it one by one
        // from begining. It is a singly link list.
        while(psTxMsg != NULL )
        {
            ///            DeleteCriticalSection(&psTxMsg->m_sMsgBlocksCriticalSection);
            if(psTxMsg->m_hSemaphore != NULL )
            {
                CloseHandle(psTxMsg->m_hSemaphore);
            }

            psNextTxMsg = psTxMsg->m_psNextTxMsgInfo;
            delete psTxMsg;
            psTxMsg = NULL;
            psTxMsg = psNextTxMsg;
        }

        // initialise the pointer to NULL
        m_psTxMsgBlockList = NULL;
    }
}

/******************************************************************************
    Function Name    :  g_unSendSelectedMsg
    Input(s)         :  pParam - Typecasted address of PSTXMSG pointer
    Output           :  Zero
    Functionality    :  This is a thread control function to process
                        transmission of selected message from the dialog.
    Member of        :  CTxMsgManager
    Friend of        :      -
    Author(s)        :  Amitesh Bharti
    Date Created     :  15.01.2004
    Modifiecations   :  Raja N on 08.09.2004, Replaced nWriteToCAN with HIL
                        function
******************************************************************************/
const int SIZE_STCAN_MSG = sizeof(STCAN_MSG);
UINT CTxMsgManager::s_unSendSelectedMsg(LPVOID pParam )
{
    s_omState.ResetEvent();
    PSTXSELMSGDATA psTxCanMsg = static_cast <PSTXSELMSGDATA> (pParam);

    if(psTxCanMsg != NULL)
    {
        s_sUtilThread.m_pvThread = (VOID*)psTxCanMsg;
        UINT unIndex = 0;

        while(unIndex < psTxCanMsg->m_unCount && g_bStopSelectedMsgTx == FALSE)
        {
            if (psTxCanMsg->m_psTxMsg[unIndex].m_unMsgID != -1)
            {
                // Use HIL function to send CAN message
                int nReturn = g_pouDIL_CAN_Interface->DILC_SendMsg(g_dwClientID, psTxCanMsg->m_psTxMsg[unIndex]);

                if (nReturn != S_OK)
                {
                    //::PostMessage(GUI_hDisplayWindow, WM_ERROR,
                    //            ERROR_DRIVER_API_FAIL, 0);
                }

                //SDLL_MSG sTempDllMsg;
                //memcpy(&sTempDllMsg.sRxMsg,&psTxCanMsg->m_psTxMsg[unIndex],SIZE_STCAN_MSG);
                //sTempDllMsg.h_DllHandle=NULL;
                //EnterCriticalSection(&g_CritSectDllBufferRead);
                //g_omLstTxCheckMsgs.AddTail(sTempDllMsg);
                //LeaveCriticalSection(&g_CritSectDllBufferRead);
            }

            unIndex++;
        }

        delete [](psTxCanMsg->m_psTxMsg);
        psTxCanMsg->m_psTxMsg = NULL;
        delete psTxCanMsg;
        psTxCanMsg = NULL;
    }

    s_sUtilThread.m_pvThread = NULL;
    g_bStopSelectedMsgTx = TRUE;
    s_omState.SetEvent();
    return 0;
}

/******************************************************************************
    Function Name    :  g_unSendMsgBlockOnTime
    Input(s)         :  pParam - Typecasted address of PSTXMSG pointer
    Output           :  Zero
    Functionality    :  This is a thread control function to process
                        message block transmission on time trigger.
    Member of        :  Global Thread Function
    Friend of        :      -
    Author(s)        :  Amitesh Bharti
    Date Created     :  15.01.2004
    Modifications    :  Amitesh Bharti
                        21.01.2004, Instead of critical section, semaphore is
                        used.
    Modifications    :  Raja N on 22.07.2004
                        Changed the event name to refer timer member. This is
                        done to support multiple triggering of same Tx block.
                        And added check for Global stop flag before transmission
                        to avoid sending message even after stopping Tx.
    Modifiecations   :  Raja N on 08.09.2004, Replaced nWriteToCAN with HIL
                        function
    Modifiecations   :  Raja N on 25.04.2005, Added code to check for enabled
                        member before transmissting a message and to select
                        next available enabled message
******************************************************************************/
UINT CTxMsgManager::s_unSendMsgBlockOnTime(LPVOID pParam )
{
    PSTXMSG psTxMsg  =  static_cast<PSTXMSG> (pParam);

    if (psTxMsg != NULL)
    {
        UINT unTimeInterval = psTxMsg->m_unTimeInterval;
        psTxMsg->m_omTxBlockTimerEvent.ResetEvent();
        PSTXCANMSGLIST psTxMsgList = psTxMsg->m_psTxCANMsgList;
        TIMECAPS time;
        MMRESULT mmResult = TIMERR_NOERROR;
        HANDLE hEventWait = NULL;

        if (timeGetDevCaps(&time, sizeof(TIMECAPS)) == TIMERR_NOERROR)
        {
            mmResult = timeBeginPeriod(time.wPeriodMin);
        }

        // Create the event object to wait for
        hEventWait = CreateEvent(NULL, FALSE, FALSE, NULL);
        MMRESULT Result = 0;
        Result = timeSetEvent(unTimeInterval, time.wPeriodMin,
                              (LPTIMECALLBACK) hEventWait, NULL,
                              TIME_CALLBACK_EVENT_SET | TIME_PERIODIC);
        LPLONG lpPreviousCount = NULL;
        BOOL bStopMsgBlockTx = CTxMsgManager::s_podGetTxMsgManager()->bGetTxStopFlag();

        while ((psTxMsgList != NULL) && (bStopMsgBlockTx == FALSE))
        {
            // Wait for the event
            WaitForSingleObject(hEventWait, INFINITE);
            //EnterCriticalSection(&g_sMsgBlocksCriticalSection);
            //EnterCriticalSection(&psTxMsg->m_sMsgBlocksCriticalSection);
            //Transmit only if the global flag is set to go

            // Get the first enabled message
            // This block will select next available enabled message
            // If there is no message in the list that is enabled this will
            // return. A check has been introduced before transmission to
            // find this condition
            // Check whether currently available message is selected or not
            if (psTxMsgList->m_sTxMsgDetails.m_bEnabled == FALSE)
            {
                // Save current position
                PSTXCANMSGLIST psCurrentPosition = psTxMsgList;
                // Get Next Node in the list
                psTxMsgList = psTxMsgList->m_psNextMsgDetails;

                // If this is the last node and it is a cyclic block
                // Set the first node pointer
                if (psTxMsgList == NULL && psTxMsg->m_bType == TRUE)
                {
                    psTxMsgList = psTxMsg->m_psTxCANMsgList;
                }

                // Go through the list of nodes
                while( psTxMsgList != NULL &&
                        psTxMsgList != psCurrentPosition &&
                        psTxMsgList->m_sTxMsgDetails.m_bEnabled == FALSE )
                {
                    // Get Next Node in the list
                    psTxMsgList = psTxMsgList->m_psNextMsgDetails;

                    // If this is the last node then select first node as
                    // next if it is of Cyclic type
                    if( psTxMsgList == NULL && psTxMsg->m_bType == TRUE )
                    {
                        psTxMsgList = psTxMsg->m_psTxCANMsgList;
                    }
                }
            }

            // After search
            // Possible results:
            // psTxMsgList = NULL  -> if it is a monoshot block and there no
            // message from got enabled from current position then return
            // psTxMsgList msg is enabled -> Transmit selected message
            // psTxMsgList msg is disabled -> There is no message in the message
            // block which has a enabled message. Skip this iteration
            bStopMsgBlockTx = CTxMsgManager::s_podGetTxMsgManager()->bGetTxStopFlag();

            if (bStopMsgBlockTx == FALSE && psTxMsgList!= NULL &&
                    (psTxMsgList->m_sTxMsgDetails.m_bEnabled == TRUE))
            {
                WaitForSingleObject(psTxMsg->m_hSemaphore, INFINITE);
                // Use HIL Function to send CAN Message
                int nRet = g_pouDIL_CAN_Interface->DILC_SendMsg(g_dwClientID,
                           psTxMsgList->m_sTxMsgDetails.m_sTxMsg);

                if (nRet != S_OK)
                {
                    //::PostMessage(GUI_hDisplayWindow, WM_ERROR,
                    //            ERROR_DRIVER_API_FAIL, 0);
                }

                //SDLL_MSG sTempDllMsg;
                //memcpy(&sTempDllMsg.sRxMsg,&psTxMsgList->m_sTxMsgDetails.m_sTxMsg,SIZE_STCAN_MSG);
                //sTempDllMsg.h_DllHandle=NULL;
                //EnterCriticalSection(&g_CritSectDllBufferRead);
                //g_omLstTxCheckMsgs.AddTail(sTempDllMsg);
                //LeaveCriticalSection(&g_CritSectDllBufferRead);
                ReleaseSemaphore(psTxMsg->m_hSemaphore,1,lpPreviousCount);
            }
            else
            {
                TRACE(_T("Tx block stopped\n"));
            }

            //LeaveCriticalSection(&psTxMsg->m_sMsgBlocksCriticalSection);
            // LeaveCriticalSection(&g_sMsgBlocksCriticalSection);

            // Select Next Message
            if( psTxMsgList != NULL )
            {
                psTxMsgList = psTxMsgList->m_psNextMsgDetails;
            }

            // Set the pointer to the begining if it is the last node and
            // this is cyclic block
            if( psTxMsgList == NULL && psTxMsg->m_bType == TRUE )
            {
                psTxMsgList = psTxMsg->m_psTxCANMsgList;
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

    psTxMsg->m_omTxBlockTimerEvent.SetEvent();

    if(psTxMsg->m_sTimerThreadInfo.m_pvThread != NULL )
    {
        // Delete the pointer in case there is any memory allocated and used
        // inside the thread function.
        psTxMsg->m_sTimerThreadInfo.m_pvThread = NULL;
    }

    psTxMsg->m_sTimerThreadInfo.m_hThread = NULL;
    return 0;
}

/******************************************************************************
    Function Name    :  g_unSendMsgBlockOnKey
    Input(s)         :  pParam - Typecasted address of PSTXMSG pointer
    Output           :  Zero
    Functionality    :  This is a thread control function to process
                        message block transmission on key trigger.
    Member of        :  Global Thread Function
    Friend of        :      -
    Author(s)        :  Amitesh Bharti
    Date Created     :  15.01.2004
    Modifications    :  Amitesh Bharti
                        21.01.2004, Instead of critical section, semaphore is
                        used.
    Modifications    :  Raja N
                        22.07.2004, Modified the code to check the thread handle
                        to verify the thread's existance and added CMap
                        structure to store monoshot key handlers so that it will
                        not transmitter again. Removed dynamic creation of
                        CEvent object.
    Modifiecations   :  Raja N on 08.09.2004, Replaced nWriteToCAN with HIL
                        function
    Modifiecations   :  Raja N on 25.04.2005, Added code to check for enabled
                        member before transmissting a message and to select
                        next available enabled message
    Modifiecations   :  ArunKumar K on 28.10.2011, Addition of delay after SendMsg(..)
                        to reduce bus overload.
******************************************************************************/
UINT CTxMsgManager::s_unSendMsgBlockOnKey(LPVOID pParam )
{
    PSTXMSG psTxMsg  =  static_cast<PSTXMSG> (pParam);

    if(psTxMsg != NULL )
    {
        psTxMsg->m_omTxBlockKeyEvent.ResetEvent();
        // Take the first node of Tx block
        PSTXCANMSGLIST psTxMsgList = psTxMsg->m_psTxCANMsgList;
        // If All Messages option is enabled search for atleast one message
        // with send option enabled. If not then quit
        BOOL bMsgFound = FALSE;

        if( psTxMsg->m_bSendAllMessages == TRUE )
        {
            while( psTxMsgList != NULL && bMsgFound == FALSE )
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
            LPLONG lpPreviousCount = NULL;

            while(psTxMsgList != NULL  && CTxMsgManager::s_podGetTxMsgManager()->bGetTxStopFlag() == FALSE)
            {
                // Get the first enabled message
                // This block will select next available enabled message
                // If there is no message in the list that is enabled this will
                // return. A check has been introduced before transmission to
                // find this condition
                // Check whether currently available message is selected or not
                if( psTxMsgList->m_sTxMsgDetails.m_bEnabled == FALSE )
                {
                    // Save current position
                    PSTXCANMSGLIST psCurrentPosition = psTxMsgList;
                    // Get Next Node in the list
                    psTxMsgList = psTxMsgList->m_psNextMsgDetails;

                    // If this is the last node then select first node as
                    // next if it is of Cyclic type
                    if( psTxMsgList == NULL && psTxMsg->m_bType == TRUE )
                    {
                        // Set the first node
                        psTxMsgList = psTxMsg->m_psTxCANMsgList;
                    }

                    // Go through the list of nodes if current one is not enabled
                    while( psTxMsgList != NULL &&
                            psTxMsgList != psCurrentPosition &&
                            psTxMsgList->m_sTxMsgDetails.m_bEnabled == FALSE )
                    {
                        // Get Next Node in the list
                        psTxMsgList = psTxMsgList->m_psNextMsgDetails;

                        // If this is the last node then select first node as
                        // next if it is of Cyclic type
                        if( psTxMsgList == NULL && psTxMsg->m_bType == TRUE )
                        {
                            psTxMsgList = psTxMsg->m_psTxCANMsgList;
                        }
                    }
                }

                // After search
                // Possible results:
                // psTxMsgList = NULL
                // if it is a monoshot block and there no message
                // from got enabled from current position then return

                // psTxMsgList msg is enabled -> Transmit selected message
                // psTxMsgList msg is disabled -> There is no message
                //                                  in the message
                // block which has a enabled message. Skip this iteration
                if( psTxMsgList != NULL &&
                        psTxMsgList->m_sTxMsgDetails.m_bEnabled == TRUE )
                {
                    //                    EnterCriticalSection(&psTxMsg->m_sMsgBlocksCriticalSection);
                    WaitForSingleObject(psTxMsg->m_hSemaphore, INFINITE);
                    int nRet = g_pouDIL_CAN_Interface->DILC_SendMsg(g_dwClientID,
                               psTxMsgList->m_sTxMsgDetails.m_sTxMsg);
                    Sleep(psTxMsg->m_unTimeInterval);

                    if (nRet != S_OK)
                    {
                        //::PostMessage(GUI_hDisplayWindow, WM_ERROR,
                        //            ERROR_DRIVER_API_FAIL, 0);
                    }

                    /*SDLL_MSG sTempDllMsg;
                    memcpy(&sTempDllMsg.sRxMsg,&psTxMsgList->m_sTxMsgDetails.m_sTxMsg,SIZE_STCAN_MSG);
                    sTempDllMsg.h_DllHandle=NULL;*/
                    //EnterCriticalSection(&g_CritSectDllBufferRead);
                    //g_omLstTxCheckMsgs.AddTail(sTempDllMsg);
                    //LeaveCriticalSection(&g_CritSectDllBufferRead);
                    ReleaseSemaphore(psTxMsg->m_hSemaphore,1,lpPreviousCount);
                    //                    LeaveCriticalSection(&psTxMsg->m_sMsgBlocksCriticalSection);
                }

                // Go to the next node
                if( psTxMsgList != NULL )
                {
                    psTxMsgList = psTxMsgList->m_psNextMsgDetails;
                }

                // Check for the last node if not
                if( psTxMsgList == NULL && psTxMsg->m_bType == TRUE )
                {
                    // Move to the begining
                    psTxMsgList = psTxMsg->m_psTxCANMsgList;
                }

                // If there are messages to be transmitted then only wait
                // and All Messages is unckecked
                if( psTxMsgList != NULL && psTxMsg->m_bSendAllMessages == FALSE)
                {
                    WaitForSingleObject(psTxMsg->m_omKeyEvent,INFINITE);
                }
            }// while
        }
    }

    // if thread is about to close, set the event.
    psTxMsg->m_omKeyEvent.SetEvent();
    psTxMsg->m_omTxBlockKeyEvent.SetEvent();

    // Clear any memory used
    if(psTxMsg->m_sKeyThreadInfo.m_pvThread != NULL )
    {
        // Delete the pointer in case there is any memory allocated and used
        // inside the thread function.
        psTxMsg->m_sKeyThreadInfo.m_pvThread = NULL;
    }

    // Set the thread handle to NULL
    psTxMsg->m_sKeyThreadInfo.m_hThread = NULL;
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

    while (NULL != psTxMsg)
    {
        nTotalByteCount += psTxMsg->nGetConfigSize();
        psTxMsg = psTxMsg->m_psNextTxMsgInfo;
    }

    return nTotalByteCount;
}

void CTxMsgManager::vGetTxWndConfigData(BYTE*& pDesBuffer, int& nBuffSize)
{
    CTxWndDataStore::ouGetTxWndDataStoreObj().pbyGetConfigData(pDesBuffer, nBuffSize);
    /*nBuffSize = nGetSizeOfTxWndConfigData();
    pDesBuffer = new BYTE[nBuffSize];
    BYTE* pDesBufferTemp = pDesBuffer;
    int nBlockCount = nGetBlockCount();
    memcpy(pDesBufferTemp, &nBlockCount, sizeof(nBlockCount));
    pDesBufferTemp += sizeof(nBlockCount);
    PSTXMSG psTxMsg     = m_psTxMsgBlockList;
    while (NULL != psTxMsg)
    {
        pDesBufferTemp = psTxMsg->pbGetBlockConfigData(pDesBufferTemp);
        psTxMsg = psTxMsg->m_psNextTxMsgInfo;
    }*/
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

    /*int nBlockCount = 0;
    memcpy(&nBlockCount, pSrcBuffer, sizeof(nBlockCount));
    pSrcBuffer += sizeof(nBlockCount);
    PSTXMSG psTxTemp = NULL, psTxPrev = NULL;
    m_psTxMsgBlockList = NULL;
    for (int i = 0; i < nBlockCount; i++)
    {
        psTxTemp = new STXMSG;
        pSrcBuffer = psTxTemp->pbSetBlockConfigData(pSrcBuffer);
        if (NULL == m_psTxMsgBlockList)
        {
            m_psTxMsgBlockList = psTxTemp;
            psTxPrev = psTxTemp;
        }
        else
        {
            psTxPrev->m_psNextTxMsgInfo = psTxTemp;
            psTxPrev = psTxTemp;
        }
    }*/
}

BOOL CTxMsgManager::bIsTxWndConfigChanged()
{
    BOOL bReturn = TRUE;
    /**************Get the old buffer pointer and then proceed *********/
    BYTE* pOldConfigBuff = NULL;
    int nOldBufferCount = 0;
    /**************Get the old buffer pointer and then proceed *********/
    BYTE* pCurrConfigBuff = NULL;
    int nCurrBuffSize = 0;
    vGetTxWndConfigData(pCurrConfigBuff, nCurrBuffSize);

    if (nCurrBuffSize == nOldBufferCount)
    {
        if ( !memcmp (pCurrConfigBuff,pOldConfigBuff, nCurrBuffSize))
        {
            bReturn = FALSE;
        }
    }

    return bReturn;
}

void CTxMsgManager::vSetTxStopFlag(BOOL bStartStop)
{
    m_bStopMsgBlockTx = bStartStop;
}

BOOL CTxMsgManager::bGetTxStopFlag()
{
    return m_bStopMsgBlockTx;
}