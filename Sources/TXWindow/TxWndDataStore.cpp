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
 * \file      TxWndDataStore.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "TxWindow_stdafx.h"
#include "TxWndDataStore.h"
#include "Include/Utils_macro.h"

// Tx Window Splitter Position
#define defTX_WND_ROOT_SPLITTER_RATIO       0.865
#define defTX_WND_LEFT_SPLITTER_RATIO       0.27
#define defTX_WND_RIGHT_SPLITTER_RATIO      0.440

CTxWndDataStore CTxWndDataStore::m_sTxWndDataStoreObj;

CTxWndDataStore::CTxWndDataStore(void)
{
    m_unNumberOfMsgBlockCount  = 0;
    m_bIsConfigurationModified = FALSE;
    m_psMsgBlockList           = NULL;
    m_sTxWndPlacement.rcNormalPosition.top = -1;
    m_sTxWndPlacement.length = 0;
    m_sTxMsgWndSplitterPos.m_nRootSplitterData[0][0] = -1;
}

CTxWndDataStore::~CTxWndDataStore(void)
{
    vReleaseMultiMsgInfo(m_psMsgBlockList);
    m_psMsgBlockList = NULL;
}

CTxWndDataStore& CTxWndDataStore::ouGetTxWndDataStoreObj()
{
    return m_sTxWndDataStoreObj;
}

BOOL CTxWndDataStore::bGetTxData(eTXWNDDETAILS  eParam, LPVOID* lpData)
{
    BOOL bRetVal = TRUE;

    switch(eParam)
    {
        case TX_MSG_BLOCK_COUNT:
        {
            UINT* punCount = static_cast<UINT*>(*lpData);

            if (punCount != NULL)
            {
                *(punCount) = static_cast<UINT>(m_unNumberOfMsgBlockCount);
            }
            else
            {
                bRetVal = FALSE;
            }
        }
        break;

        case TX_WND_SPLITTER_DATA: // Tx configure splitter position
        {
            PSTXMSGSPLITTERDATA psData =
                static_cast<PSTXMSGSPLITTERDATA>(*lpData);
            *psData = m_sTxMsgWndSplitterPos;
        }
        break;

        case TX_SEND_MULTI_MSGS:
        {
            *lpData = NULL;
            PSMSGBLOCKLIST psMsgBlockList  = new SMSGBLOCKLIST;

            if (psMsgBlockList != NULL)
            {
                vInitialiseMsgBlock(psMsgBlockList);
                // multiple message
                bRetVal = bGetMultiMsgInfo(psMsgBlockList);

                // release memory if any error occurs
                if (bRetVal == FALSE)
                {
                    vReleaseMultiMsgInfo(psMsgBlockList);
                    psMsgBlockList = NULL;
                }
            }
            else
            {
                // memory not enough
                bRetVal = FALSE;
            }

            if (bRetVal == TRUE)
            {
                *lpData = static_cast<LPVOID>(psMsgBlockList);
            }
        }
        break;

        case TX_WINDOW_PLACEMENT:
        {
            WINDOWPLACEMENT* psData =
                static_cast<WINDOWPLACEMENT*>(*lpData);
            *psData = m_sTxWndPlacement;
        }
        break;

        default:
        {
            ASSERT(FALSE);
        }
        break;
    }

    return bRetVal;
}

BOOL CTxWndDataStore::bSetTxData(eTXWNDDETAILS  eParam, LPVOID lpVoid)
{
    BOOL bRetVal = TRUE;

    switch(eParam)
    {
        case TX_MSG_BLOCK_COUNT:
        {
            UINT* punCount;
            punCount = static_cast<UINT*>(lpVoid);

            if (punCount != NULL)
            {
                m_unNumberOfMsgBlockCount = *(punCount);
            }
            else
            {
                bRetVal = FALSE;
            }

            if (bRetVal == FALSE)
            {
                m_unNumberOfMsgBlockCount = 0;
            }

            m_bIsConfigurationModified = TRUE;
        }
        break;

        case TX_WND_SPLITTER_DATA: // Tx configure splitter position
        {
            PSTXMSGSPLITTERDATA psData =
                static_cast<PSTXMSGSPLITTERDATA>(lpVoid);
            m_sTxMsgWndSplitterPos = *psData;
            m_bIsConfigurationModified = TRUE;
        }
        break;

        case TX_SEND_MULTI_MSGS:
        {
            PSMSGBLOCKLIST psMsgBlockInfo =
                static_cast<PSMSGBLOCKLIST>(lpVoid);
            // multiple message
            bRetVal = bSetMultiMsgInfo(psMsgBlockInfo);

            if (bRetVal == FALSE)
            {
                // set the default values
                vReleaseMultiMsgInfo(m_psMsgBlockList);
                m_psMsgBlockList = NULL;
            }

            m_bIsConfigurationModified = TRUE;
        }
        break;

        case TX_WINDOW_PLACEMENT:
        {
            WINDOWPLACEMENT* psData =
                static_cast<WINDOWPLACEMENT*>(lpVoid);
            m_sTxWndPlacement = *psData;
            m_bIsConfigurationModified = TRUE;
        }
        break;

        default:
        {
            ASSERT(FALSE);
        }
        break;
    }

    return bRetVal;
}

