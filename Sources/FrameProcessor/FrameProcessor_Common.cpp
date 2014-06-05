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
 * \file      FrameProcessor_Common.cpp
 * \brief     Source file for CFrameProcessor_Common class realising the
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for CFrameProcessor_Common class realising the
 */


// FrameProcessor.cpp : Defines the initialization routines for the DLL.
//

#include "FrameProcessor_stdafx.h"
#include "include/Utils_macro.h"
#include "FrameProcessor_Common.h"
#include "CommonClass/RefTimeKeeper.h"
#include "include/XMLDefines.h"

/* Log version..applicable for log files from ver 1.6.2 */
#define VERSION_CURR    0x02
/* Log version..applicable for log files before ver 1.6.2 */
/*#define VERSION_CURR    0x01*/

// CFrameProcessor_Common


DWORD WINAPI DataCopyThreadProc(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    if (pThreadParam == nullptr)
    {
        // TBD
    }
    CFrameProcessor_Common* pCurrObj = nullptr;
    if(pThreadParam != nullptr)
    {
        pCurrObj = (CFrameProcessor_Common*) pThreadParam->m_pBuffer;
    }
    if (pCurrObj == nullptr)
    {
        // TBD
    }

    //pThreadParam->m_unActionCode = INACTION;

    bool bLoopON = true;

    while (bLoopON)
    {
        if(pCurrObj != nullptr)
        {
            pCurrObj->m_bIsThreadBlocked = TRUE;
        }

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
                pCurrObj->m_bIsThreadBlocked = FALSE;
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

    return 0;
}

// CFrameProcessor_Common construction

CFrameProcessor_Common::CFrameProcessor_Common()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
    m_bLogEnabled = FALSE;
    m_bFilterON = FALSE;
    m_bClientBufferON = TRUE;
    m_ushLastBlkID = 0;
    m_bEditingON = FALSE;
    m_bIsDataLogged = FALSE;
    m_bIsJ1939DataLogged = FALSE;
    m_bIsLINDataLogged = FALSE;
}

CFrameProcessor_Common::~CFrameProcessor_Common()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

HRESULT CFrameProcessor_Common::DoInitialisation(void)
{
    m_sDataCopyThread.m_unActionCode = CREATE_TIME_MAP;

    return S_OK;
}

void CFrameProcessor_Common::vCopyLogObjArray(
    CLogObjArray& omLogObjArrayTarget, const CLogObjArray& omLogObjArraySrc)
{
    // First empty the target log object array
    vEmptyLogObjArray(omLogObjArrayTarget);

    USHORT ushBlocks = (USHORT) (omLogObjArraySrc.GetSize());

    if (ushBlocks > 0)
    {
        for (USHORT i = 0; i < ushBlocks; i++)
        {
            const CBaseLogObject* pouCurrLogObj = omLogObjArraySrc.GetAt(i);
            CBaseLogObject* pouNewLogObj = CreateNewLogObj(m_omStrVersion);
            *pouNewLogObj = *pouCurrLogObj;
            omLogObjArrayTarget.Add(pouNewLogObj);
        }
    }
}

// CFrameProcessor_Common initialization

BOOL CFrameProcessor_Common::InitInstance(void)
{
    m_sDataCopyThread.m_pBuffer = (LPVOID) this;
    m_sDataCopyThread.m_unActionCode = INACTION;
    BOOL Result = m_sDataCopyThread.bStartThread(DataCopyThreadProc);

    return Result;
}

int CFrameProcessor_Common::ExitInstance(void)
{
    // TODO: Add your specialized code here and/or call the base class
    m_sDataCopyThread.bTerminateThread();

    return 0;
}


/* Start of helper functions in CFrameProcessor_Common */

UINT CFrameProcessor_Common::unGetBufSize(void)
{
    UINT unBufSize = sizeof(BYTE);  // Version information
    unBufSize += sizeof(USHORT);    // Logging blocks

    USHORT ushLogBlks = GetLoggingBlockCount();
    for (USHORT i = 0; i < ushLogBlks; i++)
    {
        CBaseLogObject* pouCurrLogObj = FindLoggingBlock(i);
        if (nullptr != pouCurrLogObj)
        {
            unBufSize += pouCurrLogObj->unGetBufSize();
        }
    }

    return unBufSize;
}


