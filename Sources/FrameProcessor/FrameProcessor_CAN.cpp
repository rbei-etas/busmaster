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
 * \file      FrameProcessor_CAN.cpp
 * \brief     Source file for CFrameProcessor_CAN class.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for CFrameProcessor_CAN class.
 */

#include "FrameProcessor_stdafx.h"
#include "include/Utils_macro.h"
#include "Datatypes/MsgBufAll_DataTypes.h"

#include "FrameProcessor_CAN.h"
#include "LogObjectCAN.h"


CFrameProcessor_CAN::CFrameProcessor_CAN()
{
    DIL_GetInterface(CAN, (void**)&m_pouDilCanInterface);

	m_sCANProcParams.m_pILog = NULL;
	m_sCANProcParams.m_pouCANBuffer = NULL;

    m_sDataCopyThread.m_hActionEvent = m_ouFSEBufCAN.hGetNotifyingEvent();
}

CFrameProcessor_CAN::~CFrameProcessor_CAN()
{
    vEmptyLogObjArray(m_omLogListTmp);
    vEmptyLogObjArray(m_omLogObjectArray);
}

BOOL CFrameProcessor_CAN::InitInstance(void)
{
	BOOL Result = this->CFrameProcessor_Common::InitInstance();
	//m_sDataCopyThread.m_hActionEvent = m_ouFSEBufCAN.hGetNotifyingEvent();

	return Result;
}

int CFrameProcessor_CAN::ExitInstance(void)
{
	int Result = this->CFrameProcessor_Common::ExitInstance();
	m_ouFSEBufCAN.vClearMessageBuffer();

	return Result;
}

CBaseLogObject* CFrameProcessor_CAN::CreateNewLogObj(void)
{
	CLogObjectCAN* pLogObj = new CLogObjectCAN(CString(m_sCANProcParams.m_acVersion));	
	return (static_cast<CBaseLogObject *> (pLogObj));
}

void CFrameProcessor_CAN::DeleteLogObj(CBaseLogObject*& pouLogObj)
{
    CLogObjectCAN* pLogObj = static_cast<CLogObjectCAN *> (pouLogObj);
    if (NULL != pLogObj)
    {
        delete pLogObj;
        pouLogObj = NULL;
    }
    else
    {
        ASSERT(FALSE);
    }
}

void CFrameProcessor_CAN::CreateTimeModeMapping(SYSTEMTIME& CurrSysTime, 
												UINT64& unAbsTime)
{
    if (m_pouDilCanInterface != NULL)
    {
        m_pouDilCanInterface->DILC_GetTimeModeMapping(CurrSysTime, unAbsTime);
    }
}

void CFrameProcessor_CAN::vRetrieveDataFromBuffer(void)
{
    static SFORMATTEDDATA_CAN CurrDataCAN = {0, 0, DIR_RX, CAN_CHANNEL_ALL, 0, 
            {'\0'}, TYPE_ID_CAN_NONE, TYPE_MSG_CAN_NONE, _T(" x"), {'\0'}, {'\0'}, _T(""), _T(""),_T(""), _T(""), 
            _T(""), _T(""),  _T(""),  _T(""), 0, RGB(0, 0, 0) };
    static sTCANDATA CurrMsgCAN;

    while (m_ouFSEBufCAN.GetMsgCount() > 0)
    {
        m_ouFSEBufCAN.ReadFromBuffer(&CurrMsgCAN);

        if (CurrMsgCAN.m_ucDataType != INTR_FLAG)
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
					m_ouFormatMsgCAN.m_bResetMsgAbsTime = m_bResetAbsTime;
					m_ouFormatMsgCAN.m_LogSysTime = m_LogSysTime ;
					m_bResetAbsTime = FALSE;
				}

                // Format current frame in the necessary settings
                m_ouFormatMsgCAN.vFormatCANDataMsg(&CurrMsgCAN, &CurrDataCAN, m_bExprnFlag_Log);

				USHORT ushBlocks = (USHORT) (m_omLogObjectArray.GetSize());
                for (USHORT i = 0; i < ushBlocks; i++)
                {
                    CBaseLogObject* pouLogObjBase = m_omLogObjectArray.GetAt(i);
                    CLogObjectCAN* pouLogObjCon = static_cast<CLogObjectCAN *> (pouLogObjBase);
                    pouLogObjCon->bLogData(CurrDataCAN);
                }
            }
        }

        // Add this to the client buffer
        if (m_bClientBufferON)
        {
            m_sCANProcParams.m_pouCANBuffer->WriteIntoBuffer(&CurrMsgCAN);
        }
    }
}