BYTE* CTxWndDataStore::pbySetConfigData(BYTE* pbyConfigData, INT /*nConfigSize*/)
{
    BYTE* pbyTemp = pbyConfigData;
    vReleaseMultiMsgInfo(m_psMsgBlockList);
    m_psMsgBlockList = NULL;
    PSMSGBLOCKLIST psBlockTail = NULL;
    m_unNumberOfMsgBlockCount = 0;

    if (pbyTemp != NULL)
    {
        BYTE byVersion = 0;
        COPY_DATA_2(&byVersion, pbyTemp, sizeof(BYTE));
        COPY_DATA_2(&m_unNumberOfMsgBlockCount, pbyTemp, sizeof(UINT));

        for (UINT i = 0; i < m_unNumberOfMsgBlockCount; i++)
        {
            PSMSGBLOCKLIST psTempBlock = new SMSGBLOCKLIST;
            TCHAR acName[MAX_PATH] = {_T('\0')};
            COPY_DATA_2(acName, pbyTemp, (sizeof(TCHAR) * MAX_PATH));
            strcpy_s(psTempBlock->m_acStrBlockName, acName);
            COPY_DATA_2(&(psTempBlock->m_ucTrigger),pbyTemp,  sizeof(UCHAR));
            COPY_DATA_2(&(psTempBlock->m_bActive), pbyTemp, sizeof(BOOL));
            COPY_DATA_2(&(psTempBlock->m_ucKeyValue), pbyTemp, sizeof(UCHAR));
            COPY_DATA_2(&(psTempBlock->m_unTimeInterval), pbyTemp, sizeof(UINT));
            COPY_DATA_2(&(psTempBlock->m_bType), pbyTemp, sizeof(BOOL));
            COPY_DATA_2(&(psTempBlock->m_bTxAllFrame), pbyTemp, sizeof(BOOL));
            COPY_DATA_2(&(psTempBlock->m_unMsgCount), pbyTemp, sizeof(UINT));
            PSTXCANMSGLIST psTxCanMsgTail = NULL;

            for (UINT j = 0; j < psTempBlock->m_unMsgCount; j++)
            {
                PSTXCANMSGLIST psTempCanList = new STXCANMSGLIST;
                COPY_DATA_2(&(psTempCanList->m_sTxMsgDetails), pbyTemp, sizeof (STXCANMSGDETAILS));

                if (psTempBlock->m_psTxCANMsgList == NULL)//For the first time
                {
                    psTxCanMsgTail = psTempBlock->m_psTxCANMsgList = psTempCanList;
                }
                else
                {
                    psTxCanMsgTail->m_psNextMsgDetails = psTempCanList;
                    psTxCanMsgTail = psTempCanList;
                }
            }

            if (m_psMsgBlockList == NULL)//For the first time
            {
                psBlockTail = m_psMsgBlockList = psTempBlock;
            }
            else
            {
                psBlockTail->m_psNextMsgBlocksList = psTempBlock;
                psBlockTail = psTempBlock;
            }
        }

        //Get the Tx window placement
        WINDOWPLACEMENT WndPlacement;
        COPY_DATA_2(&WndPlacement, pbyTemp, sizeof (WINDOWPLACEMENT));
        bSetTxData(TX_WINDOW_PLACEMENT, &WndPlacement);
        //Get the Tx splitter position
        STXMSGSPLITTERDATA sTxSpliiterData;
        COPY_DATA_2(&sTxSpliiterData, pbyTemp,  sizeof(STXMSGSPLITTERDATA));
        bSetTxData(TX_WND_SPLITTER_DATA, &sTxSpliiterData);
    }

    return pbyTemp;
}