void CFrameProcessor_Common::InitTimeParams(void)
{
    SYSTEMTIME CurrSysTime;
    UINT64 unAbsTime;
    CreateTimeModeMapping(CurrSysTime, unAbsTime);
    m_ouRefTimer.vSetTimeParams(CurrSysTime, unAbsTime);
}


void CFrameProcessor_Common::vUpdateLoggingFlag(void)
{
    BYTE* pbCurrFlag = m_bEditingON ? &m_bLogFlagTmp : &m_bExprnFlag_Log;

    CLEAR_EXPR_FLAG(*pbCurrFlag);

    USHORT ushLogBlocks = GetLoggingBlockCount();

    for (USHORT i = 0; i < ushLogBlocks; i++)
    {
        SLOGINFO sLogInfo;
        GetLoggingBlock(i, sLogInfo);
        if (sLogInfo.m_bEnabled == FALSE)
        {
            continue;
        }

        BYTE* pbResFlag = sLogInfo.m_bResetAbsTimeStamp ? &m_bLogFlagTmp : &m_bExprnFlag_Log;

        switch (sLogInfo.m_eLogTimerMode)
        {
            case TIME_MODE_ABSOLUTE:
                SET_TM_ABS(*pbCurrFlag);
                SET_TM_ABS_RES(*pbResFlag);
                break;

            case TIME_MODE_RELATIVE:
                SET_TM_REL(*pbCurrFlag);
                break;

            case TIME_MODE_SYSTEM:
                SET_TM_SYS(*pbCurrFlag);
                break;
        }

        switch (sLogInfo.m_eNumFormat)
        {
            case HEXADECIMAL:
                SET_NUM_HEX(*pbCurrFlag);
                break;

            case DEC:
                SET_NUM_DEC(*pbCurrFlag);
                break;
        }
    }
}

CBaseLogObject* CFrameProcessor_Common::FindLoggingBlock(USHORT ushID)
{
    CLogObjArray* pomCurrArray = GetActiveLogObjArray();

    CBaseLogObject* Result = nullptr;
    USHORT Blocks = (USHORT) (pomCurrArray->GetSize());
    if (Blocks > ushID)
    {
        CBaseLogObject* pouCurrLogObj = pomCurrArray->GetAt(ushID);
        Result = pouCurrLogObj;
    }

    return Result;
}

USHORT CFrameProcessor_Common::GetUniqueID(void)
{
    USHORT Result = ID_INVALID;

#if 0
    int Blocks = (int) (m_omLogObjectArray.GetCount());
    bool bNewIDFound = false;

    USHORT CurrID = 0;
    while ((CurrID < ID_MAX) && (false == bNewIDFound))
    {
        bNewIDFound = true; // Assuming this ID is unique, keep validating
        for (int i = 0; (i < Blocks) && (true == bNewIDFound); i++)
        {
            CLogObject& ouCurrLogObj = m_omLogObjectArray.GetAt(i);
            if (ouCurrLogObj.GetID() == CurrID)
            {
                bNewIDFound = false; // This ID is already taken. Try another
                CurrID++;            // one and break the loop.
            }
        }
    }
    if (bNewIDFound == true)
    {
        Result = CurrID; // Result of the function
    }
#endif

    return Result;
}

bool CFrameProcessor_Common::bIsEditingON(void)
{
    return m_bEditingON;
}

CLogObjArray* CFrameProcessor_Common::GetActiveLogObjArray(void)
{
    CLogObjArray* RetVal = m_bEditingON ? &m_omLogListTmp : &m_omLogObjectArray;
    return RetVal;
}

void CFrameProcessor_Common::vCloseLogFile()
{
    USHORT ushBlocks = (USHORT) (m_omLogObjectArray.GetSize());

    CBaseLogObject* pouCurrLogObj = nullptr;
    for (USHORT i = 0; i < ushBlocks; i++)
    {
        pouCurrLogObj = m_omLogObjectArray.GetAt(i);

        if (nullptr == pouCurrLogObj)
        {
            pouCurrLogObj->vCloseLogFile();
        }
    }
}

/* End of helper functions in CFrameProcessor_Common */

/* Start of alias functions in CFrameProcessor_Common */

