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
#include "include/XMLDefines.h"
#include "include/XMLDefines.h"
#include "Utility/XMLUtils.h"
#include "Utility\MultiLanguageSupport.h"
//#include "../Application/GettextBusmaster.h"

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
    m_bAutoSavedEnabled = false;
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
        case TX_AUTO_UPDATE_ENABLE:
        {
            bool* psData = static_cast<bool*>(*lpData);
            *psData = m_bAutoSavedEnabled;
            break;
        }
        case TX_DELAY_BTWN_MSG_BLCK_BOOL:
        {
            bool* psData = static_cast<bool*>(*lpData);
            *psData = m_bDelayBetweenMsgBlocks;
            break;
        }
        case TX_DELAY_BTWN_MSG_BLCK_UINT:
        {
            UINT* psData = static_cast<UINT*>(*lpData);
            *psData = m_unTimeDelayBtwnMsgBlocks;
            break;
        }
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
        case TX_AUTO_UPDATE_ENABLE:
        {
            bool* psData = static_cast<bool*>(lpVoid);
            m_bAutoSavedEnabled = *psData;
            m_bIsConfigurationModified = TRUE;
            break;
        }
        case TX_DELAY_BTWN_MSG_BLCK_BOOL:
        {
            bool* psData = static_cast<bool*>(lpVoid);
            m_bDelayBetweenMsgBlocks = *psData;
            m_bIsConfigurationModified = TRUE;
            break;
        }
        case TX_DELAY_BTWN_MSG_BLCK_UINT:
        {
            UINT* psData = static_cast<UINT*>(lpVoid);
            m_unTimeDelayBtwnMsgBlocks = *psData;
            m_unTimeDelayBtwnMsgBlocks = TRUE;
            break;
        }
        case TX_MSG_UPDATE:
        {
            PSMSGBLOCKLIST psMsgBlockInfo =
                static_cast<PSMSGBLOCKLIST>(lpVoid);
            bSetDataToGlobal(psMsgBlockInfo);
        }
        default:
        {
            ASSERT(FALSE);
        }
        break;
    }
    return bRetVal;
}
bool CTxWndDataStore::bSetDataToGlobal(PSMSGBLOCKLIST psMsgBlockList )
{
    if (psMsgBlockList == NULL)
    {
        return false;
    }

    int nIndex = psMsgBlockList->m_unIndex;
    PSMSGBLOCKLIST psGlobalBlockList        = NULL;
    PSMSGBLOCKLIST psPrevMsgBlockList       = NULL;
    if(m_psMsgBlockList == NULL)            //if first message m_psMsgBlockList will be NULL
    {
        m_psMsgBlockList = new SMSGBLOCKLIST;
        psGlobalBlockList = m_psMsgBlockList;
    }
    if (psMsgBlockList != NULL)
    {
        psGlobalBlockList = m_psMsgBlockList;
        while((psGlobalBlockList != NULL) && (psMsgBlockList != NULL))
        {
            //if theindex are not same then a message is delete, so update the entire linked list
            if(psMsgBlockList->m_unIndex  != psGlobalBlockList->m_unIndex)
            {
                bCopyBlockData(psGlobalBlockList, psMsgBlockList);
            }
            else
            {
                //if(psMsgBlockList ->m_bModified)
                {
                    bCopyBlockData(psGlobalBlockList, psMsgBlockList);
                }
            }
            psMsgBlockList->m_bModified = false;
            psMsgBlockList      = psMsgBlockList->m_psNextMsgBlocksList;
            if(psMsgBlockList == NULL || psGlobalBlockList->m_psNextMsgBlocksList == NULL )
            {
                //store the pointer for later use, we have to make the last pointer NULL
                psPrevMsgBlockList  = psGlobalBlockList;
            }
            psGlobalBlockList   = psGlobalBlockList->m_psNextMsgBlocksList;
        }
    }

    while(psGlobalBlockList != NULL)    //messages from the end of Global list have to be deleted since the are delete by user
    {
        if(psPrevMsgBlockList)
        {
            psPrevMsgBlockList->m_psNextMsgBlocksList = NULL;    //set the last node's nxt node as NULL
        }
        while(psGlobalBlockList)
        {
            psPrevMsgBlockList = psGlobalBlockList;         //save the current node
            psGlobalBlockList = psPrevMsgBlockList->m_psNextMsgBlocksList; //get the nxt node
            bDeleteMsgList(psPrevMsgBlockList->m_psTxCANMsgList);
            delete psPrevMsgBlockList;                      //delete the current node
            psPrevMsgBlockList = NULL;
        }
    }
    while(psMsgBlockList != NULL) //if new block are added update global
    {
        psGlobalBlockList = new     SMSGBLOCKLIST;
        bCopyBlockData(psGlobalBlockList,psMsgBlockList);
        psPrevMsgBlockList->m_psNextMsgBlocksList = psGlobalBlockList;
        psMsgBlockList = psMsgBlockList->m_psNextMsgBlocksList;
        psGlobalBlockList->m_psNextMsgBlocksList = NULL;
        psPrevMsgBlockList = psGlobalBlockList;
    }
    return true;
}

/******************************************************************************/
/*  Function Name    :  CompareBlockLists                                     */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will compare 2 BlockList                */
/*                                                                            */
/*  Member of        :  CTxMsgManager                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Ashwin R Uchil                                        */
/*  Date Created     :  02.01.2013                                            */
/******************************************************************************/
int CTxWndDataStore::nCompareBlockLists(PSMSGBLOCKLIST psMsgBlockSrc)
{
    PSMSGBLOCKLIST psMsgBlockDest = NULL;
    psMsgBlockDest = m_psMsgBlockList;

    if((psMsgBlockSrc == NULL)&&(psMsgBlockDest == NULL))
    {
        return FALSE;
    }

    while(psMsgBlockSrc != NULL && psMsgBlockDest != NULL )
    {
        if(strcmp(psMsgBlockSrc->m_acStrBlockName,psMsgBlockDest->m_acStrBlockName) != 0)
        {
            return false;
        }
        if((psMsgBlockSrc->m_bActive != psMsgBlockDest->m_bActive)|| (psMsgBlockSrc->m_bTxAllFrame != psMsgBlockDest->m_bTxAllFrame)||
                (psMsgBlockSrc->m_bType != psMsgBlockDest->m_bType))
        {
            return false;
        }
        if((psMsgBlockSrc->m_ucKeyValue != psMsgBlockDest->m_ucKeyValue)|| (psMsgBlockSrc->m_ucTrigger != psMsgBlockDest->m_ucTrigger)||
                (psMsgBlockSrc->m_unIndex != psMsgBlockDest->m_unIndex)||(psMsgBlockSrc->m_unMsgCount != psMsgBlockDest->m_unMsgCount) ||
                (psMsgBlockSrc->m_unTimeInterval != psMsgBlockDest->m_unTimeInterval))
        {
            return false;
        }
		if((psMsgBlockSrc->m_psTxCANMsgList == NULL) && (psMsgBlockDest->m_psTxCANMsgList == NULL))
		{
			psMsgBlockSrc = psMsgBlockSrc->m_psNextMsgBlocksList;
			psMsgBlockDest = psMsgBlockDest->m_psNextMsgBlocksList;
			continue;
		}
        if(nCompareMsgList(psMsgBlockSrc->m_psTxCANMsgList,psMsgBlockDest->m_psTxCANMsgList) == false)
        {
            return false;
        }

        psMsgBlockSrc = psMsgBlockSrc->m_psNextMsgBlocksList;
        psMsgBlockDest = psMsgBlockDest->m_psNextMsgBlocksList;
    }

    if((psMsgBlockSrc != NULL)|| psMsgBlockDest != NULL)
    {
        return false;
    }

    return true;
}