BYTE* CTxWndDataStore::pbyGetConfigData(BYTE*& pbyConfigData, INT& nConfigSize)
{
    BYTE* pbyCfgData = NULL;
    UINT unSize = 0;
    //FIRST CALCULATE THE SIZE
    unSize += sizeof(BYTE);// configuration version
    UINT unBlockCount = m_unNumberOfMsgBlockCount;
    unSize += sizeof (UINT); //block count
    PSMSGBLOCKLIST psTemp = m_psMsgBlockList;

    while (psTemp != NULL && unBlockCount > 0)
    {
        unSize += (sizeof(TCHAR) * MAX_PATH); // To store the block name
        unSize += sizeof(UCHAR); // To store the trigger
        unSize += sizeof(BOOL); // To store active or not
        unSize += sizeof(UCHAR); // To store the key value
        unSize += sizeof (UINT); // to store the timeinterval
        unSize += sizeof (BOOL); // Type
        unSize += sizeof (BOOL); // Send All msgs
        unSize += sizeof(UINT);// To store no of msgs in each block
        unSize += (psTemp->m_unMsgCount) * sizeof (STXCANMSGDETAILS);
        psTemp = psTemp->m_psNextMsgBlocksList;
    }

    unSize += sizeof(WINDOWPLACEMENT);
    unSize += sizeof(STXMSGSPLITTERDATA);
    //ALLOCATE THE MEMORY
    pbyCfgData = new BYTE[unSize];
    BYTE* pbyTemp = pbyCfgData;
    BYTE byVersion = 0x1;
    COPY_DATA(pbyTemp, &byVersion, sizeof(BYTE));
    COPY_DATA(pbyTemp, &unBlockCount, sizeof(UINT));
    PSMSGBLOCKLIST psTempBlock = m_psMsgBlockList;

    while (psTempBlock != NULL && unBlockCount > 0)
    {
        CString m_omStrBlockName;
        TCHAR acName[MAX_PATH] = {_T('\0')};
        strcpy_s(acName, psTempBlock->m_acStrBlockName);
        COPY_DATA(pbyTemp, acName, (sizeof(TCHAR) * MAX_PATH));
        COPY_DATA(pbyTemp, &(psTempBlock->m_ucTrigger), sizeof(UCHAR));
        COPY_DATA(pbyTemp, &(psTempBlock->m_bActive), sizeof(BOOL));
        COPY_DATA(pbyTemp, &(psTempBlock->m_ucKeyValue), sizeof(UCHAR));
        COPY_DATA(pbyTemp, &(psTempBlock->m_unTimeInterval), sizeof(UINT));
        COPY_DATA(pbyTemp, &(psTempBlock->m_bType), sizeof(BOOL));
        COPY_DATA(pbyTemp, &(psTempBlock->m_bTxAllFrame), sizeof(BOOL));
        COPY_DATA(pbyTemp, &(psTempBlock->m_unMsgCount), sizeof(UINT));
        PSTXCANMSGLIST psTempCanList = psTempBlock->m_psTxCANMsgList;

        while (psTempCanList != NULL)
        {
            COPY_DATA(pbyTemp, &(psTempCanList->m_sTxMsgDetails), sizeof (STXCANMSGDETAILS));
            psTempCanList = psTempCanList->m_psNextMsgDetails;
        }

        psTempBlock = psTempBlock->m_psNextMsgBlocksList;
    }

    //Get the Tx window placement
    COPY_DATA(pbyTemp, &m_sTxWndPlacement, sizeof(WINDOWPLACEMENT));
    //Get the Tx splitter position
    COPY_DATA(pbyTemp, &m_sTxMsgWndSplitterPos, sizeof(STXMSGSPLITTERDATA));
    //Update the OUT PARAMETERS
    pbyConfigData = pbyCfgData;
    nConfigSize = unSize;
    return pbyTemp;//return the latest pointer
}

