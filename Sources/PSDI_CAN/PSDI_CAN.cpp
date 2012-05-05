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
 * \file      PSDI_CAN.cpp
 * \brief     Implementation of CPSDI_CANApp class
 * \authors   Anish Kumar, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of CPSDI_CANApp class
 */

#include "PSDI_CAN/stdafx_CAN.h"
#include "PSDI_CAN.h"
#include "MsgContainer_CAN.h"
#include "MsgContainer_J1939.h"

#define USAGE_EXPORT
#include "Application/PSDI_Extern.h"
//
//  Note!
//
//      If this DLL is dynamically linked against the MFC
//      DLLs, any functions exported from this DLL which
//      call into MFC must have the AFX_MANAGE_STATE macro
//      added at the very beginning of the function.
//
//      For example:
//
//      extern "C" BOOL PASCAL EXPORT ExportedFunction()
//      {
//          AFX_MANAGE_STATE(AfxGetStaticModuleState());
//          // normal function body here
//      }
//
//      It is very important that this macro appear in each
//      function, prior to any calls into MFC.  This means that
//      it must appear as the first statement within the
//      function, even before any object variable declarations
//      as their constructors may generate calls into the MFC
//      DLL.
//
//      Please see MFC Technical Notes 33 and 58 for additional
//      details.
//

// CPSDI_CANApp

BEGIN_MESSAGE_MAP(CPSDI_CANApp, CWinApp)
END_MESSAGE_MAP()


/**
 * \brief Constructor
 *
 * CPSDI_CANApp construction
 */
CPSDI_CANApp::CPSDI_CANApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only CPSDI_CANApp object
CPSDI_CANApp theApp;


/**
 * \brief Init Instance
 *
 * CPSDI_CANApp initialization
 */
BOOL CPSDI_CANApp::InitInstance()
{
    CWinApp::InitInstance();
    return TRUE;
}
static CMsgContainerCAN* sg_pouMsgContainer_CAN = NULL;
static CMsgContainerJ1939* sg_pouMsgContainer_J1939 = NULL;

USAGEMODE HRESULT PSDI_GetInterface(ETYPE_BUS eBus, void** ppvInterface)
{
    HRESULT hResult = S_OK;

    switch (eBus)
    {
        case CAN:
        {
            if (NULL == sg_pouMsgContainer_CAN)
            {
                if ((sg_pouMsgContainer_CAN = new CMsgContainerCAN) == NULL)
                {
                    ASSERT(FALSE);
                    hResult = S_FALSE;
                }
            }

            // Else the object has been existing already
            *ppvInterface = (void*) sg_pouMsgContainer_CAN;
            /* Doesn't matter even if sg_pouMsgContainer_CAN is null */
        }
        break;

        case J1939:
        {
            if (NULL == sg_pouMsgContainer_J1939)
            {
                if ((sg_pouMsgContainer_J1939 = new CMsgContainerJ1939) == NULL)
                {
                    ASSERT(FALSE);
                    hResult = S_FALSE;
                }
            }

            // Else the object has been existing already
            *ppvInterface = (void*) sg_pouMsgContainer_J1939;
            /* Doesn't matter even if sg_pouMsgContainer_J1939 is null */
        }
        break;

        default:
            *ppvInterface = NULL;
            hResult = S_FALSE;
            break;
    }

    return hResult;
}

/**
 * \brief Initialization
 * \param[in] rParam to pass any required parameter ie. list is below
 *
 * Initialization
 */
USAGEMODE HRESULT PSDI_Init(void* pParam)
{
    HRESULT hResult = S_FALSE;
    sg_pouMsgContainer_CAN->vInit(pParam);

    if (sg_pouMsgContainer_CAN->bGetDilInterFace())
    {
        hResult = S_OK;
    }

    return hResult;
}

/**
 * \brief Edit Clear All
 *
 * To remove all the data in OW and Append Buffer
 */
USAGEMODE HRESULT PSDI_EditClearAll()
{
    sg_pouMsgContainer_CAN->vEditClearAll();
    return S_OK;
}

/**
 * \brief Get Append Buffer Count
 *
 * No. of messages in Append buffer
 */
USAGEMODE HRESULT PSDI_GetAppendBufferCount(int& nCount)
{
    nCount = sg_pouMsgContainer_CAN->nGetAppendBufferCount();
    return S_OK;
}

/**
 * \brief Get OW Buffer Count
 *
 * No. of messages in Over Write buffer
 */
USAGEMODE HRESULT PSDI_GetOWBufferCount(int& nCount)
{
    nCount = sg_pouMsgContainer_CAN->nGetOWBufferCount();
    return S_OK;
}

/**
 * \brief Start Read Thread
 *
 * Start reading the messages
 */
USAGEMODE HRESULT PSDI_StartReadThread()
{
    sg_pouMsgContainer_CAN->bStartReadThread();
    return S_OK;
}

/**
 * \brief Stop Read Thread
 *
 * Stop reading the messages
 */
USAGEMODE HRESULT PSDI_StopReadThread()
{
    sg_pouMsgContainer_CAN->bStopReadThread();
    return S_OK;
}

/**
 * \brief Get Updated Current Data Pointer Array
 *
 * Get the data pointers for displaying the message formatted data
 */
