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
* \file      FrameProcessor_LIN.cpp
* \brief     Source file for CFrameProcessor_LIN class.
* \author    Shashank Vernekar
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*
* Source file for CFrameProcessor_LIN class.
*/

#include "FrameProcessor_stdafx.h"
#include "include/Utils_macro.h"
//#include "Datatypes/MsgBufAll_DataTypes.h"
#include "FrameProcessor_LIN.h"
#include "LogObjectLIN.h"
#include "Filter/Filter_extern.h"



CFrameProcessor_LIN::CFrameProcessor_LIN():m_ouFormatMsgLIN(m_ouRefTimer)
{
    DIL_GetInterface( LIN, (void**)&m_pouDilLINInterface );
    m_eBusType = LIN;
    m_sLINProcParams.m_pILog = nullptr;
    m_sLINProcParams.m_pouLINBuffer = nullptr;
    m_bIsDataLogged = FALSE;
    m_sDataCopyThread.m_hActionEvent = m_ouFSEBufLIN.hGetNotifyingEvent();
}




CFrameProcessor_LIN::~CFrameProcessor_LIN()
{
    vEmptyLogObjArray(m_omLogListTmp);
    vEmptyLogObjArray(m_omLogObjectArray);
}

BOOL CFrameProcessor_LIN::InitInstance(void)
{
    BOOL Result = this->CFrameProcessor_Common::InitInstance();
    //m_sDataCopyThread.m_hActionEvent = m_ouFSEBufLIN.hGetNotifyingEvent();

    return Result;
}

int CFrameProcessor_LIN::ExitInstance(void)
{
    int Result = this->CFrameProcessor_Common::ExitInstance();
    m_ouFSEBufLIN.vClearMessageBuffer();

    return Result;
}

CBaseLogObject* CFrameProcessor_LIN::CreateNewLogObj(const CString& omStrVersion)
{
    CLogObjectLIN* pLogObj = nullptr;
    CString strVersion = CString(m_sLINProcParams.m_acVersion);
    if (strVersion.IsEmpty())
    {
        strVersion = omStrVersion;
    }
    pLogObj = new CLogObjectLIN(strVersion);
    return (static_cast<CBaseLogObject*> (pLogObj));
}

void CFrameProcessor_LIN::DeleteLogObj(CBaseLogObject*& pouLogObj)
{
    CLogObjectLIN* pLogObj = static_cast<CLogObjectLIN*> (pouLogObj);
    if (nullptr != pLogObj)
    {
        delete pLogObj;
        pouLogObj = nullptr;
    }
    else
    {
        ASSERT(false);
    }
}

void CFrameProcessor_LIN::CreateTimeModeMapping(SYSTEMTIME& CurrSysTime,
        UINT64& unAbsTime)
{
    if (m_pouDilLINInterface != nullptr)
    {
        LARGE_INTEGER QueryTickCount;
        m_pouDilLINInterface->DILL_GetTimeModeMapping(CurrSysTime, unAbsTime, QueryTickCount);
    }
}

void CFrameProcessor_LIN::vRetrieveDataFromBuffer(void)
{
    static SFORMATTEDDATA_LIN CurrDataLIN = {0, 0, DIR_RX, LIN_CHANNEL_ALL, 0,
        {'\0'}, TYPE_ID_LIN_NONE, TYPE_MSG_LIN_NONE, {'\0'},{'\0'},EVENT_LIN_NONE, " x", {'\0'}, "", "","", "",
        "", "",  "",  "", 0, RGB(0, 0, 0)
    };
    static STLINDATA CurrMsgLIN;

    while (m_ouFSEBufLIN.GetMsgCount() > 0)
    {
        m_ouFSEBufLIN.ReadFromBuffer(&CurrMsgLIN);

        if (CurrMsgLIN.m_ucDataType != INTR_FLAG)
        {
            // Update network statistics object.
            //m_sFlexProcParams.m_pouNetworkStat->UpdateNetworkStatistics(
            //                CurrFlxMsg.stcDataMsg.dwHeaderInfoFlags);

            if (m_bLogEnabled == TRUE)
            {
                //check for new logging session
                if(m_bResetAbsTime == TRUE)
                {
                    //update msg reset flag
                    m_ouFormatMsgLIN.m_bResetMsgAbsTime = m_bResetAbsTime;
                    m_ouFormatMsgLIN.m_LogSysTime = m_LogSysTime ;
                    m_bResetAbsTime = FALSE;
                }
                // Format current frame in the necessary settings
                m_ouFormatMsgLIN.vFormatLINDataMsg(&CurrMsgLIN, &CurrDataLIN, m_bExprnFlag_Log);

                USHORT ushBlocks = (USHORT) (m_omLogObjectArray.GetSize());
                for (USHORT i = 0; i < ushBlocks; i++)
                {
                    CBaseLogObject* pouLogObjBase = m_omLogObjectArray.GetAt(i);
                    CLogObjectLIN* pouLogObjCon = static_cast<CLogObjectLIN*> (pouLogObjBase);
                    BOOL bIsDataLog = pouLogObjCon->bLogData(CurrDataLIN);

                    if(bIsDataLog == TRUE)
                    {
                        //m_bIsThreadBlocked = FALSE;
                        m_bIsDataLogged = TRUE;
                    }
                }
            }
        }

        // Add this to the client buffer
        if (m_bClientBufferON)
        {
            m_sLINProcParams.m_pouLINBuffer->WriteIntoBuffer(&CurrMsgLIN);
        }
    }
}
HRESULT CFrameProcessor_LIN::FPL_ApplyFilters( SFILTERAPPLIED_LIN& sFilterAppliedLIN )
{
    UINT unLogCount = GetLoggingBlockCount();

    for ( USHORT i = 0; i < unLogCount; i++ )
    {
        SFILTERAPPLIED_LIN sFilterLin;
        FPL_GetFilteringScheme( i, sFilterLin );
        Filter_ReUpdateAppliedFilter( &sFilterLin, &sFilterAppliedLIN, LIN );
        //            sg_pouFrameProcCAN->FPC_StartEditingSession(); //editing is not required here
        FPL_ApplyFilteringScheme( i, sFilterLin );
        //            sg_pouFrameProcCAN->FPC_StopEditingSession(TRUE);
    }
    return S_OK;
}