HRESULT CFrameProcessor_Common::EnableLogging(bool bEnable, ETYPE_BUS eBus)
{
    HRESULT hResult = S_FALSE;

    /* Modus operandi: If logging is to be enabled, then first perform the initial
    tasks and then assign TRUE to m_bLogEnabled. If logging is to be disabled,
    first assign FALSE to m_bLogEnabled and then only perform the other tasks. Else
    crash / unexpected behaviour may result */
    if(eBus == CAN)
    {
        m_bIsDataLogged = FALSE;
    }
    else if(eBus == J1939)
    {
        m_bIsJ1939DataLogged = FALSE;
    }
    else if(eBus == LIN)
    {
        m_bIsLINDataLogged = FALSE;
    }

    USHORT ushBlocks = (USHORT) (m_omLogObjectArray.GetSize());

    //update reset flag
    m_bResetAbsTime = bEnable;
    GetLocalTime(&m_LogSysTime);

    if (ushBlocks > 0)
    {
        if (bEnable == false)
        {
            m_bLogEnabled = bEnable;

            if(eBus == CAN)
            {
                m_bIsDataLogged = FALSE;
            }
            else if(eBus == J1939)
            {
                m_bIsJ1939DataLogged = FALSE;
            }

            else if(eBus == LIN)
            {
                m_bIsLINDataLogged = FALSE;
            }
        }
        for (USHORT i = 0; i < ushBlocks; i++)
        {
            CBaseLogObject* pouCurrLogObj = m_omLogObjectArray.GetAt(i);

            if (bEnable)
            {
                pouCurrLogObj->bStartLogging(eBus);
            }
            else
            {
                pouCurrLogObj->bStopLogging();
            }
        }
        if (bEnable)
        {
            m_bLogEnabled = bEnable;
            m_sDataCopyThread.m_unActionCode = CREATE_TIME_MAP;
        }

        hResult = S_OK;
    }
    else
    {
        m_bLogEnabled = FALSE; //desable the control
        if(eBus == CAN)
        {
            m_bIsDataLogged = FALSE;
        }
        else if(eBus == J1939)
        {
            m_bIsJ1939DataLogged = FALSE;
        }

        else if(eBus == LIN)
        {
            m_bIsLINDataLogged = FALSE;
        }
    }
    return hResult;
}

HRESULT CFrameProcessor_Common::EnableFilter(USHORT ushBlk, bool bEnable)
{
    HRESULT hResult = S_OK;
    // Check if at least one filtering datum is available.
    // If so, return S_OK else S_FALSE
    m_bFilterON = bEnable;

    USHORT ushLogBlks = (USHORT) (m_omLogObjectArray.GetSize());

    for (USHORT i = 0; i < ushLogBlks; i++)
    {
        CBaseLogObject* pouCurrLogObj = m_omLogObjectArray.GetAt(i);
        if (FOR_ALL == ushBlk)
        {
            pouCurrLogObj->EnableFilter(bEnable);
        }
        else
        {
            if (ushBlk == i)
            {
                pouCurrLogObj->EnableFilter(bEnable);
                break;
            }
        }
    }

    return hResult;
}

USHORT CFrameProcessor_Common::GetLoggingBlockCount(void)
{
    return (USHORT) (GetActiveLogObjArray()->GetSize());
}

HRESULT CFrameProcessor_Common::ClearLoggingBlockList(void)
{
    if (m_bEditingON)
    {
        vEmptyLogObjArray(m_omLogListTmp);
        CLEAR_EXPR_FLAG(m_bLogFlagTmp);
    }
    else
    {
        vEmptyLogObjArray(m_omLogObjectArray);
        CLEAR_EXPR_FLAG(m_bExprnFlag_Log);
    }

    return S_OK;
}

HRESULT CFrameProcessor_Common::GetLoggingBlock(USHORT ushBlk, SLOGINFO& sLogObject)
{
    HRESULT hResult = S_FALSE;

    CBaseLogObject* pouLogObj = FindLoggingBlock(ushBlk);

    if (pouLogObj != nullptr)
    {
        pouLogObj->GetLogInfo(sLogObject);
        hResult = S_OK;
    }

    return hResult;
}