/* STARTS FUNCTIONS WHOSE LOGICS ARE IMPLEMENTED IN THIS CLASS  */
// To initialise this module
HRESULT CFrameProcessor_CAN::FPC_DoInitialisation(SCANPROC_PARAMS* psInitParams)
{
    HRESULT hResult = S_FALSE;

    if (psInitParams != NULL)
    {
        m_sCANProcParams = *psInitParams;
        ASSERT(NULL != m_sCANProcParams.m_pouCANBuffer);
        ASSERT(NULL != m_sCANProcParams.m_pILog);

        m_ouFSEBufCAN.vClearMessageBuffer();
        //m_sDataCopyThread.m_hActionEvent = m_ouFSEBufCAN.hGetNotifyingEvent();
        if (this->CFrameProcessor_Common::DoInitialisation() == S_OK)
        {     
            if (m_pouDilCanInterface != NULL)
            {
                if (m_pouDilCanInterface->DILC_ManageMsgBuf(MSGBUF_ADD, m_sCANProcParams.dwClientID, &m_ouFSEBufCAN) != S_OK)
                {
                    ASSERT(FALSE);
                } 
				hResult = S_OK;
            }       
        }
        else
        {
            ASSERT(FALSE);
        }
    }
    else
    {
        ASSERT(FALSE);
    }

    return hResult;
}

// To modify the filtering scheme of a logging block
HRESULT CFrameProcessor_CAN::FPC_ApplyFilteringScheme(USHORT ushLogBlkID,
                                        const SFILTERAPPLIED_CAN& sFilterObj)
{
	HRESULT hResult = S_FALSE;

    if (bIsEditingON())
    {
		if (m_omLogListTmp.GetSize() > ushLogBlkID)
	    {
            CBaseLogObject* pouBaseLogObj = m_omLogListTmp.GetAt(ushLogBlkID);
            CLogObjectCAN* pLogObj = static_cast<CLogObjectCAN *> (pouBaseLogObj);

            if (NULL != pLogObj)
            {
                pLogObj->SetFilterInfo(sFilterObj);
                hResult = S_OK;
            }
            else
            {
                ASSERT(FALSE);
            }
	    }
    }

	return hResult;
}

// Getter for the filtering scheme of a logging block
HRESULT CFrameProcessor_CAN::FPC_GetFilteringScheme(USHORT ushLogBlk, 
                                    SFILTERAPPLIED_CAN& sFilterObj)
{
	HRESULT hResult = S_FALSE;

    CBaseLogObject* pouBaseLogObj = FindLoggingBlock(ushLogBlk);
    CLogObjectCAN* pouLogObj = static_cast<CLogObjectCAN *> (pouBaseLogObj);

    if (NULL != pouLogObj)
    {
        pouLogObj->GetFilterInfo(sFilterObj);
        hResult = S_OK;
    }
    else
    {
        ASSERT(FALSE);
    }

	return hResult;
}