int CTxWndDataStore::nCompareMsgList(PSTXCANMSGLIST TxCanMsgListSrc,PSTXCANMSGLIST TxCanMsgListDest)
{
    if((TxCanMsgListSrc == NULL)|| (TxCanMsgListDest == NULL))
    {
        return FALSE;
    }
    while(TxCanMsgListSrc != NULL && TxCanMsgListDest != NULL )
    {
        /*if(TxCanMsgListSrc->m_unIndex != TxCanMsgListDest->m_unIndex)
        {
            return false;
        }*/
        if((TxCanMsgListSrc->m_sTxMsgDetails.m_bEnabled != TxCanMsgListDest->m_sTxMsgDetails.m_bEnabled)||
                (TxCanMsgListSrc->m_sTxMsgDetails.m_bIsMsgDirty != TxCanMsgListDest->m_sTxMsgDetails.m_bIsMsgDirty))
        {
            return false;
        }
        if(TxCanMsgListSrc->m_sTxMsgDetails.m_sTxMsg.m_ucChannel != TxCanMsgListDest->m_sTxMsgDetails.m_sTxMsg.m_ucChannel)
        {
            return false;
        }
        if(TxCanMsgListSrc->m_sTxMsgDetails.m_sTxMsg.m_ucData[0] != TxCanMsgListDest->m_sTxMsgDetails.m_sTxMsg.m_ucData[0]||
                TxCanMsgListSrc->m_sTxMsgDetails.m_sTxMsg.m_ucData[1] != TxCanMsgListDest->m_sTxMsgDetails.m_sTxMsg.m_ucData[1]||
                TxCanMsgListSrc->m_sTxMsgDetails.m_sTxMsg.m_ucData[2] != TxCanMsgListDest->m_sTxMsgDetails.m_sTxMsg.m_ucData[2]||
                TxCanMsgListSrc->m_sTxMsgDetails.m_sTxMsg.m_ucData[3] != TxCanMsgListDest->m_sTxMsgDetails.m_sTxMsg.m_ucData[3]||
                TxCanMsgListSrc->m_sTxMsgDetails.m_sTxMsg.m_ucData[4] != TxCanMsgListDest->m_sTxMsgDetails.m_sTxMsg.m_ucData[4]||
                TxCanMsgListSrc->m_sTxMsgDetails.m_sTxMsg.m_ucData[5] != TxCanMsgListDest->m_sTxMsgDetails.m_sTxMsg.m_ucData[5]||
                TxCanMsgListSrc->m_sTxMsgDetails.m_sTxMsg.m_ucData[6] != TxCanMsgListDest->m_sTxMsgDetails.m_sTxMsg.m_ucData[6]||
                TxCanMsgListSrc->m_sTxMsgDetails.m_sTxMsg.m_ucData[7] != TxCanMsgListDest->m_sTxMsgDetails.m_sTxMsg.m_ucData[7])
        {
            return false;
        }
        if(TxCanMsgListSrc->m_sTxMsgDetails.m_sTxMsg.m_ucDataLen != TxCanMsgListDest->m_sTxMsgDetails.m_sTxMsg.m_ucDataLen)
        {
            return false;
        }
        if(TxCanMsgListSrc->m_sTxMsgDetails.m_sTxMsg.m_ucEXTENDED != TxCanMsgListDest->m_sTxMsgDetails.m_sTxMsg.m_ucEXTENDED)
        {
            return false;
        }
        if(TxCanMsgListSrc->m_sTxMsgDetails.m_sTxMsg.m_ucRTR != TxCanMsgListDest->m_sTxMsgDetails.m_sTxMsg.m_ucRTR)
        {
            return false;
        }
        if(TxCanMsgListSrc->m_sTxMsgDetails.m_sTxMsg.m_unMsgID != TxCanMsgListDest->m_sTxMsgDetails.m_sTxMsg.m_unMsgID)
        {
            return false;
        }

        TxCanMsgListSrc = TxCanMsgListSrc->m_psNextMsgDetails;
        TxCanMsgListDest = TxCanMsgListDest->m_psNextMsgDetails;
    }
    return true;
}

bool CTxWndDataStore::bDeleteMsgList(PSTXCANMSGLIST& psMsgList)
{
    PSTXCANMSGLIST      psPrevMsgList = NULL;
    while(psMsgList)
    {
        psPrevMsgList = psMsgList;
        psMsgList = psMsgList->m_psNextMsgDetails;
        delete psPrevMsgList;
        psPrevMsgList = NULL;
    }
    return true;
}
/******************************************************************************
    Function Name    :  bCopyBlockData

    Input(s)         :  PSMSGBLOCKLIST, PSMSGBLOCKLIST
    Output           :  bool
    Functionality    :  Copies the PSMSGBLOCKLIST from source to destination
    Member of        :  CTxWndDataStore
    Friend of        :      -

    Author(s)        :  Ashwin R Uchil
    Date Created     :  22.8.2012
    Modifications    :
******************************************************************************/
bool CTxWndDataStore::bCopyBlockData( PSMSGBLOCKLIST psDestBlockList,  PSMSGBLOCKLIST psSrcBlockList)
{
    strcpy_s(psDestBlockList->m_acStrBlockName, defBLOCKNAME_SIZE, psSrcBlockList->m_acStrBlockName);
    psDestBlockList->m_bActive              = psSrcBlockList->m_bActive;
    psDestBlockList->m_bTxAllFrame          = psSrcBlockList->m_bTxAllFrame;
    psDestBlockList->m_bType                = psSrcBlockList->m_bType;
    psDestBlockList->m_ucKeyValue           = psSrcBlockList->m_ucKeyValue;
    psDestBlockList->m_ucTrigger            = psSrcBlockList->m_ucTrigger;
    psDestBlockList->m_unMsgCount           = psSrcBlockList->m_unMsgCount;
    psDestBlockList->m_unTimeInterval       = psSrcBlockList->m_unTimeInterval;
    psDestBlockList->m_unIndex              = psSrcBlockList->m_unIndex;
    //if destination block did not have any message list, then add a new one to the destination
    if(psDestBlockList->m_psTxCANMsgList == NULL && psSrcBlockList->m_psTxCANMsgList != NULL)
    {
        psDestBlockList->m_psTxCANMsgList = new STXCANMSGLIST;
    }
    //critical section is used so that source is not changed currently in  TxWindow
    EnterCriticalSection(&CTxMsgManager::m_csUpdationLock);
    bCopyMsgList(psDestBlockList->m_psTxCANMsgList, &psSrcBlockList->m_psTxCANMsgList);
    LeaveCriticalSection(&CTxMsgManager::m_csUpdationLock);
    if(psSrcBlockList->m_unMsgCount == 0)
    {
        psDestBlockList->m_psTxCANMsgList = NULL;    //this will not be set in the above function
    }
    return true;
}