HRESULT CFrameProcessor_Common::SetLoggingBlock(USHORT ushBlk, const SLOGINFO& sLogObject)
{
    HRESULT hResult = S_FALSE;

    if (bIsEditingON())
    {
        CBaseLogObject* pouLogObj = FindLoggingBlock(ushBlk);

        if (pouLogObj != nullptr)
        {
            pouLogObj->SetLogInfo(sLogObject);
            hResult = S_OK;
        }
    }

    return hResult;
}

// Getter for the logging configuration data
HRESULT CFrameProcessor_Common::GetConfigData(BYTE** ppvConfigData, UINT& unLength)
{
    BYTE* pbBuff = new BYTE[unGetBufSize()];
    *ppvConfigData = pbBuff;

    CLogObjArray* pomCurrArray = GetActiveLogObjArray();

    BYTE bVersion = VERSION_CURR;
    COPY_DATA(pbBuff, &bVersion, sizeof(bVersion));

    USHORT ushLogBlks = (USHORT) (pomCurrArray->GetSize());

    COPY_DATA(pbBuff, &ushLogBlks, sizeof(ushLogBlks));

    for (USHORT i = 0; i < ushLogBlks; i++)
    {
        CBaseLogObject* pouLogObj = pomCurrArray->GetAt(i);
        if (nullptr != pouLogObj)
        {
            pbBuff = pouLogObj->GetConfigData(pbBuff);
        }
        else
        {
            ASSERT(false);
        }
    }

    unLength = unGetBufSize();

    return S_OK;
}

HRESULT CFrameProcessor_Common::GetConfigData(xmlNodePtr pxmlNodePtr)
{
    /*BYTE* pbBuff = new BYTE[unGetBufSize()];
    *ppvConfigData = pbBuff;*/

    CLogObjArray* pomCurrArray = GetActiveLogObjArray();

    BYTE bVersion = VERSION_CURR;
    //COPY_DATA(pbBuff, &bVersion, sizeof(bVersion));

    USHORT ushLogBlks = (USHORT) (pomCurrArray->GetSize());

    //COPY_DATA(pbBuff, &ushLogBlks, sizeof(ushLogBlks));

    for (USHORT i = 0; i < ushLogBlks; i++)
    {
        xmlNodePtr pNodeLogBlckPtr = xmlNewNode(nullptr, BAD_CAST DEF_LOG_BLOCK);
        xmlAddChild(pxmlNodePtr, pNodeLogBlckPtr);

        CBaseLogObject* pouLogObj = pomCurrArray->GetAt(i);

        if (nullptr != pouLogObj)
        {
            pouLogObj->GetConfigData(pNodeLogBlckPtr);
        }
        else
        {
            ASSERT(false);
        }
    }

    // unLength = unGetBufSize();

    return S_OK;
}

// Setter for the logging configuration data
HRESULT CFrameProcessor_Common::SetConfigData(BYTE* pvDataStream, const CString& omStrVersion)
{
    if (bIsEditingON() == false)
    {
        return S_FALSE;
    }

    ClearLoggingBlockList();

    BYTE* pbBuff = pvDataStream;
    BYTE bVersion = 0;
    USHORT ushLogBlks = 0;

    COPY_DATA_2(&bVersion, pbBuff, sizeof(bVersion));
    COPY_DATA_2(&ushLogBlks, pbBuff, sizeof(ushLogBlks));

    m_omStrVersion = omStrVersion;
    for (USHORT i = 0; i < ushLogBlks; i++)
    {
        CBaseLogObject* pouBaseLogObj = CreateNewLogObj(m_omStrVersion);
        pbBuff = pouBaseLogObj->SetConfigData(pbBuff, bVersion);
        m_omLogListTmp.Add(pouBaseLogObj);
    }

    return S_OK;
}