// To enable/disable updation of the client flexray frame buffer.
HRESULT CFrameProcessor_CAN::FPC_SetClientCANBufON(BOOL bEnable)
{
    HRESULT hResult = S_FALSE;

    /* There is only one erroneous situation likely to occur: the client buffer
    doesn't exist and the updation of the same is to be enabled. */
    if ((bEnable == TRUE) && (NULL == m_sCANProcParams.m_pouCANBuffer))
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

// To get the flexray buffer of this module
CBaseCANBufFSE* CFrameProcessor_CAN::FPC_GetCANBuffer(void)
{
    return ((CBaseCANBufFSE *) &m_ouFSEBufCAN);
}
/* ENDS FUNCTIONS WHOSE LOGICS ARE IMPLEMENTED IN THIS CLASS  */


// USE COMMON BASE CLASS ALIAS FUNCTIONS: START
/* Call to enable/disable logging for a particular block. Having ushBlk equal
to FOR_ALL, signifies the operation to be performed for all the blocks */
HRESULT CFrameProcessor_CAN::FPC_EnableLoggingBlock(USHORT ushBlk, BOOL bEnable)
{
    return EnableLoggingBlock(ushBlk, bEnable);
}

// To enable/disable logging
HRESULT CFrameProcessor_CAN::FPC_EnableLogging(BOOL bEnable)
{
    return EnableLogging(bEnable);
}

/* Call to enable/disable logging for a particular block. Having ushBlk equal
to FOR_ALL, signifies the operation to be performed for all the blocks */
HRESULT CFrameProcessor_CAN::FPC_EnableFilter(USHORT ushBlk, BOOL bEnable)
{
	return EnableFilter(ushBlk, bEnable);
}

// Query function - client flexray buffer updation status (OFF/ON)
BOOL CFrameProcessor_CAN::FPC_IsClientCANBufON(void)
{
	return IsClientBufferON();
}

// Query function - current logging status (OFF/ON). 
BOOL CFrameProcessor_CAN::FPC_IsLoggingON(void)
{
    return IsLoggingON();
}

// Query function - current filtering status
BOOL CFrameProcessor_CAN::FPC_IsFilterON(void)
{
	return IsFilterON();
}

// To log a string
HRESULT CFrameProcessor_CAN::FPC_LogString(CString& omStr)
{
	return LogString(omStr);
}

// To add a logging block; must be in editing mode
HRESULT CFrameProcessor_CAN::FPC_AddLoggingBlock(const SLOGINFO& sLogObject)
{
	return AddLoggingBlock(sLogObject);
}

// To remove a logging block by its index in the list; editing mode prerequisite
HRESULT CFrameProcessor_CAN::FPC_RemoveLoggingBlock(USHORT ushBlk)
{
    return RemoveLoggingBlock(ushBlk);
}

// Getter for total number of logging blocks
USHORT CFrameProcessor_CAN::FPC_GetLoggingBlockCount(void)
{
    return GetLoggingBlockCount();
}

// To clear the logging block list
HRESULT CFrameProcessor_CAN::FPC_ClearLoggingBlockList(void)
{
	return ClearLoggingBlockList();
}

// Getter for a logging block by specifying its index in the list
HRESULT CFrameProcessor_CAN::FPC_GetLoggingBlock(USHORT ushBlk, SLOGINFO& sLogObject)
{
    return GetLoggingBlock(ushBlk, sLogObject);
}

// Setter for a logging block by specifying its index in the list
HRESULT CFrameProcessor_CAN::FPC_SetLoggingBlock(USHORT ushBlk, 
                                const SLOGINFO& sLogObject)
{
	return SetLoggingBlock(ushBlk, sLogObject);
}

// To reset or revoke the modifications made
HRESULT CFrameProcessor_CAN::FPC_Reset(void)
{
    return Reset();
}

// To confirm the modifications made
HRESULT CFrameProcessor_CAN::FPC_Confirm(void)
{
    return Confirm();
}

// To start logging block editing session
HRESULT CFrameProcessor_CAN::FPC_StartEditingSession(void)
{
	return StartEditingSession();
}

// To stop logging block editing session
HRESULT CFrameProcessor_CAN::FPC_StopEditingSession(BOOL bConfirm)
{
	return StopEditingSession(bConfirm);
}

// Getter for the logging configuration data
HRESULT CFrameProcessor_CAN::FPC_GetConfigData(BYTE** ppvConfigData, UINT& unLength)
{
    return GetConfigData(ppvConfigData, unLength);
}

// Setter for the logging configuration data
HRESULT CFrameProcessor_CAN::FPC_SetConfigData(BYTE* pvDataStream)
{
    return SetConfigData(pvDataStream);
}

// Empty log object
void CFrameProcessor_CAN::vEmptyLogObjArray(CLogObjArray& omLogObjArray)
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

//Setter for database files associated
HRESULT CFrameProcessor_CAN::FPC_SetDatabaseFiles(const CStringArray& omList)
{
	return SetDatabaseFiles(omList);
}

// To update the channel baud rate info to logger
HRESULT CFrameProcessor_CAN::FPC_SetChannelBaudRateDetails
							(SCONTROLLER_DETAILS* controllerDetails, 
							int nNumChannels)
{
	HRESULT hResult = S_OK;
	SetChannelBaudRateDetails(controllerDetails, nNumChannels);
	return hResult;
}

// USE COMMON BASE CLASS ALIAS FUNCTIONS: END