/******************************************************************************/
/*  Function Name    :  bGetMultiMsgInfo                                      */
/*                                                                            */
/*  Input(s)         :  pSendMultMsgInfo : out-param structure                */
/*                                                                            */
/*  Output           :  TRUE : if the method succeeds                         */
/*                      FALSE : if memory allocation fails                    */
/*                                                                            */
/*  Functionality    :  This method gets the information for the multi message*/
/*                      information. The memory for the strucutre is already  */
/*                      allocated in the bGetData(..) method. This method     */
/*                      allocates memory as required by the other elements in */
/*                      this structure.                                       */
/*                                                                            */
/*  Member of        :  CTxWndDataStore                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications On :  05.06.2003, the name  an64SignalValues is changed to  */
/*                      __int64 data type.                                    */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications On :  24.12.2003, the structure for holding message details */
/*                      is changed. So new structure is adopted               */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications on :  08.01.2004, The interface for message block           */
/*                      is changed and a new flag is introduced to get the    */
/*                      message count.                                        */
/*  Modifications By :  Raja N                                                */
/*  Modifications on :  22.07.2004, The trigger value type got changed as char*/
/*                      and removed the trigger to def from enum              */
/******************************************************************************/
BOOL CTxWndDataStore::bGetMultiMsgInfo(PSMSGBLOCKLIST psDestMsgBlockList)
{
    BOOL bRetVal = TRUE;
    PSMSGBLOCKLIST psSrcMsgBlockList = m_psMsgBlockList;
    PSTXCANMSGLIST psSrcTxMsgList    = NULL;
    PSTXCANMSGLIST psDestTxMsgList   = NULL;

    if (psSrcMsgBlockList != NULL && psDestMsgBlockList != NULL)
    {
        for (UINT i =0; (i<m_unNumberOfMsgBlockCount) && (bRetVal == TRUE); i++)
        {
            psDestMsgBlockList->m_bActive        =
                psSrcMsgBlockList->m_bActive;
            psDestMsgBlockList->m_bType          = psSrcMsgBlockList->m_bType;
            psDestMsgBlockList->m_bTxAllFrame     =
                psSrcMsgBlockList->m_bTxAllFrame;
            psDestMsgBlockList->m_ucTrigger       =
                psSrcMsgBlockList->m_ucTrigger;
            psDestMsgBlockList->m_ucKeyValue     =
                psSrcMsgBlockList->m_ucKeyValue;
            psDestMsgBlockList->m_unMsgCount     =
                psSrcMsgBlockList->m_unMsgCount;
            psDestMsgBlockList->m_unTimeInterval =
                psSrcMsgBlockList->m_unTimeInterval;
            strcpy_s( psDestMsgBlockList->m_acStrBlockName,
                      psSrcMsgBlockList->m_acStrBlockName);

            if (psSrcMsgBlockList->m_unMsgCount > 0)
            {
                psSrcTxMsgList = psSrcMsgBlockList->m_psTxCANMsgList;
                psDestMsgBlockList->m_psTxCANMsgList = new STXCANMSGLIST;
                psDestTxMsgList = psDestMsgBlockList->m_psTxCANMsgList;

                if (psDestTxMsgList != NULL)
                {
                    for (UINT j =0;
                            (j<psSrcMsgBlockList->m_unMsgCount)&&(bRetVal == TRUE);
                            j++)
                    {
                        memcpy(&(psDestTxMsgList->m_sTxMsgDetails),
                               &(psSrcTxMsgList->m_sTxMsgDetails),
                               sizeof(psSrcTxMsgList->m_sTxMsgDetails));

                        if (psSrcTxMsgList->m_psNextMsgDetails != NULL &&
                                j+1 <psSrcMsgBlockList->m_unMsgCount)
                        {
                            psDestTxMsgList->m_psNextMsgDetails =
                                new STXCANMSGLIST;
                            vInitialiseMsgDetails(
                                psDestTxMsgList->m_psNextMsgDetails);

                            if (psDestTxMsgList->m_psNextMsgDetails != NULL)
                            {
                                psDestTxMsgList =
                                    psDestTxMsgList->m_psNextMsgDetails;
                                psSrcTxMsgList  =
                                    psSrcTxMsgList->m_psNextMsgDetails;
                            }
                            else
                            {
                                bRetVal = FALSE;
                            }
                        }
                        else
                        {
                            psDestTxMsgList->m_psNextMsgDetails = NULL;;
                        }
                    }
                }
                else
                {
                    bRetVal = FALSE;
                }
            }

            // Check the message count and don't allocate if the assignment for
            // last node is done i + 1 Because i is not incremented here

            if (psSrcMsgBlockList->m_psNextMsgBlocksList != NULL &&
                    (i + 1) < m_unNumberOfMsgBlockCount)
            {
                psDestMsgBlockList->m_psNextMsgBlocksList = new SMSGBLOCKLIST;
                vInitialiseMsgBlock(psDestMsgBlockList->m_psNextMsgBlocksList);

                if (psDestMsgBlockList->m_psNextMsgBlocksList != NULL)
                {
                    psDestMsgBlockList =
                        psDestMsgBlockList->m_psNextMsgBlocksList;
                    psDestMsgBlockList->m_psNextMsgBlocksList = NULL;
                    psDestMsgBlockList->m_psTxCANMsgList = NULL;
                    psSrcMsgBlockList =
                        psSrcMsgBlockList->m_psNextMsgBlocksList;
                }
                else
                {
                    bRetVal = FALSE;
                }
            }
            else
            {
                psDestMsgBlockList->m_psNextMsgBlocksList = NULL;
            }
        }
    }
    else
    {
        bRetVal = FALSE;
    }

    return bRetVal;
}
static void vCopyBlockDetails(PSMSGBLOCKLIST psDest, const PSMSGBLOCKLIST psSrc)
{
    ASSERT((psDest != NULL) && (psSrc != NULL));
    strcpy_s(psDest->m_acStrBlockName, psSrc->m_acStrBlockName);
    psDest->m_bActive              = psSrc->m_bActive;
    psDest->m_bTxAllFrame          = psSrc->m_bTxAllFrame;
    psDest->m_bType                = psSrc->m_bType;
    psDest->m_ucKeyValue           = psSrc->m_ucKeyValue;
    psDest->m_ucTrigger            = psSrc->m_ucTrigger;
    psDest->m_unMsgCount           = psSrc->m_unMsgCount;
    psDest->m_unTimeInterval       = psSrc->m_unTimeInterval;
    psDest->m_psNextMsgBlocksList = NULL;
    PSTXCANMSGLIST psSrcTemp = psSrc->m_psTxCANMsgList;

    while (psSrcTemp != NULL)
    {
        PSTXCANMSGLIST psDestTemp = new STXCANMSGLIST;
        psDestTemp->m_psNextMsgDetails = NULL;
        memcpy(&(psDestTemp->m_sTxMsgDetails), &(psSrcTemp->m_sTxMsgDetails),
               sizeof(STXCANMSGDETAILS));

        if (psDest->m_psTxCANMsgList == NULL)
        {
            psDest->m_psTxCANMsgList = psDestTemp;
        }
        else
        {
            psDestTemp->m_psNextMsgDetails = psDest->m_psTxCANMsgList;
            psDest->m_psTxCANMsgList = psDestTemp;
        }

        psSrcTemp = psSrcTemp->m_psNextMsgDetails;
    }
}