HRESULT CFrameProcessor_Common::SetConfigData( xmlDocPtr pDoc, ETYPE_BUS eBus)
{
    if (bIsEditingON() == false)
    {
        return S_FALSE;
    }

    ClearLoggingBlockList();



    USHORT ushLogBlks = 0;
    xmlChar* pchXpath = nullptr;
    xmlXPathObjectPtr pXpathPtr = nullptr;

    if(eBus == CAN)
    {
        pchXpath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Log/Log_Block";
    }
    else if(eBus == J1939)
    {
        pchXpath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/J1939_Log/Log_Block";
    }
    else if(eBus == LIN)
    {
        pchXpath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/LIN_Log/Log_Block";
    }
    pXpathPtr = xmlUtils::pGetNodes(pDoc, pchXpath);

    if ( pXpathPtr != nullptr)
    {
        xmlNodeSetPtr pNodeSet = pXpathPtr->nodesetval;
        if(pNodeSet != nullptr)
        {
            ushLogBlks = pNodeSet->nodeNr;
            for(int i = 0; i < ushLogBlks; i++)
            {
                CBaseLogObject* pouBaseLogObj = CreateNewLogObj(m_omStrVersion);
                if ( S_OK == pouBaseLogObj->nSetConfigData(pNodeSet->nodeTab[i]))
                {
                    vAddLogFile(m_omLogListTmp, pouBaseLogObj);
                    //m_omLogListTmp.Add(pouBaseLogObj);
                }
            }
        }

    }
    return S_OK;
}

void CFrameProcessor_Common::vAddLogFile( CLogObjArray& omLogListTmp, CBaseLogObject*& pouBaseLogObj)
{
    int nCount = omLogListTmp.GetSize();
    BOOL bDuplicate = FALSE;
    if(nCount == 0)
    {
        omLogListTmp.Add(pouBaseLogObj);
    }
    else
    {
        for(int i = 0; i < nCount; i++)
        {
            CBaseLogObject* pBaseObjectSrc = omLogListTmp.GetAt(i);
            SLOGINFO sLogInfo1, sLogInfo2;
            pouBaseLogObj->GetLogInfo(sLogInfo1);
            pBaseObjectSrc->GetLogInfo(sLogInfo2);
            //TODO::CString Has to be Remodved
            CString omStr1 = sLogInfo1.m_sLogFileName;
            CString omStr2 = sLogInfo2.m_sLogFileName;
            omStr1.TrimLeft(" \n\t");
            omStr1.TrimRight(" \n\t");
            omStr2.TrimLeft(" \n\t");
            omStr2.TrimRight(" \n\t");
            if(omStr1 == omStr2)
            {
                bDuplicate = TRUE;
            }
        }
        if(bDuplicate == FALSE)
        {
            omLogListTmp.Add(pouBaseLogObj);
        }
    }
}

bool CFrameProcessor_Common::IsClientBufferON(void)
{
    return m_bClientBufferON;
}

HRESULT CFrameProcessor_Common::LogString(CString& omStr)
{
    USHORT ushLogBlks = (USHORT) (m_omLogObjectArray.GetSize());

    for (USHORT i = 0; i < ushLogBlks; i++)
    {
        CBaseLogObject* pouCurrLogObj = m_omLogObjectArray.GetAt(i);
        pouCurrLogObj->bLogString(omStr);
    }

    return S_OK;
}

bool CFrameProcessor_Common::IsLoggingON(void)
{
    return m_bLogEnabled;
}

bool CFrameProcessor_Common::IsDataLogged(void)
{
    return m_bIsDataLogged;
}

bool CFrameProcessor_Common::IsLINDataLogged(void)
{
    return m_bIsLINDataLogged;
}

bool CFrameProcessor_Common::IsJ1939DataLogged(void)
{
    return m_bIsJ1939DataLogged;
}

bool CFrameProcessor_Common::IsThreadBlocked(void)
{
    return m_bIsThreadBlocked;
}

void CFrameProcessor_Common::DisableDataLogFlag(void)
{
    m_bIsDataLogged = FALSE;
}

void CFrameProcessor_Common::DisableJ1939DataLogFlag(void)
{
    m_bIsJ1939DataLogged = FALSE;
}

void CFrameProcessor_Common::DisableLINDataLogFlag(void)
{
    m_bIsLINDataLogged = FALSE;
}

bool CFrameProcessor_Common::IsFilterON(void)
{
    return m_bFilterON;
}

HRESULT CFrameProcessor_Common::EnableLoggingBlock(USHORT ushBlk, bool bEnable)
{
    HRESULT hResult = S_FALSE;

    CBaseLogObject* pouLogObj = FindLoggingBlock(ushBlk);

    if (nullptr != pouLogObj)
    {
        if (pouLogObj->IsLoggingEnable() != bEnable)
        {
            pouLogObj->EnableLogging(bEnable);
            hResult = S_OK;
        }
    }

    if (S_OK == hResult)
    {
        vUpdateLoggingFlag();
    }

    return hResult;
}

