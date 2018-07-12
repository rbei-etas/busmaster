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
 * \file      MsgContainerBase.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "CommonClass_stdafx.h"
#include "MsgContainerBase.h"
#include "Utility/Utility_Thread.h"
#include "Utility/MultiLanguageSupport.h"


/******************************************************************************
    Function Name    :  DataCopyThreadProc
    Input(s)         :
    Output           :
    Functionality    :  Read thread for the messages from DIL buffer
    Member of        :  Global
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
DWORD WINAPI DataCopyThreadProc(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    if (pThreadParam == nullptr)
    {
        return 0;
    }
    CMsgContainerBase* pCurrObj = nullptr;
    if(pThreadParam != nullptr)
    {
        pCurrObj = (CMsgContainerBase*) pThreadParam->m_pBuffer;
    }
    if (pCurrObj == nullptr)
    {
        return 0;
    }

    pThreadParam->m_unActionCode = CREATE_TIME_MAP;

    bool bLoopON = true;

    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);

        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                // Retrieve message from the driver
                pCurrObj->vRetrieveDataFromBuffer();
                ResetEvent(pThreadParam->m_hActionEvent);
            }
            break;
            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;
            case CREATE_TIME_MAP:
            {
                pCurrObj->InitTimeParams();
                pThreadParam->m_unActionCode = INVOKE_FUNCTION;
                SetEvent(pThreadParam->m_hActionEvent);
            }
            break;
            default:
            case INACTION:
            {
                // nothing right at this moment
            }
            break;
        }
    }
    SetEvent(pThreadParam->hGetExitNotifyEvent());
    //Sleep(0);

    return 0;
}


CMsgContainerBase::CMsgContainerBase()
{
    m_pRxMsgCallBack  = nullptr;
    m_pouImportLogFile = nullptr;
    m_bIsFileImported=false;
}

CMsgContainerBase::~CMsgContainerBase()
{
    m_sDataCopyThread.bTerminateThread();
}

/******************************************************************************
    Function Name    :  vSetRxMsgCallBkPtr
    Input(s)         :
    Output           :
    Functionality    :  Save the call back function pointer from the User
                        for any new Rx/Tx message
    Member of        :  CMsgContainerBase
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CMsgContainerBase::vSetRxMsgCallBkPtr(IRxMsgCallBack* pFuncPtr)
{
    m_pRxMsgCallBack = pFuncPtr;
}


/**********************************************************************************
    Function Name   :   bStartReadThread
    Input(s)        :   -
    Output      :   -
    Functionality   :   -
    Member of       :   CMsgContainerBase
    Friend of       :   -
    Authors     :
    Date Created    :
    Modifications   :
************************************************************************************/
BOOL CMsgContainerBase:: bStartReadThread(HANDLE hActionEvent)
{
    BOOL bResult = TRUE;
    m_sDataCopyThread.m_pBuffer = this;
    m_sDataCopyThread.m_hActionEvent = hActionEvent;

    if ( !m_sDataCopyThread.bStartThread(DataCopyThreadProc) )
    {
        // Log error message
        bResult = FALSE;
    }
    return bResult;
}


/******************************************************************************
    Function Name    :  bStopReadThread
    Input(s)         :
    Output           :
    Functionality    :  Stop the read thread
    Member of        :  CMsgContainerBase
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
BOOL CMsgContainerBase:: bStopReadThread()
{
    BOOL bReturn = m_sDataCopyThread.bTerminateThread();
    return bReturn;
}

/**
 * \brief Copy String to char array
 *
 * String to char
 */
BOOL CMsgContainerBase::bCopyStringTocharArr( char acDesStr[], CString omSrc, int MaxDesLen )
{
    BOOL bReturn = TRUE;
    memset(acDesStr, 0, MaxDesLen);

    if (MaxDesLen <= omSrc.GetLength())
    {
        //One for null char
        omSrc = omSrc.Left(MaxDesLen - 1);
        bReturn = FALSE;
    }

    strcpy(acDesStr, omSrc.GetBuffer(MAX_PATH));
    return bReturn;
}

/*
* \breif      Import Log File
* \req        RS_FLX_04
* \design     DS_FLX_04
* \codetag    CT_FLX_04
*/

IImportLogFile* CMsgContainerBase::getLogFileImporter()
{
    return nullptr;
}

HRESULT CMsgContainerBase::IsInterpretable(ETYPE_BUS eBus, IBMNetWorkGetService* m_pBmNetWork, __int64 msgId)
{
    HRESULT hResult = S_FALSE;
    __int64 nTempMsgCode = 0;
    if (m_pBmNetWork != nullptr)
    {
        IFrame* pIFrame = nullptr;
        int nTempMsgCode = (int)(msgId & 0x1FFFFFFF);
        m_pBmNetWork->GetFrame(eBus, 0, nTempMsgCode, nullptr, &pIFrame);
        if (nullptr != pIFrame)
        {
            if (pIFrame->GetSignalCount() > 0)
            {
                hResult = S_OK;
            }
        }
    }
    return hResult;
}

HRESULT CMsgContainerBase::LoadLogFile(std::string strFileName)
{
    HRESULT hResult = S_FALSE;
    if ( nullptr == m_pouImportLogFile )
    {
        m_pouImportLogFile = getLogFileImporter();
    }
    if (nullptr == m_pouImportLogFile)
    {
        return hResult;
    }
    hResult = m_pouImportLogFile->LoadFile(strFileName);
    if(hResult == S_OK)
    {
        m_bIsFileImported = true;
        hResult = m_pouImportLogFile->SetPageLength(defDEFAULT_PAGE_LENGTH);
    }
    return hResult;
}

HRESULT CMsgContainerBase::UnloadLogFile()
{
    HRESULT hResult = S_FALSE;
    if(m_pouImportLogFile!=nullptr)
    {
        hResult = m_pouImportLogFile->UnLoadFile();
        if(hResult == S_OK)
        {
            m_bIsFileImported = false;
        }
    }
    return hResult;
}

HRESULT CMsgContainerBase::GetTotalPages(unsigned long& nPagesCount)
{
    HRESULT hResult = S_FALSE;
    if(m_pouImportLogFile!=nullptr)
    {
        hResult = m_pouImportLogFile->GetTotalPages(nPagesCount);
    }
    return hResult;
}
int CMsgContainerBase::GetPercentageLoaded()
{
    int nPercentage = 0;
    if(m_pouImportLogFile!=nullptr)
    {
        m_pouImportLogFile->GetPercentageRead(nPercentage);
    }
    return nPercentage;
}
HRESULT CMsgContainerBase::CancelFileLoad()
{
    HRESULT hResult = S_FALSE;
    if(m_pouImportLogFile!=nullptr)
    {
        hResult = m_pouImportLogFile->CancelFileLoad();
    }
    return hResult;
}

void CMsgContainerBase::setMessageAttributes(CMessageAttrib* msgAttribute)
{
    mMsgAttributes = msgAttribute;
}