/******************************************************************************/
/*  Function Name    :  bSetMultiMsgInfo                                      */
/*                                                                            */
/*  Input(s)         :  pSendMultMsgInfo : in param structure                 */
/*                                                                            */
/*  Output           :  TRUE : if the method succeeds                         */
/*                      FALSE : if memory allocation fails                    */
/*                                                                            */
/*  Functionality    :  This method sets the information from the input       */
/*                      structure into the data member. This updates the      */
/*                      information.                                          */
/*                                                                            */
/*  Member of        :  CTxWndDataStore                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications On :  05.06.2003, the name  an64SignalValues is changed to  */
/*                      __int64 data type.                                    */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications on :  08.01.2004, The interface for message block           */
/*                      is changed and a new flag is introduced to get the    */
/*                      message count.                                        */
/*  Modofication By  :  Raja N                                                */
/*  Modifications on :  22.07.2004, The trigger value type got changed as char*/
/*                      and removed the trigger to def from enum              */
/******************************************************************************/
BOOL CTxWndDataStore::bSetMultiMsgInfo(PSMSGBLOCKLIST psMsgBlockList)
{
    BOOL bRetVal = TRUE;
    vReleaseMultiMsgInfo(m_psMsgBlockList);
    m_psMsgBlockList = NULL;
    PSMSGBLOCKLIST psTail = NULL;

    while (psMsgBlockList != NULL)
    {
        PSMSGBLOCKLIST psTemp = new SMSGBLOCKLIST;
        vCopyBlockDetails(psTemp, psMsgBlockList);
        psMsgBlockList = psMsgBlockList->m_psNextMsgBlocksList;

        if (m_psMsgBlockList == NULL)
        {
            psTail = m_psMsgBlockList =  psTemp;
        }
        else
        {
            psTail->m_psNextMsgBlocksList = psTemp;
            psTail = psTemp;
        }
    }

    /*PSMSGBLOCKLIST psSrcMsgBlockList = psMsgBlockList;
    PSMSGBLOCKLIST psDesMsgBlockList = m_psMsgBlockList;
    PSTXMSG psTxMsg = m_psTxMsgBlockList;
    if (psDesMsgBlockList == NULL)
    {
        m_psMsgBlockList = new SMSGBLOCKLIST;
        vInitialiseMsgBlock(m_psMsgBlockList);
        if (m_psMsgBlockList == NULL)
        {
            bRetVal = FALSE;
        }
        else
        {
            psDesMsgBlockList = m_psMsgBlockList;
            m_psMsgBlockList->m_psNextMsgBlocksList = NULL;
            m_psMsgBlockList->m_psTxCANMsgList = NULL;
        }
    }
    if (psSrcMsgBlockList != NULL &&
        psDesMsgBlockList != NULL &&
        bRetVal == TRUE)
    {
        do
        {
            psDesMsgBlockList->m_bActive        = psSrcMsgBlockList->m_bActive;
            psDesMsgBlockList->m_bType          = psSrcMsgBlockList->m_bType;
            psDesMsgBlockList->m_bTxAllFrame     =
                psSrcMsgBlockList->m_bTxAllFrame;
            psDesMsgBlockList->m_ucTrigger       =
                psSrcMsgBlockList->m_ucTrigger;

            psDesMsgBlockList->m_ucKeyValue  = psSrcMsgBlockList->m_ucKeyValue;
            psDesMsgBlockList->m_unMsgCount  = psSrcMsgBlockList->m_unMsgCount;

            strcpy( psDesMsgBlockList->m_acStrBlockName,
                psSrcMsgBlockList->m_acStrBlockName);

            psDesMsgBlockList->m_unTimeInterval =
                psSrcMsgBlockList->m_unTimeInterval;

            if (psSrcMsgBlockList->m_unMsgCount>0)
            {
                PSTXCANMSGLIST psSrcTxMsgList =
                    psSrcMsgBlockList->m_psTxCANMsgList;
                PSTXCANMSGLIST psDesTxMsgList = NULL;
                if (psDesMsgBlockList->m_psTxCANMsgList == NULL)
                {
                    psDesTxMsgList = new STXCANMSGLIST;
                    psDesMsgBlockList->m_psTxCANMsgList = psDesTxMsgList;
                    psDesTxMsgList->m_psNextMsgDetails  = NULL;
                }
                else
                {
                    psDesTxMsgList = psDesMsgBlockList->m_psTxCANMsgList;
                }
                if (psDesTxMsgList != NULL && psSrcTxMsgList != NULL)
                {
                    LPLONG lpPreviousCount = NULL;
                    BOOL   bTxON = CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG);
                    do
                    {
                        if (psTxMsg != NULL && bTxON == TRUE)
                        {
                            //EnterCriticalSection(
                            //          &psTxMsg->m_sMsgBlocksCriticalSection);
                            //Wait for twice to block both Key and Timer threads
                            WaitForSingleObject(psTxMsg->m_hSemaphore,
                                INFINITE);
                            WaitForSingleObject(psTxMsg->m_hSemaphore,
                                INFINITE);
                        }
                        psDesTxMsgList->m_sTxMsgDetails =
                            psSrcTxMsgList->m_sTxMsgDetails;
                        if (psTxMsg != NULL && bTxON == TRUE)
                        {
                            // LeaveCriticalSection(
                            //           &psTxMsg->m_sMsgBlocksCriticalSection);
                            //Release twice to block both Key and Timer threads
                            ReleaseSemaphore(psTxMsg->m_hSemaphore,
                                1,
                                lpPreviousCount);
                            ReleaseSemaphore(psTxMsg->m_hSemaphore,
                                1,
                                lpPreviousCount);
                        }
                        psSrcTxMsgList = psSrcTxMsgList->m_psNextMsgDetails;
                        if (psSrcTxMsgList != NULL)
                        {
                            if (psDesTxMsgList->m_psNextMsgDetails == NULL)
                            {
                                psDesTxMsgList->m_psNextMsgDetails =
                                    new STXCANMSGLIST;
                                if (psDesTxMsgList->m_psNextMsgDetails == NULL)
                                {
                                    bRetVal = FALSE;
                                }
                                else
                                {
                                    psDesTxMsgList =
                                        psDesTxMsgList->m_psNextMsgDetails;
                                    psDesTxMsgList->m_psNextMsgDetails = NULL;
                                }
                            }
                            else
                            {
                                psDesTxMsgList =
                                    psDesTxMsgList->m_psNextMsgDetails;
                            }
                        }
                    }while(psSrcTxMsgList!= NULL  && bRetVal == TRUE);
                    // Delete the extra element if user has reduced the size.
                    if (psDesTxMsgList->m_psNextMsgDetails != NULL)
                    {
                        PSTXCANMSGLIST psDelTxMsgList =
                            psDesTxMsgList->m_psNextMsgDetails;
                        psDesTxMsgList->m_psNextMsgDetails = NULL;
                        while(psDesTxMsgList != NULL && psDelTxMsgList != NULL)
                        {
                            psDesTxMsgList = psDelTxMsgList->m_psNextMsgDetails;
                            delete psDelTxMsgList ;
                            psDelTxMsgList  = NULL;
                            if (psDesTxMsgList != NULL)
                            {
                                psDelTxMsgList =
                                    psDesTxMsgList->m_psNextMsgDetails;
                            }
                        }
                    }
                }
                else
                {
                    bRetVal = FALSE;
                }
            }
            if (psSrcMsgBlockList->m_psNextMsgBlocksList != NULL)
            {
                if (psDesMsgBlockList->m_psNextMsgBlocksList == NULL)
                {
                    psDesMsgBlockList->m_psNextMsgBlocksList =
                        new SMSGBLOCKLIST;
                    vInitialiseMsgBlock(
                        psDesMsgBlockList->m_psNextMsgBlocksList);
                    if (psDesMsgBlockList->m_psNextMsgBlocksList == NULL)
                    {
                        bRetVal = FALSE;
                    }
                    else
                    {
                        psDesMsgBlockList =
                            psDesMsgBlockList->m_psNextMsgBlocksList;
                        psDesMsgBlockList->m_psTxCANMsgList = NULL;
                        psDesMsgBlockList->m_psNextMsgBlocksList = NULL;
                    }
                }
                else
                {
                    psDesMsgBlockList =
                        psDesMsgBlockList->m_psNextMsgBlocksList;
                }

            }
            psSrcMsgBlockList = psSrcMsgBlockList->m_psNextMsgBlocksList;
            if (psTxMsg != NULL)
            {
                psTxMsg = psTxMsg->m_psNextTxMsgInfo;
            }
        }while( psSrcMsgBlockList != NULL
            && bRetVal == TRUE);

    }
    else
    {
        bRetVal = FALSE;
    }*/
    return bRetVal;
}