HRESULT CFrameProcessor_Common::AddLoggingBlock(const SLOGINFO& sLogObject)
{
    HRESULT hResult = S_FALSE;

    if (bIsEditingON())
    {
        CBaseLogObject* pouCurrLogBlk = CreateNewLogObj(m_omStrVersion);
        if (nullptr != pouCurrLogBlk)
        {
            pouCurrLogBlk->SetLogInfo(sLogObject);
            m_omLogListTmp.Add(pouCurrLogBlk);
            hResult = S_OK;
        }
        else
        {
            ASSERT(false);
        }
    }

    return hResult;
}

HRESULT CFrameProcessor_Common::RemoveLoggingBlock(USHORT ushBlk)
{
    HRESULT hResult = S_FALSE;

    //if (bIsEditingON())
    //{
    if (FindLoggingBlock(ushBlk) != nullptr)
    {
        GetActiveLogObjArray()->RemoveAt(ushBlk);
        hResult = S_OK;
    }
    //}

    return hResult;
}

HRESULT CFrameProcessor_Common::Reset(void)
{
    vEmptyLogObjArray(m_omLogListTmp);
    //m_omLogListTmp.Copy(m_omLogObjectArray);
    vCopyLogObjArray(m_omLogListTmp, m_omLogObjectArray);
    m_bLogFlagTmp = m_bExprnFlag_Log;

    return S_OK;
}

HRESULT CFrameProcessor_Common::Confirm(void)
{
    HRESULT hResult = S_FALSE;

    if (bIsEditingON())
    {
        vUpdateLoggingFlag(); // First ensure m_bLogFlagTmp is updated
        vEmptyLogObjArray(m_omLogObjectArray);
        //m_omLogObjectArray.Copy(m_omLogListTmp);
        vCopyLogObjArray(m_omLogObjectArray, m_omLogListTmp);
        m_bExprnFlag_Log = m_bLogFlagTmp; // Then only assign value to log flag
        hResult = S_OK;
    }

    return hResult;
}

HRESULT CFrameProcessor_Common::StartEditingSession(void)
{
    Reset();
    m_bEditingON = TRUE;

    return S_OK;
}

HRESULT CFrameProcessor_Common::StopEditingSession(bool bConfirm)
{
    HRESULT hResult = S_OK; // Success is default assumption

    if (bIsEditingON())     // Proceed only when editing is going on
    {
        if (bConfirm)       // Confirm changes and stop the session
        {
            hResult = Confirm();
        }
        if (S_OK == hResult) // Wrap up only if everything is successful
        {
            vEmptyLogObjArray(m_omLogListTmp); // Clean the temporary repository
            m_bEditingON = FALSE;
        }
    }
    else
    {
        hResult = S_FALSE; // Not supposed to call otherwise
    }

    return hResult;
}

HRESULT CFrameProcessor_Common::SetDatabaseFiles(const CStringArray& omList)
{
    HRESULT hResult = S_OK; // Success is default assumption

    CLogObjArray* pomCurrArray = GetActiveLogObjArray();
    if (nullptr != pomCurrArray)
    {
        for (int nIdx = 0; nIdx < pomCurrArray->GetSize(); nIdx++)
        {
            CBaseLogObject* pouCurrLogObj = pomCurrArray->GetAt(nIdx);

            if (nullptr != pouCurrLogObj)
            {
                pouCurrLogObj->Der_SetDatabaseFiles(omList);
            }
        }
    }

    return hResult;
}

void CFrameProcessor_Common::GetDatabaseFiles(CStringArray & /* omList */)
{
}

void CFrameProcessor_Common::SetChannelBaudRateDetails(void* controllerDetails,
    int nNumChannels, ETYPE_BUS /* eBus */)
{
    CLogObjArray* pomCurrArray = GetActiveLogObjArray();
    if (nullptr != pomCurrArray)
    {
        for (int nIdx = 0; nIdx < pomCurrArray->GetSize(); nIdx++)
        {
            CBaseLogObject* pouCurrLogObj = pomCurrArray->GetAt(nIdx);

            if (nullptr != pouCurrLogObj)
            {
                pouCurrLogObj->Der_SetChannelBaudRateDetails(controllerDetails,
                        nNumChannels);
            }
        }
    }
}

/* End of alias functions in CFrameProcessor_Common */