USAGEMODE HRESULT PSDI_GetUpdatedCurrDataPtrArray( SMSGWNDHDRCOL& sHdrColStruct,
        char* pomDataPtrArr[MAX_MSG_WND_COL_CNT],
        BYTE bExprnFlag_Disp)
{
    sg_pouMsgContainer_CAN->vGetUpdatedCurrDataPtrArray( sHdrColStruct,
            pomDataPtrArr,
            bExprnFlag_Disp);
    return S_OK;
}

/**
 * \brief Read From OW Buffer
 *
 * Over write message
 */
USAGEMODE HRESULT PSDI_ReadFromOWBuffer( void* psMsg, __int64 nMapIndex)
{
    return sg_pouMsgContainer_CAN->hReadFromOWBuffer(psMsg, nMapIndex);
}

/**
 * \brief Read From Append Buffer
 *
 * Append message
 */
USAGEMODE HRESULT PSDI_ReadFromAppendBuffer( void* psMsg, int nMsgIndex)
{
    return sg_pouMsgContainer_CAN->hReadFromAppendBuffer(psMsg, nMsgIndex);
}

/**
 * \brief Update Formatted Message Structure
 *
 * Update the Message formatted data pointers due to change in display format
 */
USAGEMODE HRESULT PSDI_UpdateFormattedMsgStruct( int nListIndex, int& nMsgCode,
        BYTE bExprnFlag_Disp,
        __int64 nTimeOffset)
{
    return sg_pouMsgContainer_CAN->hUpdateFormattedMsgStruct( nListIndex, nMsgCode,
            bExprnFlag_Disp, nTimeOffset);
}

/**
 * \brief Set Current Message Name
 *
 * Set the name (from database) of current message being processed
 */
USAGEMODE HRESULT PSDI_SetCurrMsgName(CString strMsgNameOrCode)
{
    sg_pouMsgContainer_CAN->vSetCurrMsgName(strMsgNameOrCode);
    return S_OK;
}

/**
 * \brief Clear Formatted Message Structure
 *
 * Clear the formatted msg data pointers so that there won't be any data to be displayed
 * for current entry ie. rows behind interpretation tree
 */
USAGEMODE HRESULT PSDI_ClearFormattedMsgStruct()
{
    sg_pouMsgContainer_CAN->vClearFormattedMsgStruct();
    return S_OK;
}

/**
 * \brief Save OW and Get Details
 *
 * Save the current message to OW buffers and get the reqd. details for MsgDispEntry struct
 */
USAGEMODE HRESULT PSDI_SaveOWandGetDetails( void* pMsg,
        __int64& dwMapIndex,
        __int64& dwTimeStamp,
        UINT& nMsgCode,
        int& nBufferIndex )
{
    sg_pouMsgContainer_CAN->vSaveOWandGetDetails( pMsg,
            dwMapIndex,
            dwTimeStamp,
            nMsgCode,
            nBufferIndex );
    return S_OK;
}

/**
 * \brief Set Rx Message Callback Pointer
 *
 * Set the call back function pointer for any new msg received
 */
USAGEMODE HRESULT PSDI_SetRxMsgCallBkPtr(MSG_RX_CALL_BK pFuncPtr)
{
    sg_pouMsgContainer_CAN->vSetRxMsgCallBkPtr(pFuncPtr);
    return S_OK;
}

/**
 * \brief Set Client ID
 *
 * Set the Client ID currently assigned to the application.
 */
USAGEMODE HRESULT PSDI_SetClientID(DWORD dwClientID)
{
    sg_pouMsgContainer_CAN->SetClientID(dwClientID);
    return S_OK;
}

/**
 * \brief Do Sort Buffer
 *
 * Sort the Message Buffers based on the Column ID
 */
USAGEMODE HRESULT PSDI_DoSortBuffer(int nField,bool bAscending)
{
    sg_pouMsgContainer_CAN->DoSortBuffer(nField,bAscending);
    return S_OK;
}

/**
 * \brief Get Map Index At ID
 *
 * Sort the Message Buffers based on the Column ID
 */
USAGEMODE HRESULT PSDI_nGetMapIndexAtID(int nIndex,__int64& nMapIndex)
{
    sg_pouMsgContainer_CAN->GetMapIndexAtID(nIndex,nMapIndex);
    return S_OK;
}

/**
 * \brief Apply Filter Scheme
 *
 * Apply filter scheme.
 */
USAGEMODE HRESULT PSDI_ApplyFilterScheme(void* pvFilterApplied)
{
    return sg_pouMsgContainer_CAN->ApplyFilterScheme(pvFilterApplied);
}

/**
 * \brief Get Filter Scheme
 *
 * Get filter scheme
 */
USAGEMODE HRESULT PSDI_GetFilterScheme(void* pvFilterApplied)
{
    return sg_pouMsgContainer_CAN->GetFilterScheme(pvFilterApplied);
}

/**
 * \brief Enable Filter Applied
 *
 * enable filter applied
 */
USAGEMODE HRESULT PSDI_EnableFilterApplied(BOOL bEnable)
{
    return sg_pouMsgContainer_CAN->EnableFilterApplied(bEnable);
}