/******************************************************************************/
/*  Function Name    :  vReleaseMultiMsgInfo                                  */
/*                                                                            */
/*  Input(s)         :                                                        */
/*                                                                            */
/*  Output           :                                                        */
/*                                                                            */
/*  Functionality    :  This method releases all the memory allocated for the */
/*                      send multiple message info structure.                 */
/*                                                                            */
/*  Member of        :  CTxWndDataStore                                        */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Gopi                                                  */
/*  Date Created     :  18.10.2002                                            */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications on :  08.01.2004, The interface for message block           */
/*                      is changed                                            */
/******************************************************************************/
void CTxWndDataStore::vReleaseMultiMsgInfo(PSMSGBLOCKLIST psMsgBlockList)
{
    PSMSGBLOCKLIST psNextMsgBlockList    = NULL;
    PSTXCANMSGLIST psNextTxCANMsgList    = NULL;
    PSMSGBLOCKLIST psCurrentMsgBlockList = NULL;
    PSTXCANMSGLIST psCurrentTxCANMsgList = NULL;

    if (psMsgBlockList != NULL)
    {
        psCurrentMsgBlockList = psMsgBlockList;

        do
        {
            if (psCurrentMsgBlockList->m_unMsgCount > 0)
            {
                psCurrentTxCANMsgList =
                    psCurrentMsgBlockList->m_psTxCANMsgList;

                do
                {
                    psNextTxCANMsgList    =
                        psCurrentTxCANMsgList->m_psNextMsgDetails;
                    delete psCurrentTxCANMsgList;
                    psCurrentTxCANMsgList = NULL;
                    psCurrentTxCANMsgList = psNextTxCANMsgList;
                }
                while(psNextTxCANMsgList != NULL);
            }

            psNextMsgBlockList = psCurrentMsgBlockList->m_psNextMsgBlocksList;
            delete psCurrentMsgBlockList;
            psCurrentMsgBlockList = NULL;
            psCurrentMsgBlockList = psNextMsgBlockList;
        }
        while(psNextMsgBlockList != NULL);
    }
}
void CTxWndDataStore::vInitialiseMsgDetails(PSTXCANMSGLIST& psMsgDetails)
{
    if (psMsgDetails != NULL)
    {
        psMsgDetails->m_psNextMsgDetails    = NULL;
    }
}
void CTxWndDataStore::vInitialiseMsgBlock(PSMSGBLOCKLIST& psMsgBlockList)
{
    if (psMsgBlockList != NULL)
    {
        psMsgBlockList->m_psNextMsgBlocksList = NULL;
        psMsgBlockList->m_unMsgCount          = 0;
        psMsgBlockList->m_bActive             = FALSE;
        psMsgBlockList->m_bType               = TRUE;
        psMsgBlockList->m_bTxAllFrame         = FALSE;
        psMsgBlockList->m_ucTrigger           = defTIME_TRIGGER;
        psMsgBlockList->m_psTxCANMsgList      = NULL;
        psMsgBlockList->m_ucKeyValue          = defDEFAULT_KEY_VAL;
        psMsgBlockList->m_unTimeInterval      = defDEFAULT_TIME_VAL;
        strcpy_s( psMsgBlockList->m_acStrBlockName, defDEFAULT_MSG_BLOCK_NAME);
    }
}


