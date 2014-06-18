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

#include "include/struct_can.h"
#define defTX_MSG_WND_BOTTOM_MARGIN     0.14
#define defTX_MSG_WND_RIGHT_MARGIN      0.17

//#define defTX_MSG_WND_VERSION         0x01 // Initial version
#define defTX_MSG_WND_VERSION           0x03 // Added auto save option

/** Tx window spilitter details */
struct tagTxMsgSplitterData
{
    int m_nRootSplitterData[2][2];
    int m_nLeftViewSplitterData[2][2];
    int m_nRightViewSplitterData[2][2];
};
typedef tagTxMsgSplitterData sTxMsgSplitterData;
typedef sTxMsgSplitterData STXMSGSPLITTERDATA;
typedef STXMSGSPLITTERDATA* PSTXMSGSPLITTERDATA;

struct sTXCANMSGDETAILS
{
    /**
     * for a database message;to Indicate user enter
     * bytes value instead of signal value.
     */
    BOOL m_bIsMsgDirty;

    /** To indicate eligiblity of the message for Tx */
    BOOL m_bEnabled;

    STCAN_MSG m_sTxMsg;
};
typedef sTXCANMSGDETAILS STXCANMSGDETAILS;
typedef sTXCANMSGDETAILS* PSTXCANMSGDETAILS;

struct sTXCANMSGLIST
{
    bool        m_bModified;
    UINT        m_unIndex;
    STXCANMSGDETAILS m_sTxMsgDetails;
    struct sTXCANMSGLIST* m_psNextMsgDetails;
    sTXCANMSGLIST()
    {
        m_bModified         = false;
        m_unIndex           = 0;
        m_psNextMsgDetails = nullptr;
    }
};
typedef sTXCANMSGLIST STXCANMSGLIST;
typedef sTXCANMSGLIST* PSTXCANMSGLIST;

#define defDEFAULT_MSG_BLOCK_NAME   "OnTime"
#define defDEFAULT_TIME_VAL         100

// Bit wise representation of trigger condition
// Bit 0        - Time Trigger
// Bit 1        - Key Trigger
#define defTIME_TRIGGER     0x1
#define MASK_DISABLE_TIME_TRIGGER ~defTIME_TRIGGER
#define defKEY_TRIGGER      0x2
#define MASK_DISABLE_KEY_TRIGGER ~defKEY_TRIGGER

/* Macros to find the type */
#define IS_TIME_TRIGGERED(x)    ( (x) & defTIME_TRIGGER ? TRUE : FALSE )
#define IS_KEY_TRIGGERED(x)     ( (x) & defKEY_TRIGGER ? TRUE : FALSE )

#define defBLOCKNAME_SIZE                128

struct sMSGBLOCKLIST
{
    char m_acStrBlockName[defBLOCKNAME_SIZE];
    unsigned char m_ucTrigger;
    bool    m_bModified;
    BOOL m_bType;
    BOOL m_bActive;
    BOOL m_bTxAllFrame;
    UCHAR m_ucKeyValue;
    UINT m_unTimeInterval;
    UINT m_unMsgCount;
    UINT m_unIndex;
    PSTXCANMSGLIST m_psTxCANMsgList;
    sMSGBLOCKLIST* m_psNextMsgBlocksList;

    sMSGBLOCKLIST()
    {
        m_bModified           = false;
        m_psNextMsgBlocksList = nullptr;
        m_unMsgCount          = 0;
        m_bActive             = TRUE;
        m_bType               = TRUE;
        m_bTxAllFrame         = FALSE;
        m_ucTrigger           = defTIME_TRIGGER;
        m_psTxCANMsgList      = nullptr;
        m_ucKeyValue          = 0;
        m_unIndex             = 0;
        m_unTimeInterval      = defDEFAULT_TIME_VAL;
        m_psTxCANMsgList      = nullptr;
        strcpy_s(m_acStrBlockName, defBLOCKNAME_SIZE, defDEFAULT_MSG_BLOCK_NAME);
    }
};
typedef sMSGBLOCKLIST SMSGBLOCKLIST;
typedef sMSGBLOCKLIST* PSMSGBLOCKLIST;