/* STARTS FUNCTIONS WHOSE LOGICS ARE IMPLEMENTED IN THIS CLASS  */
// To initialise this module
HRESULT CFrameProcessor_LIN::FPL_DoInitialisation(SLINPROC_PARAMS* psInitParams)
{
    HRESULT hResult = S_FALSE;

    if (psInitParams != nullptr)
    {
        m_sLINProcParams = *psInitParams;
        ASSERT(nullptr != m_sLINProcParams.m_pouLINBuffer);
        ASSERT(nullptr != m_sLINProcParams.m_pILog);

        m_ouFSEBufLIN.vClearMessageBuffer();
        //m_sDataCopyThread.m_hActionEvent = m_ouFSEBufLIN.hGetNotifyingEvent();
        if (this->CFrameProcessor_Common::DoInitialisation() == S_OK)
        {
            if (m_pouDilLINInterface != nullptr)
            {
                hResult = S_OK;
                if (m_pouDilLINInterface->DILL_ManageMsgBuf(MSGBUF_ADD, m_sLINProcParams.dwClientID, &m_ouFSEBufLIN) != S_OK)
                {
                    hResult = S_FALSE;
                }

            }
        }
        else
        {
            ASSERT(false);
        }
    }
    else
    {
        ASSERT(false);
    }

    return hResult;
}

// To modify the filtering scheme of a logging block
/*HRESULT CFrameProcessor_LIN::FPL_ApplyFilteringScheme(USHORT ushLogBlkID,
const SFILTERAPPLIED_LIN& sFilterObj)
{
HRESULT hResult = S_FALSE;
CLogObjectLIN* pLogObj = nullptr;

if (bIsEditingON())
{
if (m_omLogListTmp.GetSize() > ushLogBlkID)
{
CBaseLogObject* pouBaseLogObj = m_omLogListTmp.GetAt(ushLogBlkID);
pLogObj = static_cast<CLogObjectLIN*> (pouBaseLogObj);
}
}
else
{
if (m_omLogObjectArray.GetSize() > ushLogBlkID)
{
CBaseLogObject* pouBaseLogObj = m_omLogObjectArray.GetAt(ushLogBlkID);
pLogObj = static_cast<CLogObjectLIN*> (pouBaseLogObj);
}
}

//update the filters
if (nullptr != pLogObj)
{
pLogObj->SetFilterInfo(sFilterObj);
hResult = S_OK;
}
else
{
ASSERT(false);
}

return hResult;
}

// Getter for the filtering scheme of a logging block
//HRESULT CFrameProcessor_LIN::FPL_GetFilteringScheme(USHORT ushLogBlk,
//SFILTERAPPLIED_LIN& sFilterObj)
{
HRESULT hResult = S_FALSE;

CBaseLogObject* pouBaseLogObj = FindLoggingBlock(ushLogBlk);
CLogObjectLIN* pouLogObj = static_cast<CLogObjectLIN*> (pouBaseLogObj);

if (nullptr != pouLogObj)
{
pouLogObj->GetFilterInfo(sFilterObj);
hResult = S_OK;
}
else
{
ASSERT(false);
}

return hResult;
}
*/