PSMSGBLOCKLIST CTxWndDataStore::psReturnMsgBlockPointer()
{
    return m_psMsgBlockList;
}

BOOL CTxWndDataStore::bGetDefaultTXSplitterPostion(CRect omWndSize,
        LPVOID* psSplitterData)
{
    BOOL bWindowFound = TRUE;
    // Init Tx Window postion
    // Cast the data to STXMSGSPLITTERDATA type
    STXMSGSPLITTERDATA* pData = static_cast<STXMSGSPLITTERDATA*>
                                (*psSplitterData);

    // Init Graph window splitter postion
    if (pData != NULL)
    {
        // Root Splitter information
        pData->m_nRootSplitterData[0][0] =
            (int)(omWndSize.Height() * defTX_WND_ROOT_SPLITTER_RATIO);
        pData->m_nRootSplitterData[0][1] = 0;
        pData->m_nRootSplitterData[1][0] =
            (int)(omWndSize.Height() * ( 1 - defTX_WND_ROOT_SPLITTER_RATIO));
        pData->m_nRootSplitterData[1][1] = 0;
        // Left splitter information
        pData->m_nLeftViewSplitterData[0][0] =
            (int)(omWndSize.Width() * defTX_WND_LEFT_SPLITTER_RATIO);
        pData->m_nLeftViewSplitterData[0][1] = 0;
        pData->m_nLeftViewSplitterData[1][0] =
            (int)(omWndSize.Width() * (1 - defTX_WND_LEFT_SPLITTER_RATIO));
        pData->m_nLeftViewSplitterData[1][1] = 0;
        // Take Right view height as size
        pData->m_nRightViewSplitterData[0][0] =
            (int)(pData->m_nRootSplitterData[0][0] *
                  defTX_WND_RIGHT_SPLITTER_RATIO);
        pData->m_nRightViewSplitterData[0][1] = 0;
        pData->m_nRightViewSplitterData[1][0] =
            (int)(pData->m_nRootSplitterData[0][0] *
                  ( 1- defTX_WND_RIGHT_SPLITTER_RATIO));
        pData->m_nRightViewSplitterData[1][1] = 0;
    }
    else
    {
        // Set the result to false
        bWindowFound = FALSE;
    }

    return bWindowFound;
}