struct sTHREAD_INFO
{
    HANDLE m_hThread;
    void* m_pvThread;
};
typedef sTHREAD_INFO  STHREADINFO;
typedef sTHREAD_INFO* PTHREADINFO;

struct sTXMSGINFO
{
    bool m_bMonoShotDone;
    UINT m_unTimeInterval;
    UINT m_unIndex;
    STHREADINFO m_sTimerThreadInfo;
    STHREADINFO m_sKeyThreadInfo;
    CEvent m_omTxBlockTimerEvent;
    CEvent m_omTxBlockKeyEvent;
    CEvent m_omKeyEvent;
    CEvent m_omTxBlockAutoUpdateEvent;// to set event from UI on auto update of Msg blocks
    CEvent m_omTxBlockAutoUpdateEventForKey;
    BOOL m_bType;
    BOOL m_bSendAllMessages;
    //CRITICAL_SECTION m_sMsgBlocksCriticalSection;
    HANDLE m_hSemaphore;
    PSTXCANMSGLIST m_psTxCANMsgList;
    struct sTXMSGINFO* m_psNextTxMsgInfo;

public:
    int nGetMsgCount()
    {
        int nCount = 0;
        PSTXCANMSGLIST psCanTemp = m_psTxCANMsgList;
        while (nullptr != psCanTemp)
        {
            ++nCount;
            psCanTemp = psCanTemp->m_psNextMsgDetails;
        }
        return nCount;
    }

public:
    sTXMSGINFO()
    {
        m_bMonoShotDone = false;
        m_sTimerThreadInfo.m_hThread = nullptr;
        m_sTimerThreadInfo.m_pvThread = nullptr;
        m_sKeyThreadInfo.m_hThread = nullptr;
        m_sKeyThreadInfo.m_pvThread = nullptr;
        m_hSemaphore = nullptr;
        m_psTxCANMsgList = nullptr;
        m_psNextTxMsgInfo = nullptr;
        m_unIndex             = 0;
    }

    int nGetConfigSize()
    {
        int nSize = 0;
        // Only those members which should be stored
        nSize = sizeof(m_unTimeInterval) + sizeof (m_bType) + sizeof(m_bSendAllMessages);
        // To store number of msg as int
        int nMsgCnt = nGetMsgCount();
        nSize += sizeof(nMsgCnt);
        // For each message only the STXCANMSGDETAILS will be saved
        nSize += nMsgCnt * sizeof (STXCANMSGDETAILS);
        return nSize;
    }

    BYTE* pbGetBlockConfigData(BYTE* pDesBuffer)
    {
        memcpy(pDesBuffer, &m_unTimeInterval, sizeof(m_unTimeInterval));
        pDesBuffer += sizeof(m_unTimeInterval);
        memcpy(pDesBuffer, &m_bType, sizeof(m_bType));
        pDesBuffer += sizeof(m_bType);
        memcpy(pDesBuffer, &m_bSendAllMessages, sizeof(m_bSendAllMessages));
        pDesBuffer += sizeof(m_bSendAllMessages);
        // To store number of msg as int
        int nMsgCnt = nGetMsgCount();
        memcpy(pDesBuffer, &nMsgCnt, sizeof(nMsgCnt));
        pDesBuffer += sizeof(nMsgCnt);
        PSTXCANMSGLIST psCanTemp = m_psTxCANMsgList;
        while (nullptr != psCanTemp)
        {
            memcpy(pDesBuffer, &psCanTemp->m_sTxMsgDetails, sizeof(psCanTemp->m_sTxMsgDetails));
            pDesBuffer += sizeof(psCanTemp->m_sTxMsgDetails);
            psCanTemp = psCanTemp->m_psNextMsgDetails;
        }
        return pDesBuffer;
    }

