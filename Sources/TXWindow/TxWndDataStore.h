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
 * \file      TxWndDataStore.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

typedef enum eTXWNDDETAILS
{
    TX_MSG_BLOCK_COUNT,    // Message Block count
    TX_WND_SPLITTER_DATA,
    TX_SEND_MULTI_MSGS,
    TX_WINDOW_PLACEMENT
};

class CTxWndDataStore
{
public:
    CTxWndDataStore(void);
    ~CTxWndDataStore(void);
    static CTxWndDataStore& ouGetTxWndDataStoreObj();
    static CTxWndDataStore m_sTxWndDataStoreObj;
    BOOL bGetTxData(eTXWNDDETAILS  eParam, LPVOID* lpData);
    BOOL bSetTxData(eTXWNDDETAILS  eParam, LPVOID lpVoid);
    // sets the multiple message structure data member with the info
    BOOL    bSetMultiMsgInfo(PSMSGBLOCKLIST psMsgBlockList);
    // releases the multiple message structure pointer that is passed..
    void    vReleaseMultiMsgInfo(PSMSGBLOCKLIST psMsgBlockList);
     // gets the multiple message info ..
    BOOL    bGetMultiMsgInfo(PSMSGBLOCKLIST psDestMsgBlockList);
    PSMSGBLOCKLIST psReturnMsgBlockPointer();
    BOOL bGetDefaultTXSplitterPostion(CRect omWndSize, LPVOID* psSplitterData);
    BYTE* pbySetConfigData(BYTE* pbyConfigData, INT nConfigSize);
    BYTE* pbyGetConfigData(BYTE*& pbyConfigData, INT& nConfigSize);
private:
    
    UINT               m_unNumberOfMsgBlockCount;
    BOOL               m_bIsConfigurationModified;
    // Tx Msg Splitter Position
    STXMSGSPLITTERDATA m_sTxMsgWndSplitterPos;
    //Tx Child frame window placement
    WINDOWPLACEMENT m_sTxWndPlacement;
    // send multiple msgs written/read from configuration
    PSMSGBLOCKLIST          m_psMsgBlockList;
    void vInitialiseMsgDetails(PSTXCANMSGLIST& psMsgDetails);
    void vInitialiseMsgBlock(PSMSGBLOCKLIST& psMsgBlockList);
};
