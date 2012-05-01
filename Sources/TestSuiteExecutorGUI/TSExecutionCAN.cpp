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
 * \file      TSExecutionCAN.cpp
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "TSExecutorGUI_stdafx.h"
#include "TSExecutionCAN.h"
#include "Include/Can_Error_Defs.h"
#include "DIL_Interface/DIL_Interface_Extern.h"


/******************************************************************************
Function Name  :  ReadTSXDataBuffer
Input(s)       :  CTSExecutionCAN* pTSXCan - CAN Executor Object
Output         :  int
Functionality  :  Read Thread
Member of      :  -
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
int ReadTSXDataBuffer(CTSExecutionCAN* pTSXCan)
{
    ASSERT(pTSXCan != NULL);

    while (pTSXCan->m_ouCanBufFSE.GetMsgCount() > 0)
    {
        static STCANDATA sCanData;
        sCanData.m_lTickCount.QuadPart;
        int Result = pTSXCan->m_ouCanBufFSE.ReadFromBuffer(&sCanData);

        if (Result == ERR_READ_MEMORY_SHORT)
        {
            TRACE(_T("ERR_READ_MEMORY_SHORT"));
        }
        else if (Result == EMPTY_APP_BUFFER)
        {
            TRACE(_T("EMPTY_APP_BUFFER"));
        }
        else
        {
            INT nIndex;
            pTSXCan->m_ouCanBufVFSE.WriteIntoBuffer(&sCanData, sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID, nIndex);
        }
    }

    return 0;
}
HRESULT VerifyCurrentMessage(STCANDATA& sCanData, CTSExecutionCAN* pTSXCan)
{
    AfxTrace("In VERIFY - Before Enter\n");
    EnterCriticalSection(&(pTSXCan->m_omCritSecTS));
    AfxTrace("In VERIFY - After Enter\n");

    if(pTSXCan->m_bTimeOver == TRUE || pTSXCan->m_pCurrentVerify == NULL)
    {
        LeaveCriticalSection(&(pTSXCan->m_omCritSecTS));
        return S_FALSE;
    }

    UINT unCount;
    pTSXCan->m_pCurrentVerify->GetSubEntryCount(unCount);

    if(pTSXCan->m_MsgVerifiedList.GetCount() < (INT)unCount)
    {
        CVerify_MessageData ouMsgData;

        if(pTSXCan->m_pCurrentVerify->GetMessageFromId(sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID, &ouMsgData) == S_OK)
        {
            if (pTSXCan->m_MsgVerifiedList.Find(sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID) == NULL)
            {
                UCHAR pucData[8] = {0};
                memcpy(pucData, &sCanData.m_uDataInfo.m_sCANMsg.m_ucData, sCanData.m_uDataInfo.m_sCANMsg.m_ucDataLen);
                CSignalInfoArray ouSignalInfo;
                //Interprete The Messages
                sMESSAGE sMsg;
                SMSGENTRY* sMsgEntry = new SMSGENTRY;
                pTSXCan->m_pCurrentVerify->m_ouDataBaseManager.nGetMessageInfo(ouMsgData.m_omMessageName, sMsg);
                sMsgEntry->m_psMsg = &sMsg;
                sMsgEntry->m_psNext = NULL;
                pTSXCan->m_ouMsgInterpret.vSetMessageList(sMsgEntry);
                pTSXCan->m_ouMsgInterpret.bInterpretMsgs(ouMsgData.m_dwMessageID, pucData, ouSignalInfo);
                CMessageResult ouMsgResult;

                if( pTSXCan->bVerifyCanMessage(ouMsgData, ouSignalInfo, ouMsgResult) == TRUE)
                {
                    //pTSXCan->m_nVerifyCount++;
                    if(pTSXCan->m_ouVerifyResult != NULL && pTSXCan->m_bTimeOver == FALSE)
                    {
                        //pTSXCan->TSX_DisplayMessage(ouMsgData.m_omMessageName);
                        //pTSXCan->TSX_DisplayResult(CString("SUCCESS"));
                        pTSXCan->m_ouVerifyResult->m_MessageResultList.AddTail(ouMsgResult);
                    }

                    pTSXCan->m_MsgVerifiedList.AddTail((UINT&)ouMsgData.m_dwMessageID);
                }
            }
        }
    }

    if(pTSXCan->m_MsgVerifiedList.GetCount() == (INT)unCount)
    {
        //pTSXCan->m_nVerifyCount = 0;
        pTSXCan->m_pCurrentVerify = NULL;
        pTSXCan->m_ouVerifyEvent.SetEvent();
        //pTSXCan->m_ouReadThread.m_unActionCode = INVOKE_FUNC;
    }

    LeaveCriticalSection(&(pTSXCan->m_omCritSecTS));
    return S_OK;
}
int ReadVerifyTSXDataBuffer(CTSExecutionCAN* pTSXCan)
{
    ASSERT(pTSXCan != NULL);

    while (pTSXCan->m_ouCanBufFSE.GetMsgCount() > 0)
    {
        static STCANDATA sCanData;
        sCanData.m_lTickCount.QuadPart;
        int Result = pTSXCan->m_ouCanBufFSE.ReadFromBuffer(&sCanData);

        if (Result == ERR_READ_MEMORY_SHORT)
        {
            TRACE(_T("ERR_READ_MEMORY_SHORT"));
        }
        else if (Result == EMPTY_APP_BUFFER)
        {
            TRACE(_T("EMPTY_APP_BUFFER"));
        }
        else
        {
            INT nIndex;
            pTSXCan->m_ouCanBufVFSE.WriteIntoBuffer(&sCanData, sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID, nIndex);
        }

        if(IS_RX_MESSAGE(sCanData.m_ucDataType))
        {
            pTSXCan->m_LastCanMsg = sCanData.m_lTickCount.QuadPart;
            VerifyCurrentMessage(sCanData, pTSXCan);
        }
    }

    return 0;
}
/******************************************************************************
Function Name  :  TSDataReadThreadProc
Input(s)       :  LPVOID pVoid - Thread Parameter
Output         :  DWORD - S_OK if Success otherwise S_FALSE
Functionality  :  Starts The Read Thread
Member of      :  -
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
DWORD WINAPI TSDataReadThreadProc(LPVOID pVoid)
{
    USES_CONVERSION;
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;

    if (pThreadParam == NULL)
    {
        return ((DWORD)-1);
    }

    CTSExecutionCAN* pTSXExecutionCAN = (CTSExecutionCAN*)pThreadParam->m_pBuffer;

    if (pTSXExecutionCAN == NULL)
    {
        return ((DWORD)-1);
    }

    bool bLoopON = true;
    pThreadParam->m_unActionCode = INVOKE_FUNCTION;

    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);

        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                ReadTSXDataBuffer(pTSXExecutionCAN); // Retrieve message from the driver
            }
            break;

            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;

            case CREATE_TIME_MAP:
            {
            }
            break;

            case INACTION:
            {
                ReadVerifyTSXDataBuffer(pTSXExecutionCAN);
                // nothing right at this moment
            }
            break;

            default:
            {
            }
            break;
        }
    }

    SetEvent(pThreadParam->hGetExitNotifyEvent());
    return 0;
}

/******************************************************************************
Function Name  :  CTSExecutionCAN
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CTSExecutionCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
CTSExecutionCAN::CTSExecutionCAN(void)
{
    m_ouReadThread.m_hActionEvent = m_ouCanBufFSE.hGetNotifyingEvent();
    InitializeCriticalSection(&m_omCritSecTS);
    m_pCurrentVerify= NULL;
    m_ouVerifyEvent.ResetEvent();
    m_nVerifyCount = 0;
    m_ouVerifyResult = NULL;
    QueryPerformanceFrequency(&m_QueryFrequency);
}

/******************************************************************************
Function Name  :  ~CTSExecutionCAN
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CTSExecutionCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
CTSExecutionCAN::~CTSExecutionCAN(void)
{
    m_ouReadThread.bTerminateThread();      // Terminate read thread
    m_ouCanBufVFSE.vClearMessageBuffer();    // clear can buffer
}

/******************************************************************************
Function Name  :  TSX_DoInitialization
Input(s)       :  -
Output         :  HRESULT
Functionality  :  Registers to CAN DIL
Member of      :  CTSExecutionCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
HRESULT CTSExecutionCAN::TSX_DoInitialization(void)
{
    if (DIL_GetInterface(CAN, (void**)&m_pouDIL_CAN) == S_OK)
    {
        m_pouDIL_CAN->DILC_RegisterClient(TRUE, m_dwClientId, CAN_MONITOR_NODE);
        m_pouDIL_CAN->DILC_ManageMsgBuf(MSGBUF_ADD, m_dwClientId, &m_ouCanBufFSE);
        return S_OK;
    }

    return S_FALSE;
}

/******************************************************************************
Function Name  :  TSX_bStartStopReadThread
Input(s)       :  BOOL - Boolean Value to start and stop thread
Output         :  HRESULT
Functionality  :  Starts or stops the Reeading thread
Member of      :  CTSExecutionCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
HRESULT CTSExecutionCAN::TSX_bStartStopReadThread(BOOL bStart)
{
    m_ouReadThread.bTerminateThread();
    m_ouReadThread.m_hActionEvent = NULL;
    m_ouReadThread.m_unActionCode = IDLE;

    if(bStart == TRUE)
    {
        //First stop the thread if running
        m_ouReadThread.m_pBuffer = this;
        m_ouReadThread.m_hActionEvent = m_ouCanBufFSE.hGetNotifyingEvent();
        m_ouReadThread.bStartThread(TSDataReadThreadProc);
        TSX_Reset();
    }

    return S_OK;
}

/******************************************************************************
Function Name  :  TSX_Reset
Input(s)       :  -
Output         :  HRESULT
Functionality  :  Resets the Message Buffer
Member of      :  CTSExecutionCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
HRESULT CTSExecutionCAN::TSX_Reset(void)
{
    m_ouCanBufVFSE.vClearMessageBuffer();
    return S_OK;
}

/******************************************************************************
Function Name  :  TSX_VerifyResponse
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CTSExecutionCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  25/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutionCAN::TSX_VerifyResponse(CBaseEntityTA* pEntity, CResultVerify& ouVerifyResult)
{
    m_pCurrentVerify = (CVerifyResponse*)pEntity;
    m_ouVerifyEvent.ResetEvent();
    m_ouVerifyResult = &ouVerifyResult;
    m_MsgVerifiedList.RemoveAll();
    m_nVerifyCount = 0;
    m_ouReadThread.m_unActionCode = INACTION;
    m_bTimeOver = FALSE;
    m_LastCanMsg = (UINT)-1;
    LARGE_INTEGER Tick1;
    QueryPerformanceCounter(&Tick1);
    //Wait For Specified duration
    HRESULT hResult = WaitForSingleObject(m_ouVerifyEvent, m_pCurrentVerify->m_ushDuration);
    m_ouReadThread.m_unActionCode = INVOKE_FUNCTION;
    EnterCriticalSection(&m_omCritSecTS);
    m_bTimeOver = TRUE;
    LeaveCriticalSection(&m_omCritSecTS);
    HRESULT hResultTC;

    if(hResult == WAIT_OBJECT_0)
    {
        CString strResult(_T("SUCCESS"));
        TSX_DisplayResult(strResult);
        hResultTC = S_OK;
    }
    else
    {
        //time calculation
        LARGE_INTEGER Tickr;
        UINT64 tr;
        SYSTEMTIME sysTime;
        m_pouDIL_CAN->DILC_GetTimeModeMapping(sysTime, tr, &Tickr);
        //UINT64 t2 = m_LastCanMsg;
        CString omStrCount;

        if(m_LastCanMsg != -1)
        {
            omStrCount.Format(_T("SUCCESS COUNT %d"), m_MsgVerifiedList.GetCount());
        }
        else
        {
            omStrCount.Format(_T("No Message  has Recieved"));
        }

        hResultTC = S_FALSE;
        TSX_DisplayResult(omStrCount);
    }

    return hResultTC;
}

/******************************************************************************
Function Name  :  TSX_VerifyMessage
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CTSExecutionCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
HRESULT CTSExecutionCAN::TSX_VerifyMessage(CBaseEntityTA* pEntity, CResultVerify& ouVerifyResult)
{
    //TODO::Proper Handling Required
    UCHAR* pucData;
    STCANDATA sCanData;
    UINT unVerifyCount;
    HRESULT hResult = S_OK;
    CBaseEntityTA* pVerifyEntity;
    CVerify_MessageData ouVerifyData;
    CString omResult;
    sMESSAGE sMsg;
    CSignalInfoArray ouSignalInfo;
    pEntity->GetSubEntryCount(unVerifyCount);
    ouVerifyResult.m_MessageResultList.RemoveAll();

    //Loop For all Messages
    for(UINT j=0; j<unVerifyCount; j++)
    {
        pEntity->GetSubEntityObj(j, &pVerifyEntity);
        pVerifyEntity->GetEntityData(VERIFY_MESSAGE, &ouVerifyData);

        if(m_ouCanBufVFSE.ReadFromBuffer(&sCanData, (__int64)ouVerifyData.m_dwMessageID)==0)
        {
            pucData = new UCHAR[sCanData.m_uDataInfo.m_sCANMsg.m_ucDataLen];
            memcpy(pucData, &sCanData.m_uDataInfo.m_sCANMsg.m_ucData, sCanData.m_uDataInfo.m_sCANMsg.m_ucDataLen);
        }
        else
        {
            pucData = new UCHAR[8];
            memset(pucData, 0, 8);
        }

        //Interprete The Messages
        SMSGENTRY* sMsgEntry = new SMSGENTRY;
        pEntity->m_ouDataBaseManager.nGetMessageInfo(ouVerifyData.m_omMessageName, sMsg);
        sMsgEntry->m_psMsg = &sMsg;
        sMsgEntry->m_psNext = NULL;
        m_ouMsgInterpret.vSetMessageList(sMsgEntry);
        m_ouMsgInterpret.bInterpretMsgs(ouVerifyData.m_dwMessageID, pucData, ouSignalInfo);
        CString strVerDisplay = "Verifying Message "+ouVerifyData.m_omMessageName;
        TSX_DisplayMessage(strVerDisplay);
        //Verify The Signals
        CMessageResult ouMsgResult;
        omResult = "SUCCESS";

        if( bVerifyCanMessage(ouVerifyData, ouSignalInfo, ouMsgResult) == FALSE)
        {
            omResult = "FAIL";
            hResult = S_FALSE;
        }

        ouVerifyResult.m_MessageResultList.AddTail(ouMsgResult);
        TSX_DisplayResult(omResult);
        delete []pucData;
    }

    return hResult;
}

/******************************************************************************
Function Name  :  bVerifyCanMessage
Input(s)       :
Output         :  BOOL
Functionality  :
Member of      :  CTSExecutionCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
BOOL CTSExecutionCAN::bVerifyCanMessage(CVerify_MessageData& ouVerifyData, CSignalInfoArray& ouSignalInfo, CMessageResult& ouMsgResult)
{
    BOOL bResult = TRUE;
    ouMsgResult.m_omMessage = ouVerifyData.m_omMessageName;
    INT nSignalCount = (INT)ouSignalInfo.GetSize();

    for(INT nIndex = 0; nIndex < nSignalCount; nIndex++)
    {
        CString omStrCondition;
        SINTERPRETSIGNALINFO ouSignal = ouSignalInfo[nIndex];

        //TODO::Handle condition for having Same Signals are presenent(Like mAllrad_1)
        if(ouVerifyData.GetSignalCondition(ouSignal.m_omStrSignalName, omStrCondition) == S_OK)
        {
            CSignalResult ouSignalResult;
            ouSignalResult.m_omSignal = ouSignal.m_omStrSignalName;
            ouSignalResult.m_omSignalCondition = omStrCondition;
            ouSignalResult.m_omResult = "SUCCESS";
            BOOL bRetVal;

            if(ouVerifyData.m_eSignalUnitType == RAW)
            {
                ouSignalResult.m_omSignalValue.Format("%I64u", ouSignal.m_un64RawValue);
                bRetVal = m_ouExpressionEWxecutor.bGetExpressionValue(omStrCondition, (float)ouSignal.m_un64RawValue);
            }
            else
            {
                ouSignalResult.m_omSignalValue.Format("%f", ouSignal.m_dPhyValue);
                bRetVal = m_ouExpressionEWxecutor.bGetExpressionValue(omStrCondition, (float)ouSignal.m_dPhyValue);
            }

            if(bRetVal == FALSE)
            {
                ouSignalResult.m_omResult = "FAIL";
                //if One signal failed total message and total Testcase will be failed
                bResult = FALSE;
            }

            ouMsgResult.m_SignalResultList.AddTail(ouSignalResult);
        }
    }

    return bResult;
}

/******************************************************************************
Function Name  :  TSX_SendMessage
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CTSExecutionCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
HRESULT CTSExecutionCAN::TSX_SendMessage(CBaseEntityTA* pEntity)
{
    UCHAR aucData[8];
    CBaseEntityTA* pSendEntity;
    UINT unSendCount;
    pEntity->GetSubEntryCount(unSendCount);

    for(UINT j=0; j<unSendCount; j++)
    {
        STCAN_MSG stCanData;
        CSend_MessageData ouSendData;
        pEntity->GetSubEntityObj(j, &pSendEntity);
        pSendEntity->GetEntityData(SEND_MESSAGE, &ouSendData);
        sMESSAGE* pMsg = pEntity->m_ouDataBaseManager.unGetMsg(ouSendData.m_dwMessageID);
        bMakeCanMessage(pMsg, ouSendData, aucData, stCanData);
        CString strSendDisplay = "Sending Message "+ouSendData.m_omMessageName;
        TSX_DisplayMessage(strSendDisplay);
        m_pouDIL_CAN->DILC_SendMsg(m_dwClientId, stCanData);
    }

    return S_OK;
}

/******************************************************************************
Function Name  :  bMakeCanMessage
Input(s)       :
Output         :  BOOL
Functionality  :
Member of      :  CTSExecutionCAN
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  01/04/2011
Modifications  :
******************************************************************************/
BOOL CTSExecutionCAN::bMakeCanMessage(sMESSAGE*& pMsg, CSend_MessageData& ouSendData,
                                      UCHAR aucData[8], STCAN_MSG& stCanMsg)
{
    UNREFERENCED_PARAMETER(aucData[0]);

    if(pMsg == NULL)
    {
        return FALSE;
    }

    INT nIndex = 0;
    sSIGNALS* psCurrSignal = pMsg->m_psSignals;
    memset(&stCanMsg.m_ucData, 0, 8*sizeof(UCHAR));

    while (NULL != psCurrSignal)
    {
        POSITION pos = ouSendData.m_odSignalDataList.FindIndex(nIndex++);
        CSignalData& ouSignalData =  ouSendData.m_odSignalDataList.GetAt(pos);
        tagUSIGNALVALUE sSignalValue;

        if(ouSendData.m_eSignalUnitType == ENG)
        {
            float fTempVal;
            fTempVal  = (ouSignalData.m_uValue.m_fValue - psCurrSignal->m_fSignalOffset);
            fTempVal = (fTempVal / psCurrSignal->m_fSignalFactor);
            sSignalValue.m_u64Value = (UINT64)(fTempVal+0.5);

            if(sSignalValue.m_u64Value < (UINT64)psCurrSignal->m_SignalMinValue.n64Value)
            {
                sSignalValue.m_u64Value = (UINT64)psCurrSignal->m_SignalMinValue.n64Value;
            }
        }
        else
        {
            sSignalValue.m_u64Value = ouSignalData.m_uValue.m_u64Value;
        }

        sSIGNALS::vSetSignalValue(psCurrSignal, stCanMsg.m_ucData, sSignalValue.m_u64Value);
        psCurrSignal = psCurrSignal->m_psNextSignalList;
    }

    //Make CAN Message
    stCanMsg.m_ucChannel = 1;
    stCanMsg.m_ucDataLen = (UCHAR)pMsg->m_unMessageLength;
    stCanMsg.m_ucEXTENDED = (UCHAR)pMsg->m_bMessageFrameFormat;
    stCanMsg.m_ucRTR = (pMsg->m_unNumberOfSignals>0)? 0 : 1;        //TODO:CheckThis
    stCanMsg.m_unMsgID = pMsg->m_unMessageCode;
    return TRUE;
}