/******************************************************************************
    Function Name    :  bCopyMsgList

    Input(s)         :  PSTXCANMSGLIST&, PSTXCANMSGLIST*
    Output           :  bool
    Functionality    :  Copies the CanMsgList from source to destination
    Member of        :  CTxWndDataStore
    Friend of        :      -

    Author(s)        :  Ashwin R Uchil
    Date Created     :  22.8.2012
    Modifications    :
******************************************************************************/
bool CTxWndDataStore::bCopyMsgList(PSTXCANMSGLIST&  ppsDestTxCANMsgList , PSTXCANMSGLIST*  ppsSrcTxCANMsgList)
{
    //pointer to pointer is used so that, if its the pointer that is stored in the block and all the messages are deleted,
    //then the block will point to a junk value and we will not be able to send the new pointer to the block
    PSTXCANMSGLIST psSrcTxCANMsgList = *ppsSrcTxCANMsgList;         //pointer to pointer
    PSTXCANMSGLIST psDestTxCANMsgList = ppsDestTxCANMsgList;        //pointer to pointer
    PSTXCANMSGLIST psPrevTxCANMsgList       = NULL;
    psPrevTxCANMsgList = NULL;
    //copy the contents directly from the src msg  list
    while((psSrcTxCANMsgList != NULL)&& (psDestTxCANMsgList != NULL))
    {
        memcpy(&(psDestTxCANMsgList->m_sTxMsgDetails), &(psSrcTxCANMsgList->m_sTxMsgDetails),
               sizeof(STXCANMSGDETAILS));
        psDestTxCANMsgList->m_unIndex = psSrcTxCANMsgList->m_unIndex;
        psSrcTxCANMsgList->m_bModified = false;
        psSrcTxCANMsgList     = psSrcTxCANMsgList->m_psNextMsgDetails;
        //store the previous message so that we can provide the link to the next message
        if(psSrcTxCANMsgList != NULL || psDestTxCANMsgList->m_psNextMsgDetails != NULL)
        {
            psPrevTxCANMsgList      = psDestTxCANMsgList;
        }
        psDestTxCANMsgList    = psDestTxCANMsgList->m_psNextMsgDetails;
    }
    if(psDestTxCANMsgList != NULL)    //if any message is delete then delete the extra messages
    {
        if(psPrevTxCANMsgList)
        {
            psPrevTxCANMsgList->m_psNextMsgDetails = NULL;
        }
        bDeleteMsgList(psDestTxCANMsgList);
    }
    while(psSrcTxCANMsgList != NULL)      //if new messages are added update global
    {
        psDestTxCANMsgList = new  STXCANMSGLIST;
        memcpy(&(psDestTxCANMsgList->m_sTxMsgDetails), &(psSrcTxCANMsgList->m_sTxMsgDetails),
               sizeof(STXCANMSGDETAILS));
        psDestTxCANMsgList->m_unIndex = psSrcTxCANMsgList->m_unIndex;
        psPrevTxCANMsgList->m_psNextMsgDetails= psDestTxCANMsgList;
        psSrcTxCANMsgList = psSrcTxCANMsgList->m_psNextMsgDetails;
        psDestTxCANMsgList->m_psNextMsgDetails = NULL;
        psPrevTxCANMsgList = psDestTxCANMsgList;
    }
    return true;
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
        int nVersion = (int)byVersion;

        COPY_DATA_2(&m_unNumberOfMsgBlockCount, pbyTemp, sizeof(UINT));


        for (UINT i = 0; i < m_unNumberOfMsgBlockCount; i++)
        {
            PSMSGBLOCKLIST psTempBlock = new SMSGBLOCKLIST;
            char acName[MAX_PATH] = {_T('\0')};
            COPY_DATA_2(acName, pbyTemp, (sizeof(char) * MAX_PATH));
            strcpy_s(psTempBlock->m_acStrBlockName, defBLOCKNAME_SIZE, acName);
            COPY_DATA_2(&(psTempBlock->m_ucTrigger),pbyTemp,  sizeof(UCHAR));
            COPY_DATA_2(&(psTempBlock->m_bActive), pbyTemp, sizeof(BOOL));
            COPY_DATA_2(&(psTempBlock->m_ucKeyValue), pbyTemp, sizeof(UCHAR));
            COPY_DATA_2(&(psTempBlock->m_unTimeInterval), pbyTemp, sizeof(UINT));
            COPY_DATA_2(&(psTempBlock->m_bType), pbyTemp, sizeof(BOOL));
            COPY_DATA_2(&(psTempBlock->m_bTxAllFrame), pbyTemp, sizeof(BOOL));

            COPY_DATA_2(&(psTempBlock->m_unMsgCount), pbyTemp, sizeof(UINT));
            if(nVersion <3)
            {
                psTempBlock->m_unIndex = i;
            }
            else
            {
                COPY_DATA_2(&(psTempBlock->m_unIndex), pbyTemp, sizeof(UINT));
            }
            PSTXCANMSGLIST psTxCanMsgTail = NULL;
            for (UINT j = 0; j < psTempBlock->m_unMsgCount; j++)
            {
                PSTXCANMSGLIST psTempCanList = new STXCANMSGLIST;
                int nOldMsgStructSize = sizeof (STXCANMSGDETAILS) /*- sizeof(psTempCanList->m_sTxMsgDetails.m_sTxMsg.m_aucCANFDData)*/;
                COPY_DATA_2(&(psTempCanList->m_sTxMsgDetails), pbyTemp, nOldMsgStructSize);
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

        if(nVersion >= 2)
        {
            COPY_DATA_2(&m_bAutoSavedEnabled, pbyTemp,  sizeof(bool));
            bSetTxData(TX_AUTO_UPDATE_ENABLE, &m_bAutoSavedEnabled);        //save the status of auto save
            //COPY_DATA_2(&m_bDelayBetweenMsgBlocks, pbyTemp,  sizeof(bool));
            //bSetTxData(TX_DELAY_BTWN_MSG_BLCK_BOOL, &m_bDelayBetweenMsgBlocks);     //save the delay between msg
            //COPY_DATA_2(&m_unTimeDelayBtwnMsgBlocks, pbyTemp,  sizeof(UINT));
            //bSetTxData(TX_DELAY_BTWN_MSG_BLCK_UINT, &m_unTimeDelayBtwnMsgBlocks);       //save the delay between msg
            //CTxMsgManager::s_bDelayBetweenBlocksOnly = m_bDelayBetweenMsgBlocks;
            //CTxMsgManager::s_unTimeDelayBtnMsgBlocks = m_unTimeDelayBtwnMsgBlocks;
        }
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
        unSize += (sizeof(char) * MAX_PATH); // To store the block name
        unSize += sizeof(UCHAR); // To store the trigger
        unSize += sizeof(BOOL); // To store active or not
        unSize += sizeof(UCHAR); // To store the key value
        unSize += sizeof (UINT); // to store the timeinterval
        unSize += sizeof (BOOL); // Type
        unSize += sizeof (BOOL); // Send All msgs

        unSize += sizeof(UINT);// To store no of msgs in each block
        unSize += sizeof(UINT); //index
        unSize += (psTemp->m_unMsgCount) * sizeof (STXCANMSGDETAILS);
        psTemp = psTemp->m_psNextMsgBlocksList;
    }

    unSize += sizeof(WINDOWPLACEMENT);
    unSize += sizeof(STXMSGSPLITTERDATA);
    unSize += sizeof(bool);                 //allocation for auto update
    unSize += sizeof(bool);                 //allocation for delay between msg
    unSize += sizeof(UINT);                 //allocation for delay between msg

    //ALLOCATE THE MEMORY
    pbyCfgData = new BYTE[unSize];
    BYTE* pbyTemp = pbyCfgData;

    BYTE byVersion = defTX_MSG_WND_VERSION;
    COPY_DATA(pbyTemp, &byVersion, sizeof(BYTE));
    COPY_DATA(pbyTemp, &unBlockCount, sizeof(UINT));

    PSMSGBLOCKLIST psTempBlock = m_psMsgBlockList;
    while (psTempBlock != NULL && unBlockCount > 0)
    {
        CString m_omStrBlockName;
        char acName[MAX_PATH] = {_T('\0')};
        strcpy_s(acName, MAX_PATH, psTempBlock->m_acStrBlockName);
        COPY_DATA(pbyTemp, acName, (sizeof(char) * MAX_PATH));
        COPY_DATA(pbyTemp, &(psTempBlock->m_ucTrigger), sizeof(UCHAR));
        COPY_DATA(pbyTemp, &(psTempBlock->m_bActive), sizeof(BOOL));
        COPY_DATA(pbyTemp, &(psTempBlock->m_ucKeyValue), sizeof(UCHAR));
        COPY_DATA(pbyTemp, &(psTempBlock->m_unTimeInterval), sizeof(UINT));
        COPY_DATA(pbyTemp, &(psTempBlock->m_bType), sizeof(BOOL));
        COPY_DATA(pbyTemp, &(psTempBlock->m_bTxAllFrame), sizeof(BOOL));

        COPY_DATA(pbyTemp, &(psTempBlock->m_unMsgCount), sizeof(UINT));

        COPY_DATA(pbyTemp, &(psTempBlock->m_unIndex), sizeof(UINT));
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

    //Get the Auto save option
    COPY_DATA(pbyTemp, &m_bAutoSavedEnabled, sizeof(bool));

    COPY_DATA(pbyTemp, &m_bDelayBetweenMsgBlocks, sizeof(bool));
    COPY_DATA(pbyTemp, &m_unTimeDelayBtwnMsgBlocks, sizeof(UINT));
    //Update the OUT PARAMETERS
    pbyConfigData = pbyCfgData;
    nConfigSize = unSize;

    return pbyTemp;//return the latest pointer
}

bool CTxWndDataStore::pbySetConfigData(xmlDocPtr pDoc)
{
    vReleaseMultiMsgInfo(m_psMsgBlockList);
    m_psMsgBlockList = NULL;
    PSMSGBLOCKLIST psBlockTail = NULL;
    m_unNumberOfMsgBlockCount = 0;

    if(pDoc == NULL)
    {
        m_sTxWndPlacement.rcNormalPosition.top = -1;
        m_sTxWndPlacement.length = 0;
        return false;
    }
    xmlChar* pXpath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Tx_Window";
    xmlXPathObjectPtr pObjectPath = xmlUtils::pGetNodes(pDoc, pXpath);
    xmlNodePtr pNode = NULL;
    xmlNodePtr pChildNode = NULL;
    xmlNodePtr pMsgNode = NULL;
    xmlNodePtr pDataBytesNode = NULL;
    PSTXCANMSGLIST psTxCanMsgTail = NULL;
    string   strVar;
    if( NULL != pObjectPath )
    {
        xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
        if( NULL != pNodeSet )
        {
            pNode = pNodeSet->nodeTab[0];       //Take First One only
        }
        if( NULL != pNode )
        {
            pNode = pNode->xmlChildrenNode;
            while (pNode != NULL)           //loop through the node of "CAN_Tx_Window"
            {
                if ((!xmlStrcmp(pNode->name, (const xmlChar*)DEF_MSG_BLOCK)))               //check whether block exists
                {
                    PSMSGBLOCKLIST psTempBlock = new SMSGBLOCKLIST;
                    pChildNode = pNode->xmlChildrenNode;
                    m_unNumberOfMsgBlockCount++;
                    int     nNoOfMess = 0;
                    while(pChildNode)       //loop through the node of "Message_Block"
                    {
                        if (xmlUtils::GetDataFrmNode(pChildNode,DEF_BLOCK_NAME,strVar))
                        {
                            strcpy(psTempBlock->m_acStrBlockName,strVar.c_str());               //copy block name
                        }
                        if (xmlUtils::GetDataFrmNode(pChildNode,DEF_MSG_ENABLED,strVar))
                        {
                            psTempBlock->m_bActive = (bool)atoi(strVar.c_str());                //copy Is enabled
                        }
                        if (xmlUtils::GetDataFrmNode(pChildNode,DEF_MSG_TRIGGER,strVar))
                        {
                            psTempBlock->m_ucTrigger = (UCHAR)atoi( strVar.c_str());            //Trigger value
                        }
                        if (xmlUtils::GetDataFrmNode(pChildNode,DEF_KEY_VAL,strVar))
                        {
                            if(strVar.length() > 0)
                            {
                                psTempBlock->m_ucKeyValue = strVar.at(0);
                            }
                        }
                        if (xmlUtils::GetDataFrmNode(pChildNode,DEF_INDEX,strVar))
                        {
                            psTempBlock->m_unIndex = (UINT)atoi(strVar.c_str());                    //Index
                        }
                        if (xmlUtils::GetDataFrmNode(pChildNode,DEF_TIME_INTERVAL,strVar))
                        {
                            psTempBlock->m_unTimeInterval = (UINT)atoi(strVar.c_str());             //Time interval
                        }
                        if (xmlUtils::GetDataFrmNode(pChildNode,DEF_MSG_IS_CYCLIC,strVar))
                        {
                            psTempBlock->m_bType = (bool)atoi(strVar.c_str());                      //Is cyclic
                        }
                        if (xmlUtils::GetDataFrmNode(pChildNode,DEF_TRANSMIT_ALL_FRAMES,strVar))
                        {
                            psTempBlock->m_bTxAllFrame = (bool)atoi(strVar.c_str());                //Transmit all frames
                        }
                        if ((!xmlStrcmp(pChildNode->name, (const xmlChar*)DEF_MSGS)))
                        {
                            pMsgNode = pChildNode->xmlChildrenNode;
                            PSTXCANMSGLIST psTempCanList = new STXCANMSGLIST;
                            STCAN_MSG*   pStcanMsg = &psTempCanList->m_sTxMsgDetails.m_sTxMsg;
                            nNoOfMess++;
                            while(pMsgNode)
                            {

                                if(pStcanMsg)
                                {
                                    if (xmlUtils::GetDataFrmNode(pMsgNode,DEF_MSG_ID,strVar))
                                    {
                                        pStcanMsg->m_unMsgID =  (UINT)atoi(strVar.c_str());                 //Msg ID
                                    }
                                    if (xmlUtils::GetDataFrmNode(pMsgNode,DEF_EXTENDED,strVar))             //Is Extended
                                    {
                                        if(strVar == "TRUE")
                                        {
                                            pStcanMsg->m_ucEXTENDED = 1;
                                        }
                                        else
                                        {
                                            pStcanMsg->m_ucEXTENDED = 0;
                                        }
                                    }
                                    if (xmlUtils::GetDataFrmNode(pMsgNode,DEF_RTR,strVar))
                                    {
                                        pStcanMsg->m_ucRTR =  (UCHAR)atoi(strVar.c_str());      //Is RTR
                                    }
                                    if (xmlUtils::GetDataFrmNode(pMsgNode,DEF_DLC,strVar))
                                    {
                                        pStcanMsg->m_ucDataLen =  (UCHAR)atoi(strVar.c_str());          //DLC
                                    }

                                    if (xmlUtils::GetDataFrmNode(pMsgNode,DEF_CHANNEL,strVar))
                                    {
                                        pStcanMsg->m_ucChannel =  (UCHAR)atoi(strVar.c_str());          //Channel
                                    }
                                    if ((!xmlStrcmp(pMsgNode->name, (const xmlChar*)DEF_DATABYTES)))
                                    {
                                        pDataBytesNode = pMsgNode->xmlChildrenNode;
                                        int iCnt =0;
                                        while(pDataBytesNode)
                                        {
                                            if (xmlUtils::GetDataFrmNode(pDataBytesNode,DEF_BYTE,strVar))
                                            {
                                                pStcanMsg->m_ucData[iCnt] = (UCHAR)atoi(strVar.c_str());    //Channel
                                                iCnt++;
                                            }
                                            pDataBytesNode = pDataBytesNode->next;
                                        }
                                    }

                                }
                                if (xmlUtils::GetDataFrmNode(pMsgNode,DEF_MSG_ENABLED,strVar))
                                {
                                    psTempCanList->m_sTxMsgDetails.m_bEnabled =  (bool)atoi(strVar.c_str());    //enabled
                                    psTempCanList->m_sTxMsgDetails.m_bIsMsgDirty = true;
                                }
                                if (xmlUtils::GetDataFrmNode(pMsgNode,DEF_INDEX,strVar))
                                {
                                    psTempCanList->m_unIndex =  (UINT)atoi(strVar.c_str());             //Index
                                }
                                pMsgNode = pMsgNode->next;          //incerment messages
                            }

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
                        pChildNode = pChildNode->next;              //increment mesage block
                    }
                    psTempBlock->m_unMsgCount = nNoOfMess;
                    nNoOfMess = 0;
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

                if(!xmlStrcmp(pNode->name, (const xmlChar*)DEF_WND_POS))
                {
                    WINDOWPLACEMENT WndPlacement;
                    xmlUtils::ParseWindowsPlacement(pNode,WndPlacement);
                    bSetTxData(TX_WINDOW_PLACEMENT, &WndPlacement);
                }
                if(xmlUtils::GetDataFrmNode(pNode,DEF_AUTOSAVE,strVar))
                {
                    m_bAutoSavedEnabled =  (bool)atoi(strVar.c_str());                  //Channel
                }

                if(xmlUtils::GetDataFrmNode(pNode,DEF_BLOCKS_TIME_ENABLED,strVar))
                {
                    //delay between enabled
                    if(strVar == "TRUE")
                    {
                        m_bDelayBetweenMsgBlocks = true;
                    }
                    else
                    {
                        m_bDelayBetweenMsgBlocks = false;
                    }
                    CTxMsgManager::s_bDelayBetweenBlocksOnly = m_bDelayBetweenMsgBlocks;
                }
                if(xmlUtils::GetDataFrmNode(pNode,DEF_BLOCKS_TIME_DELAY,strVar))
                {
                    m_unTimeDelayBtwnMsgBlocks =  (UINT)atoi(strVar.c_str());               //delay in milli seconds
                    CTxMsgManager::s_unTimeDelayBtnMsgBlocks = m_unTimeDelayBtwnMsgBlocks;
                }

                bGetSplitterDataFrmNode(pNode);

                pNode = pNode->next;
            }
        }
    }
    return true;
}

bool CTxWndDataStore::bGetSplitterDataFrmNode(xmlNodePtr pxmlNodePtr)
{
    xmlNodePtr pChildNode = NULL;
    string   strVar;
    {
        if(!xmlStrcmp(pxmlNodePtr->name, (const xmlChar*)DEF_SW_ROOT_0))
        {
            pChildNode = pxmlNodePtr->children;
            while(pChildNode != NULL)
            {
                if(xmlUtils::GetDataFrmNode(pChildNode,DEF_CX_IDEAL,strVar))
                {
                    m_sTxMsgWndSplitterPos.m_nRootSplitterData[0][0] = atoi(strVar.c_str());
                }
                if(xmlUtils::GetDataFrmNode(pChildNode,DEF_CX_MIN,strVar))
                {
                    m_sTxMsgWndSplitterPos.m_nRootSplitterData[0][1] = atoi(strVar.c_str());
                }
                pChildNode = pChildNode->next;
            }
        }

        if(!xmlStrcmp(pxmlNodePtr->name, (const xmlChar*)DEF_SW_ROOT_1))
        {
            pChildNode = pxmlNodePtr->children;
            while(pChildNode != NULL)
            {
                if(xmlUtils::GetDataFrmNode(pChildNode,DEF_CX_IDEAL,strVar))
                {
                    m_sTxMsgWndSplitterPos.m_nRootSplitterData[1][0] = atoi(strVar.c_str());
                }
                if(xmlUtils::GetDataFrmNode(pChildNode,DEF_CX_MIN,strVar))
                {
                    m_sTxMsgWndSplitterPos.m_nRootSplitterData[1][1] = atoi(strVar.c_str());
                }
                pChildNode = pChildNode->next;
            }
        }

        if(!xmlStrcmp(pxmlNodePtr->name, (const xmlChar*)DEF_SW_LEFT_0))
        {
            pChildNode = pxmlNodePtr->children;
            while(pChildNode != NULL)
            {
                if(xmlUtils::GetDataFrmNode(pChildNode,DEF_CX_IDEAL,strVar))
                {
                    m_sTxMsgWndSplitterPos.m_nLeftViewSplitterData[0][0] = atoi(strVar.c_str());
                }
                if(xmlUtils::GetDataFrmNode(pChildNode,DEF_CX_MIN,strVar))
                {
                    m_sTxMsgWndSplitterPos.m_nLeftViewSplitterData[0][1] = atoi(strVar.c_str());
                }
                pChildNode = pChildNode->next;
            }
        }

        if(!xmlStrcmp(pxmlNodePtr->name, (const xmlChar*)DEF_SW_LEFT_1))
        {
            pChildNode = pxmlNodePtr->children;
            while(pChildNode != NULL)
            {
                if(xmlUtils::GetDataFrmNode(pChildNode,DEF_CX_IDEAL,strVar))
                {
                    m_sTxMsgWndSplitterPos.m_nLeftViewSplitterData[1][0] = atoi(strVar.c_str());
                }
                if(xmlUtils::GetDataFrmNode(pChildNode,DEF_CX_MIN,strVar))
                {
                    m_sTxMsgWndSplitterPos.m_nLeftViewSplitterData[1][1] = atoi(strVar.c_str());
                }
                pChildNode = pChildNode->next;
            }
        }

        if(!xmlStrcmp(pxmlNodePtr->name, (const xmlChar*)DEF_SW_RIGHT_0))
        {
            pChildNode = pxmlNodePtr->children;
            while(pChildNode != NULL)
            {
                if(xmlUtils::GetDataFrmNode(pChildNode,DEF_CX_IDEAL,strVar))
                {
                    m_sTxMsgWndSplitterPos.m_nRightViewSplitterData[0][0] = atoi(strVar.c_str());
                }
                if(xmlUtils::GetDataFrmNode(pChildNode,DEF_CX_MIN,strVar))
                {
                    m_sTxMsgWndSplitterPos.m_nRightViewSplitterData[0][1] = atoi(strVar.c_str());
                }
                pChildNode = pChildNode->next;
            }
        }

        if(!xmlStrcmp(pxmlNodePtr->name, (const xmlChar*)DEF_SW_RIGHT_1))
        {
            pChildNode = pxmlNodePtr->children;
            while(pChildNode != NULL)
            {
                if(xmlUtils::GetDataFrmNode(pChildNode,DEF_CX_IDEAL,strVar))
                {
                    m_sTxMsgWndSplitterPos.m_nRightViewSplitterData[1][0] = atoi(strVar.c_str());
                }
                if(xmlUtils::GetDataFrmNode(pChildNode,DEF_CX_MIN,strVar))
                {
                    m_sTxMsgWndSplitterPos.m_nRightViewSplitterData[1][1] = atoi(strVar.c_str());
                }
                pChildNode = pChildNode->next;
            }
        }
        pxmlNodePtr = pxmlNodePtr->next;
    }
    return true;
}


BOOL CTxWndDataStore::pbyGetConfigData(xmlNodePtr pxmlNodePtr)
{
    PSMSGBLOCKLIST psTempBlock = m_psMsgBlockList;

    const char* omcVarChar ;

    while (psTempBlock != NULL && m_unNumberOfMsgBlockCount > 0)
    {
        //<Message_Block>
        xmlNodePtr pNodeMsgBlock = xmlNewNode(NULL, BAD_CAST DEF_MSG_BLOCK);
        xmlAddChild(pxmlNodePtr, pNodeMsgBlock);

        //<Block_Name>Block_name</Block_Name>
        CString csName = psTempBlock->m_acStrBlockName;
        omcVarChar = csName;
        xmlNodePtr pName = xmlNewChild(pNodeMsgBlock, NULL, BAD_CAST DEF_BLOCK_NAME,BAD_CAST omcVarChar);
        xmlAddChild(pNodeMsgBlock, pName);

        //<IsEnabled>bool</IsEnabled>
        CString csIsEnabled;
        csIsEnabled.Format("%d", psTempBlock->m_bActive);
        omcVarChar = csIsEnabled;
        xmlNodePtr pEnabled = xmlNewChild(pNodeMsgBlock, NULL, BAD_CAST DEF_MSG_ENABLED, BAD_CAST omcVarChar);
        xmlAddChild(pNodeMsgBlock, pEnabled);

        //<Trigger>KEY</Trigger>
        CString csTrigger;
        csTrigger.Format("%d", psTempBlock->m_ucTrigger);
        omcVarChar = csTrigger;
        xmlNodePtr pTrigger = xmlNewChild(pNodeMsgBlock, NULL, BAD_CAST DEF_MSG_TRIGGER, BAD_CAST omcVarChar);
        xmlAddChild(pNodeMsgBlock, pTrigger);

        //<Index>int</Index>
        CString csIndex;
        csIndex.Format("%d", psTempBlock->m_unIndex);
        omcVarChar = csIndex;
        xmlNodePtr pIndex = xmlNewChild(pNodeMsgBlock, NULL, BAD_CAST DEF_INDEX, BAD_CAST omcVarChar);
        xmlAddChild(pNodeMsgBlock, pIndex);
        //<Key_Value>Key</Key_Value>
        CString csKeyValue;
        csKeyValue.Format("%c", psTempBlock->m_ucKeyValue);
        omcVarChar = csKeyValue;
        xmlNodePtr pKeyValue = xmlNewChild(pNodeMsgBlock, NULL, BAD_CAST DEF_KEY_VAL, BAD_CAST omcVarChar);
        xmlAddChild(pNodeMsgBlock, pKeyValue);

        //<Time_Interval>Int_val</Time_Interval>
        CString csTimerInterval;
        csTimerInterval.Format("%u", psTempBlock->m_unTimeInterval);
        omcVarChar = csTimerInterval;
        xmlNodePtr pTimerInterval = xmlNewChild(pNodeMsgBlock, NULL, BAD_CAST DEF_TIME_INTERVAL, BAD_CAST omcVarChar);
        xmlAddChild(pNodeMsgBlock, pTimerInterval);

        //<IsCyclic>TRUE</IsCyclic>
        CString csType;
        csType.Format("%u", psTempBlock->m_bType);
        omcVarChar = csType;
        xmlNodePtr pType = xmlNewChild(pNodeMsgBlock, NULL, BAD_CAST DEF_MSG_IS_CYCLIC, BAD_CAST omcVarChar);
        xmlAddChild(pNodeMsgBlock, pType);

        //<Transmit_All_Frames>bool</Transmit_All_Frames>
        CString csAllFrame;
        csAllFrame.Format("%u",psTempBlock->m_bTxAllFrame);
        omcVarChar = csAllFrame;
        xmlNodePtr pAllFrame = xmlNewChild(pNodeMsgBlock, NULL, BAD_CAST DEF_TRANSMIT_ALL_FRAMES, BAD_CAST omcVarChar);
        xmlAddChild(pNodeMsgBlock, pAllFrame);

        PSTXCANMSGLIST psTempCanList = psTempBlock->m_psTxCANMsgList;
        while (psTempCanList != NULL)           //loop through the message to store it
        {
            xmlNodePtr pNodeMsg = xmlNewNode(NULL, BAD_CAST DEF_MSGS); //<MESSAGES>
            xmlAddChild(pNodeMsgBlock, pNodeMsg);

            //<Id>X</Id> , where X is Msg ID
            CString csMsgID;
            csMsgID.Format("%u", psTempCanList->m_sTxMsgDetails.m_sTxMsg.m_unMsgID);
            omcVarChar = csMsgID;
            xmlNodePtr pMsgID = xmlNewChild(pNodeMsg, NULL, BAD_CAST DEF_MSG_ID, BAD_CAST omcVarChar);
            xmlAddChild(pNodeMsg, pMsgID);

            //<IsExtended>X</IsExtended> , where X is bool
            CString csExtended;

            if(psTempCanList->m_sTxMsgDetails.m_sTxMsg.m_ucEXTENDED == TRUE)
            {
                omcVarChar = "TRUE";
            }
            else
            {
                omcVarChar = "FALSE";
            }

            //csExtended.Format("%c", psTempCanList->m_sTxMsgDetails.m_sTxMsg.m_ucEXTENDED);
            //omcVarChar = csExtended;
            xmlNodePtr pExtended = xmlNewChild(pNodeMsg, NULL, BAD_CAST DEF_EXTENDED, BAD_CAST omcVarChar);
            xmlAddChild(pNodeMsg, pExtended);

            //<IsRtr>X</IsRtr> , where X is RTR
            CString csRTR;
            csRTR.Format("%d", psTempCanList->m_sTxMsgDetails.m_sTxMsg.m_ucRTR);
            omcVarChar = csRTR;
            xmlNodePtr pRTR = xmlNewChild(pNodeMsg, NULL, BAD_CAST DEF_RTR, BAD_CAST omcVarChar);
            xmlAddChild(pNodeMsg, pRTR);

            //<DLC>X</DLC> , where X is DLC
            CString csDLC;
            csDLC.Format("%d", psTempCanList->m_sTxMsgDetails.m_sTxMsg.m_ucDataLen);
            omcVarChar = csDLC;
            xmlNodePtr pDLC = xmlNewChild(pNodeMsg, NULL, BAD_CAST DEF_DLC, BAD_CAST omcVarChar);
            xmlAddChild(pNodeMsg, pDLC);
            //<Index>int</Index>
            CString csIndex;
            csIndex.Format("%d", psTempCanList->m_unIndex);
            omcVarChar = csIndex;
            xmlNodePtr pIndex = xmlNewChild(pNodeMsg, NULL, BAD_CAST DEF_INDEX, BAD_CAST omcVarChar);
            xmlAddChild(pNodeMsg, pIndex);

            //<Channel>X</Channel> , where X is channel
            CString csChannel;
            csChannel.Format("%d", psTempCanList->m_sTxMsgDetails.m_sTxMsg.m_ucChannel);
            omcVarChar = csChannel;
            xmlNodePtr pChannel = xmlNewChild(pNodeMsg, NULL, BAD_CAST DEF_CHANNEL, BAD_CAST omcVarChar);
            xmlAddChild(pNodeMsg, pChannel);

            xmlNodePtr pNodeDataBytes = xmlNewNode(NULL, BAD_CAST DEF_DATABYTES);       //<DataBytes>
            xmlAddChild(pNodeMsg, pNodeDataBytes);
            for(int iDataCnt =0; iDataCnt <8; iDataCnt++)
            {
                //  <Byte>X</Byte>, where X is the data
                CString  csDataByte;
                csDataByte.Format("%d", psTempCanList->m_sTxMsgDetails.m_sTxMsg.m_ucData[iDataCnt]);
                omcVarChar = csDataByte;
                xmlNodePtr pDataByte= xmlNewChild(pNodeDataBytes, NULL, BAD_CAST DEF_BYTE, BAD_CAST omcVarChar);
                xmlAddChild(pNodeDataBytes, pDataByte);
            }
            //<Channel>X</Channel> , where X is channel
            CString csMsgEnabled;
            csMsgEnabled.Format("%d", psTempCanList->m_sTxMsgDetails.m_bEnabled);
            omcVarChar = csMsgEnabled;
            xmlNodePtr pMsgEnabled = xmlNewChild(pNodeMsg, NULL, BAD_CAST DEF_MSG_ENABLED, BAD_CAST omcVarChar);
            xmlAddChild(pNodeMsg, pMsgEnabled);

            psTempCanList = psTempCanList->m_psNextMsgDetails;
        }

        psTempBlock = psTempBlock->m_psNextMsgBlocksList;
    }

    //windows position
    xmlNodePtr pNodeWndPos = xmlNewNode(NULL, BAD_CAST DEF_WND_POS);
    xmlAddChild(pxmlNodePtr, pNodeWndPos);

    xmlUtils::CreateXMLNodeFrmWindowsPlacement(pNodeWndPos,m_sTxWndPlacement);

    CString csAutoSave;
    if(m_bAutoSavedEnabled)
    {
        csAutoSave = "1";
    }
    else
    {
        csAutoSave = "0";
    }


    omcVarChar = csAutoSave;
    xmlNodePtr pAutoSave = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_AUTOSAVE, BAD_CAST omcVarChar);
    xmlAddChild(pxmlNodePtr, pAutoSave);

    CString strIsDelayBtwnBlcksEnabled = "FALSE";

    if(m_bDelayBetweenMsgBlocks == TRUE)
    {
        strIsDelayBtwnBlcksEnabled = "TRUE";
    }

    xmlNodePtr pIsEnbldBlcksDelay = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_BLOCKS_TIME_ENABLED, BAD_CAST strIsDelayBtwnBlcksEnabled.GetBuffer(strIsDelayBtwnBlcksEnabled.GetLength()));
    xmlAddChild(pxmlNodePtr, pIsEnbldBlcksDelay);

    CString strDelayBtwnBlcks = "";

    strDelayBtwnBlcks.Format("%d", m_unTimeDelayBtwnMsgBlocks);

    xmlNodePtr pIsBlcksDelay = xmlNewChild(pxmlNodePtr, NULL, BAD_CAST DEF_BLOCKS_TIME_DELAY, BAD_CAST strDelayBtwnBlcks.GetBuffer(strDelayBtwnBlcks.GetLength()));
    xmlAddChild(pxmlNodePtr, pIsBlcksDelay);

    bSplitterConfig(pxmlNodePtr);

    return TRUE;
}

bool CTxWndDataStore::bSplitterConfig(xmlNodePtr pxmlNodePtr)
{
    const char* omcVarChar ;

    //<Splitter_Window_Root_0>---------------------------------------------
    xmlNodePtr pNodeSWRoot0 = xmlNewNode(NULL, BAD_CAST DEF_SW_ROOT_0);
    xmlAddChild(pxmlNodePtr, pNodeSWRoot0);

    //<CxIdeal />
    CString  csCxIdeal;
    csCxIdeal.Format("%d", m_sTxMsgWndSplitterPos.m_nRootSplitterData[0][0] );
    omcVarChar = csCxIdeal;
    xmlNodePtr pCxIdeal = xmlNewChild(pNodeSWRoot0, NULL, BAD_CAST DEF_CX_IDEAL, BAD_CAST omcVarChar);
    xmlAddChild(pNodeSWRoot0, pCxIdeal);

    // <CxMin />
    CString  csCxMin;
    csCxMin.Format("%d", m_sTxMsgWndSplitterPos.m_nRootSplitterData[0][1] );
    omcVarChar = csCxMin;
    xmlNodePtr pcsCxMin = xmlNewChild(pNodeSWRoot0, NULL, BAD_CAST DEF_CX_MIN, BAD_CAST omcVarChar);
    xmlAddChild(pNodeSWRoot0, pcsCxMin);

    //<Splitter_Window_Root_1>-----------------------------------------------
    xmlNodePtr pNodeSWRoot1 = xmlNewNode(NULL, BAD_CAST DEF_SW_ROOT_1);
    xmlAddChild(pxmlNodePtr, pNodeSWRoot1);

    //<CxIdeal />
    csCxIdeal.Format("%d", m_sTxMsgWndSplitterPos.m_nRootSplitterData[1][0] );
    omcVarChar = csCxIdeal;
    pCxIdeal = xmlNewChild(pNodeSWRoot1, NULL, BAD_CAST DEF_CX_IDEAL, BAD_CAST omcVarChar);
    xmlAddChild(pNodeSWRoot1, pCxIdeal);

    // <CxMin />
    csCxMin.Format("%d", m_sTxMsgWndSplitterPos.m_nRootSplitterData[1][1] );
    omcVarChar = csCxMin;
    pcsCxMin = xmlNewChild(pNodeSWRoot1, NULL, BAD_CAST DEF_CX_MIN, BAD_CAST omcVarChar);
    xmlAddChild(pNodeSWRoot1, pcsCxMin);

    //<Splitter_Window_Left_0>--------------------------------------------------
    xmlNodePtr pNodeSWLeft0 = xmlNewNode(NULL, BAD_CAST DEF_SW_LEFT_0);
    xmlAddChild(pxmlNodePtr, pNodeSWLeft0);

    //<CxIdeal />
    csCxIdeal.Format("%d", m_sTxMsgWndSplitterPos.m_nLeftViewSplitterData[0][0] );
    omcVarChar = csCxIdeal;
    pCxIdeal = xmlNewChild(pNodeSWLeft0, NULL, BAD_CAST DEF_CX_IDEAL, BAD_CAST omcVarChar);
    xmlAddChild(pNodeSWLeft0, pCxIdeal);

    // <CxMin />
    csCxMin.Format("%d", m_sTxMsgWndSplitterPos.m_nLeftViewSplitterData[0][1] );
    omcVarChar = csCxMin;
    pcsCxMin = xmlNewChild(pNodeSWLeft0, NULL, BAD_CAST DEF_CX_MIN, BAD_CAST omcVarChar);
    xmlAddChild(pNodeSWLeft0, pcsCxMin);

    //<Splitter_Window_Left_1>---------------------------------------------------
    xmlNodePtr pNodeSWLeft1 = xmlNewNode(NULL, BAD_CAST DEF_SW_LEFT_1);
    xmlAddChild(pxmlNodePtr, pNodeSWLeft1);

    //<CxIdeal />
    csCxIdeal.Format("%d", m_sTxMsgWndSplitterPos.m_nLeftViewSplitterData[1][0] );
    omcVarChar = csCxIdeal;
    pCxIdeal = xmlNewChild(pNodeSWLeft1, NULL, BAD_CAST DEF_CX_IDEAL, BAD_CAST omcVarChar);
    xmlAddChild(pNodeSWLeft1, pCxIdeal);

    // <CxMin />
    csCxMin.Format("%d", m_sTxMsgWndSplitterPos.m_nLeftViewSplitterData[1][1] );
    omcVarChar = csCxMin;
    pcsCxMin = xmlNewChild(pNodeSWLeft1, NULL, BAD_CAST DEF_CX_MIN, BAD_CAST omcVarChar);
    xmlAddChild(pNodeSWLeft1, pcsCxMin);

    //<Splitter_Window_Right_0>--------------------------------------------------
    xmlNodePtr pNodeSWRight0 = xmlNewNode(NULL, BAD_CAST DEF_SW_RIGHT_0);
    xmlAddChild(pxmlNodePtr, pNodeSWRight0);

    //<CxIdeal />
    csCxIdeal.Format("%d", m_sTxMsgWndSplitterPos.m_nRightViewSplitterData[0][0] );
    omcVarChar = csCxIdeal;
    pCxIdeal = xmlNewChild(pNodeSWRight0, NULL, BAD_CAST DEF_CX_IDEAL, BAD_CAST omcVarChar);
    xmlAddChild(pNodeSWRight0, pCxIdeal);

    // <CxMin />
    csCxMin.Format("%d", m_sTxMsgWndSplitterPos.m_nRightViewSplitterData[0][1] );
    omcVarChar = csCxMin;
    pcsCxMin = xmlNewChild(pNodeSWRight0, NULL, BAD_CAST DEF_CX_MIN, BAD_CAST omcVarChar);
    xmlAddChild(pNodeSWRight0, pcsCxMin);

    //<Splitter_Window_Right_1>---------------------------------------------------
    xmlNodePtr pNodeSWRight1 = xmlNewNode(NULL, BAD_CAST DEF_SW_RIGHT_1);
    xmlAddChild(pxmlNodePtr, pNodeSWLeft1);

    //<CxIdeal />
    csCxIdeal.Format("%d", m_sTxMsgWndSplitterPos.m_nRightViewSplitterData[1][0] );
    omcVarChar = csCxIdeal;
    pCxIdeal = xmlNewChild(pNodeSWLeft1, NULL, BAD_CAST DEF_CX_IDEAL, BAD_CAST omcVarChar);
    xmlAddChild(pNodeSWLeft1, pCxIdeal);

    // <CxMin />
    csCxMin.Format("%d", m_sTxMsgWndSplitterPos.m_nRightViewSplitterData[1][1] );
    omcVarChar = csCxMin;
    pcsCxMin = xmlNewChild(pNodeSWLeft1, NULL, BAD_CAST DEF_CX_MIN, BAD_CAST omcVarChar);
    xmlAddChild(pNodeSWLeft1, pcsCxMin);

    return true;
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
            psDestMsgBlockList->m_unIndex           =
                psSrcMsgBlockList->m_unIndex;
            strcpy_s( psDestMsgBlockList->m_acStrBlockName, defBLOCKNAME_SIZE,
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
    strcpy_s(psDest->m_acStrBlockName, defBLOCKNAME_SIZE, psSrc->m_acStrBlockName);
    psDest->m_bActive              = psSrc->m_bActive;
    psDest->m_bTxAllFrame          = psSrc->m_bTxAllFrame;
    psDest->m_bType                = psSrc->m_bType;
    psDest->m_ucKeyValue           = psSrc->m_ucKeyValue;
    psDest->m_ucTrigger            = psSrc->m_ucTrigger;
    psDest->m_unMsgCount           = psSrc->m_unMsgCount;
    psDest->m_unTimeInterval       = psSrc->m_unTimeInterval;
    psDest->m_unIndex               = psSrc->m_unIndex;

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

            _tcscpy( psDesMsgBlockList->m_acStrBlockName,
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
        psMsgDetails->m_unIndex = -1;
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
        psMsgBlockList->m_unIndex             = -1;
        strcpy_s( psMsgBlockList->m_acStrBlockName, defBLOCKNAME_SIZE, _(defDEFAULT_MSG_BLOCK_NAME));
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