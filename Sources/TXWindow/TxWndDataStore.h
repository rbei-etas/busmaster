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
 * @author Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

typedef enum eTXWNDDETAILS
{
    TX_MSG_BLOCK_COUNT,    // Message Block count
    TX_WND_SPLITTER_DATA,
    TX_SEND_MULTI_MSGS,
    TX_WINDOW_PLACEMENT,
    TX_AUTO_UPDATE_ENABLE,
    TX_DELAY_BTWN_MSG_BLCK_BOOL,
    TX_DELAY_BTWN_MSG_BLCK_UINT,
    TX_MSG_UPDATE
};

class CTxWndDataStore
{
public:
    CTxWndDataStore(void);
    ~CTxWndDataStore(void);

    static CTxWndDataStore& ouGetTxWndDataStoreObj();
    static CTxWndDataStore m_sTxWndDataStoreObj;
    bool m_bAutoSavedEnabled;
    bool m_bDelayBetweenMsgBlocks;
    UINT m_unTimeDelayBtwnMsgBlocks;

    BOOL bGetTxData(eTXWNDDETAILS  eParam, LPVOID* lpData);

    BOOL bSetTxData(eTXWNDDETAILS  eParam, LPVOID lpVoid);

    /**
     * @brief sets the multiple message structure data member with the info
     *
     * This method sets the information from the input
     * structure into the data member. This updates the
     * information.
     *
     * @param[in] pSendMultMsgInfo in param structure
     * @return TRUE if the method succeeds. FALSE if memory allocation fails
     */
    BOOL bSetMultiMsgInfo(PSMSGBLOCKLIST psMsgBlockList);

    /**
     * @brief releases the multiple message structure pointer that is passed 
     *
     * This method releases all the memory allocated for the
     * send multiple message info structure.
     */
    void vReleaseMultiMsgInfo(PSMSGBLOCKLIST psMsgBlockList);

    /**
     * @brief gets the multiple message info
     *
     * This method gets the information for the multi message
     * information. The memory for the strucutre is already
     * allocated in the bGetData(..) method. This method
     * allocates memory as required by the other elements in
     * this structure.
     *
     * @param[in] pSendMultMsgInfo out-param structure
     * @return TRUE if the method succeeds. FALSE if memory allocation fails
     */
    BOOL bGetMultiMsgInfo(PSMSGBLOCKLIST psDestMsgBlockList);

    PSMSGBLOCKLIST psReturnMsgBlockPointer();

    BOOL bGetDefaultTXSplitterPostion(CRect omWndSize, LPVOID* psSplitterData);

    BYTE* pbySetConfigData(BYTE* pbyConfigData, INT nConfigSize);

    bool bSetDataToGlobal(PSMSGBLOCKLIST psMsgBlockList);

    /**
     * @brief Copies the data and content from one msgblock to another
     *
     * Copies the PSMSGBLOCKLIST from source to destination
     */
    bool bCopyBlockData(PSMSGBLOCKLIST psDestBlockList, PSMSGBLOCKLIST psSrcBlockList);

    /** delete the massages in MsgList of a block */
    bool bDeleteMsgList(PSTXCANMSGLIST& psMsgList);

    /**
     * @brief copy MsgList from local to global
     *
     * Copies the CanMsgList from source to destination
     */
    bool bCopyMsgList(PSTXCANMSGLIST&  ppsDestTxCANMsgList, PSTXCANMSGLIST*  ppsSrcTxCANMsgList);

    BYTE* pbyGetConfigData(BYTE*& pbyConfigData, INT& nConfigSize);

    bool pbySetConfigData(xmlDocPtr pDoc);

    BOOL pbyGetConfigData(xmlNodePtr pxmlNodePtr);

    bool bSplitterConfig(xmlNodePtr pxmlNodePtr);

    bool bGetSplitterDataFrmNode(xmlNodePtr pxmlNodePtr);

    /**
     * @brief To compare 2 BlockLists
     *
     * This function will compare 2 BlockList
     */
    int nCompareBlockLists(PSMSGBLOCKLIST psMsgBlockSrc);

    /** to compare 2 MsgLists */
    int nCompareMsgList(PSTXCANMSGLIST TxCanMsgListSrc,PSTXCANMSGLIST TxCanMsgListDest);

private:
    UINT m_unNumberOfMsgBlockCount;
    BOOL m_bIsConfigurationModified;

    /**  Tx Msg Splitter Position */
    STXMSGSPLITTERDATA m_sTxMsgWndSplitterPos;

    /** Tx Child frame window placement */
    WINDOWPLACEMENT m_sTxWndPlacement;

    /** send multiple msgs written/read from configuration */
    PSMSGBLOCKLIST m_psMsgBlockList;
    void vInitialiseMsgDetails(PSTXCANMSGLIST& psMsgDetails);
    void vInitialiseMsgBlock(PSMSGBLOCKLIST& psMsgBlockList);
};
