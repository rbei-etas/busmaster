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
 * \file      TXWnd_Defines.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "include/struct_lin.h"
#define defTX_MSG_WND_BOTTOM_MARGIN     0.14
#define defTX_MSG_WND_RIGHT_MARGIN      0.17
//#define defTX_MSG_WND_VERSION         0x01 // Initial version
#define defTX_MSG_WND_VERSION           0x03 // Added auto save option

// Tx window spilitter details
struct tagTxMsgSplitterData
{
    int m_nRootSplitterData[2][2];
    int m_nLeftViewSplitterData[2][2];
    int m_nRightViewSplitterData[2][2];
};

typedef tagTxMsgSplitterData sTxMsgSplitterData;
typedef sTxMsgSplitterData STXMSGSPLITTERDATA;
typedef STXMSGSPLITTERDATA* PSTXMSGSPLITTERDATA;


struct sTXLINMSGDETAILS
{
    BOOL m_bIsMsgDirty;          // for a database message;to Indicate user enter
    // bytes value instead of signal value.
    BOOL m_bEnabled;            // To indicate eligiblity of the message for Tx
    STLIN_MSG m_sTxMsg;
};
typedef sTXLINMSGDETAILS STXLINMSGDETAILS;
typedef sTXLINMSGDETAILS* PSTXLINMSGDETAILS;

struct sTXLINMSGLIST
{
    bool        m_bModified;
    UINT        m_unIndex;
    STXLINMSGDETAILS m_sTxMsgDetails;
    struct sTXLINMSGLIST* m_psNextMsgDetails;
    sTXLINMSGLIST()
    {
        m_bModified         = false;
        m_unIndex           = 0;
        m_psNextMsgDetails = NULL;
    }
};
typedef sTXLINMSGLIST STXLINMSGLIST;
typedef sTXLINMSGLIST* PSTXLINMSGLIST;

#define defDEFAULT_MSG_BLOCK_NAME   "OnTime"
#define defDEFAULT_TIME_VAL         100
// Bit wise representation of trigger condition
// Bit 0        - Time Trigger
// Bit 1        - Key Trigger
// Bit 2        -
// Bit 3        -

// Bit 4        -
// Bit 5        -
// Bit 6        -
// Bit 7        -
#define defTIME_TRIGGER     0x1
#define MASK_DISABLE_TIME_TRIGGER ~defTIME_TRIGGER
#define defKEY_TRIGGER      0x2
#define MASK_DISABLE_KEY_TRIGGER ~defKEY_TRIGGER

// Macros to find the type
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
    PSTXLINMSGLIST m_psTxLINMsgList;
    sMSGBLOCKLIST* m_psNextMsgBlocksList;
    /*  SLAVE(FALSE)/MASTER(TRUE) */
    BOOL m_bIsBlockTypeMaster;


    sMSGBLOCKLIST()
    {
        m_bModified           = false;
        m_psNextMsgBlocksList = NULL;
        m_unMsgCount          = 0;
        m_bActive             = TRUE;
        m_bType               = TRUE;
        m_bTxAllFrame         = FALSE;
        m_ucTrigger           = defTIME_TRIGGER;
        m_psTxLINMsgList      = NULL;
        m_ucKeyValue          = 0;
        m_unIndex             = 0;
        m_unTimeInterval      = defDEFAULT_TIME_VAL;
        m_psTxLINMsgList      = NULL;
        strcpy_s(m_acStrBlockName, defBLOCKNAME_SIZE, defDEFAULT_MSG_BLOCK_NAME);
        /* By default, every Block is Slave */
        m_bIsBlockTypeMaster  = FALSE;
    }
};
typedef sMSGBLOCKLIST SMSGBLOCKLIST;
typedef sMSGBLOCKLIST* PSMSGBLOCKLIST;



struct sTHREAD_INFO
{
    HANDLE m_hThread;
    VOID*  m_pvThread;
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
    PSTXLINMSGLIST m_psTxLINMsgList;
    struct sTXMSGINFO* m_psNextTxMsgInfo;

public:
    int nGetMsgCount()
    {
        int nCount = 0;
        PSTXLINMSGLIST psLinTemp = m_psTxLINMsgList;
        while (NULL != psLinTemp)
        {
            ++nCount;
            psLinTemp = psLinTemp->m_psNextMsgDetails;
        }
        return nCount;
    }

public:
    sTXMSGINFO()
    {
        m_bMonoShotDone = false;
        m_sTimerThreadInfo.m_hThread = NULL;
        m_sTimerThreadInfo.m_pvThread = NULL;
        m_sKeyThreadInfo.m_hThread = NULL;
        m_sKeyThreadInfo.m_pvThread = NULL;
        m_hSemaphore = NULL;
        m_psTxLINMsgList = NULL;
        m_psNextTxMsgInfo = NULL;
        m_unIndex             = 0;
    }
    int nGetConfigSize()
    {
        int nSize = 0;
        //Only those members which should be stored
        nSize = sizeof(m_unTimeInterval) + sizeof (m_bType) + sizeof(m_bSendAllMessages);
        //To store number of msg as int
        int nMsgCnt = nGetMsgCount();
        nSize += sizeof(nMsgCnt);
        //For each message only the STXLINMSGDETAILS will be saved
        nSize += nMsgCnt * sizeof (STXLINMSGDETAILS);
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
        //To store number of msg as int
        int nMsgCnt = nGetMsgCount();
        memcpy(pDesBuffer, &nMsgCnt, sizeof(nMsgCnt));
        pDesBuffer += sizeof(nMsgCnt);
        PSTXLINMSGLIST psLinTemp = m_psTxLINMsgList;
        while (NULL != psLinTemp)
        {
            memcpy(pDesBuffer, &psLinTemp->m_sTxMsgDetails, sizeof(psLinTemp->m_sTxMsgDetails));
            pDesBuffer += sizeof(psLinTemp->m_sTxMsgDetails);
            psLinTemp = psLinTemp->m_psNextMsgDetails;
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
        //Load number of msg as int
        int nMsgCnt;
        memcpy(&nMsgCnt, pSrcBuffer, sizeof(nMsgCnt));
        pSrcBuffer += sizeof(nMsgCnt);
        PSTXLINMSGLIST psLinTemp = NULL, psLinPrev = NULL;
        m_psTxLINMsgList = NULL;
        for (int i = 0; i < nMsgCnt; i++)
        {
            psLinTemp = new STXLINMSGLIST;
            memcpy(&psLinTemp->m_sTxMsgDetails, pSrcBuffer, sizeof(psLinTemp->m_sTxMsgDetails));
            pSrcBuffer += sizeof(psLinTemp->m_sTxMsgDetails);
            if (NULL == m_psTxLINMsgList)
            {
                m_psTxLINMsgList = psLinTemp;
                psLinPrev = psLinTemp;
            }
            else
            {
                psLinPrev->m_psNextMsgDetails = psLinTemp;
                psLinPrev = psLinTemp;
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
    PSTXMSG             m_psTxMsgLin;
public:
    sCOMPLETEMSGINFO()
    {
        m_psMsgBlockList = NULL;
        m_psTxMsgLin        = NULL;
    }
};
typedef sCOMPLETEMSGINFO    SCOMPLETEMSGINFO;
typedef sCOMPLETEMSGINFO*   PSCOMPLETEMSGINFO;
struct sTXMSGDATA
{
    UINT  m_unCount;              // Total array element in the point m_psTxMsgLin
    PSTLIN_MSG m_psTxMsgLin;         // pointer to array of structure for
    // Transmitting the data.
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

// For Tx message views enumeration
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