    BYTE* pbSetBlockConfigData(BYTE* pSrcBuffer)
    {
        memcpy(&m_unTimeInterval, pSrcBuffer, sizeof(m_unTimeInterval));
        pSrcBuffer += sizeof(m_unTimeInterval);
        memcpy(&m_bType, pSrcBuffer, sizeof(m_bType));
        pSrcBuffer += sizeof(m_bType);
        memcpy(&m_bSendAllMessages, pSrcBuffer, sizeof(m_bSendAllMessages));
        pSrcBuffer += sizeof(m_bSendAllMessages);
        // Load number of msg as int
        int nMsgCnt;
        memcpy(&nMsgCnt, pSrcBuffer, sizeof(nMsgCnt));
        pSrcBuffer += sizeof(nMsgCnt);
        PSTXCANMSGLIST psCanTemp = nullptr, psCanPrev = nullptr;
        m_psTxCANMsgList = nullptr;
        for (int i = 0; i < nMsgCnt; i++)
        {
            psCanTemp = new STXCANMSGLIST;
            memcpy(&psCanTemp->m_sTxMsgDetails, pSrcBuffer, sizeof(psCanTemp->m_sTxMsgDetails));
            pSrcBuffer += sizeof(psCanTemp->m_sTxMsgDetails);
            if (nullptr == m_psTxCANMsgList)
            {
                m_psTxCANMsgList = psCanTemp;
                psCanPrev = psCanTemp;
            }
            else
            {
                psCanPrev->m_psNextMsgDetails = psCanTemp;
                psCanPrev = psCanTemp;
            }
        }
        return pSrcBuffer;
    }
};
typedef  sTXMSGINFO   STXMSG;
typedef  sTXMSGINFO*  PSTXMSG;

struct sCOMPLETEMSGINFO
{
    PSMSGBLOCKLIST      m_psMsgBlockList;
    PSTXMSG             m_psTxMsg;
public:
    sCOMPLETEMSGINFO()
    {
        m_psMsgBlockList = nullptr;
        m_psTxMsg        = nullptr;
    }
};
typedef sCOMPLETEMSGINFO    SCOMPLETEMSGINFO;
typedef sCOMPLETEMSGINFO*   PSCOMPLETEMSGINFO;

struct sTXMSGDATA
{
    /** Total array element in the point m_psTxMsg */
    UINT  m_unCount;

    /**
     * pointer to array of structure for
     * Transmitting the data.
     */
    PSTCAN_MSG m_psTxMsg;
};
typedef sTXMSGDATA STXSELMSGDATA;
typedef sTXMSGDATA* PSTXSELMSGDATA;

#define defTX_SEL_MSG_THREAD                0
#define defOFFSET_TXMSG                     1
#define defTX_BLOCK_SEM_MAX_COUNT           2
#define defBITS_IN_BYTE                     8
#define defMAX_BYTE                         8
#define defDEFAULT_KEY_VAL                  'A'

const int DATA_LENGTH_MAX                 = 8;

/** For Tx message views enumeration */
enum tagTxViewTypes
{
    eTxMsgBlocksView,
    eTxMsgMessageListView,
    eTxMsgMessageDetailsView,
    eTxMsgFunctionsView
};
typedef tagTxViewTypes eTxMsgViewType;
typedef eTxMsgViewType ETXMSGVIEWTYPE;
typedef ETXMSGVIEWTYPE* PETXMSGVIEWTYPE;

typedef enum eUSERSELCTION
{
    eHEXDECCMD = 0,
    eTXMSGCMD,
    eCONNECTCMD,
    eDATABASEIMPORTCMD,
    eCONFIGCHANGECMD,
    eCHANNELCOUNTUPDATED
};

typedef struct tagSBYTE
{
    unsigned char Bit0 : 1;
    unsigned char Bit1 : 1;
    unsigned char Bit2 : 1;
    unsigned char Bit3 : 1;
    unsigned char Bit4 : 1;
    unsigned char Bit5 : 1;
    unsigned char Bit6 : 1;
    unsigned char Bit7 : 1;
} STRUCT_BYTE;

typedef union tagUBYTE
{
    BYTE        byByte;
    STRUCT_BYTE sByte;
} UNION_BYTE;

#define CHAR_INT                'I'