// Getter for the filtering scheme of a logging block
HRESULT CFrameProcessor_LIN::FPL_GetFilteringScheme(USHORT ushLogBlk,
        SFILTERAPPLIED_LIN& sFilterObj)
{
    HRESULT hResult = S_FALSE;

    CBaseLogObject* pouBaseLogObj = FindLoggingBlock(ushLogBlk);
    CLogObjectLIN* pouLogObj = static_cast<CLogObjectLIN*> (pouBaseLogObj);

    if (nullptr != pouLogObj)
    {
        pouLogObj->GetFilterInfo(sFilterObj);
        hResult = S_OK;
    }
    else
    {
        ASSERT(false);
    }

    return hResult;
}

// To modify the filtering scheme of a logging block
HRESULT CFrameProcessor_LIN::FPL_ApplyFilteringScheme(USHORT ushLogBlkID,
        const SFILTERAPPLIED_LIN& sFilterObj)
{
    HRESULT hResult = S_FALSE;
    CLogObjectLIN* pLogObj = nullptr;

    if (bIsEditingON())
    {
        if (m_omLogListTmp.GetSize() > ushLogBlkID)
        {
            CBaseLogObject* pouBaseLogObj = m_omLogListTmp.GetAt(ushLogBlkID);
            pLogObj = static_cast<CLogObjectLIN*> (pouBaseLogObj);
        }
    }
    else
    {
        if (m_omLogObjectArray.GetSize() > ushLogBlkID)
        {
            CBaseLogObject* pouBaseLogObj = m_omLogObjectArray.GetAt(ushLogBlkID);
            pLogObj = static_cast<CLogObjectLIN*> (pouBaseLogObj);
        }
    }

    //update the filters
    if (nullptr != pLogObj)
    {
        pLogObj->SetFilterInfo(sFilterObj);
        hResult = S_OK;
    }
    else
    {
        ASSERT(false);
    }

    return hResult;
}

HRESULT CFrameProcessor_LIN::FPL_SetClientLINBufON(bool bEnable)
{
    HRESULT hResult = S_FALSE;

    /* There is only one erroneous situation likely to occur: the client buffer
    doesn't exist and the updation of the same is to be enabled. */
    if ((bEnable == TRUE) && (nullptr == m_sLINProcParams.m_pouLINBuffer))
    {
        ;
    }
    else
    {
        m_bClientBufferON = bEnable;
        hResult = S_OK;
    }

    return hResult;
}


CBaseLINBufFSE* CFrameProcessor_LIN::FPL_GetLINBuffer(void)
{
    return ((CBaseLINBufFSE*) &m_ouFSEBufLIN);
}
/* ENDS FUNCTIONS WHOSE LOGICS ARE IMPLEMENTED IN THIS CLASS  */


void CFrameProcessor_LIN::FPL_vCloseLogFile()
{
    USHORT ushBlocks = (USHORT) (m_omLogObjectArray.GetSize());

    CBaseLogObject* pouCurrLogObj  = nullptr;
    for (USHORT i = 0; i < ushBlocks; i++)
    {
        pouCurrLogObj = m_omLogObjectArray.GetAt(i);

        if (pouCurrLogObj != nullptr)
        {
            pouCurrLogObj->bStopOnlyLogging();
            //pouCurrLogObj->vCloseLogFile();
        }
    }
}


// Empty log object
void CFrameProcessor_LIN::vEmptyLogObjArray(CLogObjArray& omLogObjArray)
{
    USHORT ushBlocks = (USHORT) (omLogObjArray.GetSize());

    if (ushBlocks > 0)
    {
        for (USHORT i = 0; i < ushBlocks; i++)
        {
            CBaseLogObject* pouCurrLogObj = omLogObjArray.GetAt(i);
            DeleteLogObj(pouCurrLogObj);
        }
    }
    omLogObjArray.RemoveAll();
}

// To update the channel baud rate info to logger
HRESULT CFrameProcessor_LIN::FPL_SetChannelBaudRateDetails
(SCONTROLLER_DETAILS_LIN* controllerDetails,
 int nNumChannels)
{
    HRESULT hResult = S_OK;
    SetChannelBaudRateDetails(controllerDetails, nNumChannels);
    return hResult;
}
void CFrameProcessor_LIN::vSetMeasurementFileName()
{
    USHORT ushBlocks = (USHORT) (m_omLogObjectArray.GetSize());
    for (USHORT i = 0; i < ushBlocks; i++)
    {
        CBaseLogObject* pouLogObjBase = m_omLogObjectArray.GetAt(i);
        CLogObjectLIN* pouLogObjCon = static_cast<CLogObjectLIN*> (pouLogObjBase);
        pouLogObjCon->m_sLogInfo.m_sLogAdvStngs.m_nConnectionCount++;
        pouLogObjCon->vSetMeasurementFileName();
    }
}