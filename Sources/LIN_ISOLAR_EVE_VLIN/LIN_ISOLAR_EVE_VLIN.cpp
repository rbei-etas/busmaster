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
 * \file      LIN_ISOLAR_EVE_VLIN.cpp
 * \brief     Source file for ISOLAR EVE VIRTUAL LIN DIL functions
 * \author    Manohar H
 * \copyright Copyright (c) 2011, ETAS GmbH. All rights reserved.
 */
// LIN_ISOLAR_EVE_VLIN.cpp : Defines the initialization routines for the DLL.
//

#include "LIN_ISOLAR_EVE_VLIN_stdafx.h"
#include "LIN_ISOLAR_EVE_VLIN_Defines.h"
#include "EVE_LIN_Controller.h"

/* C++ includes */
#include <sstream>
#include <Windows.h>
#include <string>
#include <vector>

/* Project includes */
#include "LIN_ISOLAR_EVE_VLIN.h"
#include "DIL_Interface/BaseDIL_LIN_Controller.h"
#include "DataTypes\Cluster.h"
#define USAGE_EXPORT
#include "LIN_ISOLAR_EVE_VLIN_Extern.h"

BEGIN_MESSAGE_MAP(CISOLAR_EVE_VLIN, CWinApp)
END_MESSAGE_MAP()


static STLINDATA sg_EVE_LINMsg;
static STLIN_MSG asLinTxMsg[LIN_PID_MAX];

static LARGE_INTEGER sg_QueryTickCount;
static LARGE_INTEGER sg_lnFrequency;
static LARGE_INTEGER sg_lnCurrCounter;
static LARGE_INTEGER sg_lnTimeStamp;
static UINT64     sg_TimeStampRef     = 0x0;
static SYSTEMTIME sg_CurrSysTime;

static HRESULT    sg_hResult          = S_FALSE;

/**
 * CISOLAR_EVE_VLIN construction
 */
CISOLAR_EVE_VLIN::CISOLAR_EVE_VLIN()
{
    // Place all significant initialization in InitInstance
}


// The one and only CISOLAR_EVE_VLIN object
CISOLAR_EVE_VLIN EVE_VLIN_OBJ_0;


/**
 * CISOLAR_EVE_VLIN initialization
 */

BOOL CISOLAR_EVE_VLIN::InitInstance()
{
    CWinApp::InitInstance();

    return TRUE;
}

/* CDIL_ISOLAR_EVE_VLIN class definition */
class CDIL_ISOLAR_EVE_VLIN : public CBaseDIL_LIN_Controller
{
private:
    HINSTANCE m_dllHandle,m_dllHandle_tx;
    ECONTR_STATUS m_Ctrl_Status;

public:
    /* STARTS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
    HRESULT LIN_PerformInitOperations(void);
    HRESULT LIN_PerformClosureOperations(void);
    HRESULT LIN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL);
    HRESULT LIN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount);
    HRESULT LIN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount);
    HRESULT LIN_DeselectHwInterface(void);
    HRESULT LIN_DisplayConfigDlg(PSCONTROLLER_DETAILS_LIN InitData, int& Length);
    HRESULT LIN_SetConfigData(ClusterConfig& ouConfig);
    HRESULT LIN_StartHardware(void);
    HRESULT LIN_PreStartHardware(void);

    HRESULT LIN_StopHardware(void);
    HRESULT LIN_ResetHardware(void);
    HRESULT LIN_GetCurrStatus(s_STATUSMSG& StatusData);
    HRESULT LIN_GetTxMsgBuffer(BYTE*& pouFlxTxMsgBuffer);
    HRESULT LIN_SendMsg(DWORD dwClientID, STLIN_MSG& sLinTxMsg);
    HRESULT LIN_SetSlaveRespData(const STLIN_MSG stRespMsg);
    HRESULT LIN_ResetSlaveRespData(void);
    HRESULT LIN_DisableSlaveRespData(DWORD dwClientID, STLIN_MSG& sMessage);
    HRESULT LIN_GetBusConfigInfo(BYTE* BusInfo);
    HRESULT LIN_GetLastErrorString(std::string& acErrorStr);
    HRESULT LIN_FilterFrames(FILTER_TYPE FilterType, TYPE_CHANNEL Channel, UINT* punMsgIds, UINT nLength);
    HRESULT LIN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT LIN_GetConfiguration(sCONTROLLERDETAILSLIN[], INT& nSize);
    HRESULT LIN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam);
    HRESULT LIN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT LIN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog);
    HRESULT LIN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseLINBufFSE* pBufObj);
    HRESULT LIN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);
    HRESULT LIN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);
    HRESULT LIN_LoadDriverLibrary(void);
    HRESULT LIN_UnloadDriverLibrary(void);
};

CDIL_ISOLAR_EVE_VLIN* g_pouDIL_ISOLAR_EVE_VLIN = NULL;

/**
* \brief         Function to store the Tx message entry into sg_asAckMapBuf list.
* \param[in]     RefObj, is SACK_MAP entry
* \return        void
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
void vMarkEntryIntoMap(const SACK_MAP& RefObj)
{
    //EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    sg_asAckMapBuf.push_back(RefObj);
    //LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
}

/**
* \brief         Function to retreive the Tx message entry from sg_asAckMapBuf list and delete.
*                Also returns the respective client ID.
* \param[in]     RefObj, is SACK_MAP entry to find and delete
* \param[out]    ClientID, is client ID of the RefObj found
* \return        TRUE if RefObj is found, else FALSE
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
BOOL bRemoveMapEntry(const SACK_MAP& RefObj, UINT& ClientID)
{
    EnterCriticalSection(&sg_CritSectForAckBuf); // Lock the buffer
    BOOL bResult = FALSE;
    CACK_MAP_LIST::iterator  iResult =
        std::find( sg_asAckMapBuf.begin(), sg_asAckMapBuf.end(), RefObj );

    //if ((*iResult).m_ClientID > 0)
    if (iResult != sg_asAckMapBuf.end())
    {
        bResult = TRUE;
        ClientID = (*iResult).m_ClientID;
        sg_asAckMapBuf.erase(iResult);
    }
    LeaveCriticalSection(&sg_CritSectForAckBuf); // Unlock the buffer
    return bResult;
}

/**
* \brief         Writes the message 'sLinData' to the corresponding clients buffer
* \param[out]    sLinData, is STLINDATA structure
* \return        void
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
static void vWriteIntoClientsBuffer(STLINDATA& sLinData)
{
    UINT64 TimeStamp = 0x0;

    // Calculate the current time stamp assigning the same to
    // the message
    LARGE_INTEGER CurrCounter;
    QueryPerformanceCounter(&CurrCounter);
    // Convert it to time stamp with the granularity of hundreds of us
    TimeStamp =
        (CurrCounter.QuadPart * 10000) / sg_lnFrequency.QuadPart;

    sLinData.m_lTickCount.QuadPart = TimeStamp;

    if (CREATE_MAP_TIMESTAMP == sg_byCurrState)
    {
        UINT64 unConnectionTime;
        unConnectionTime = ((CurrCounter.QuadPart * 10000) / sg_lnFrequency.QuadPart) - sg_TimeStampRef;
        //Time difference should be +ve value
        if(sLinData.m_lTickCount.QuadPart >= unConnectionTime)
        {
            sg_TimeStampRef  = (LONGLONG)(sLinData.m_lTickCount.QuadPart - unConnectionTime);
        }
        else
        {
            sg_TimeStampRef  = (LONGLONG)(unConnectionTime - sLinData.m_lTickCount.QuadPart);
        }

        sg_byCurrState = CALC_TIMESTAMP_READY;
    }


    //Write into the client's buffer and Increment message Count
    static SACK_MAP sAckMap;
    UINT ClientId = 0;
    static UINT Index = (UINT)-1;
    sAckMap.m_Channel = sLinData.m_uDataInfo.m_sLINMsg.m_ucChannel;
    sAckMap.m_MsgID = sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgID;

    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        for (UINT j = 0; j < sg_asClientToBufMap[i].unBufCount; j++)
        {
            sg_asClientToBufMap[i].pClientBuf[j]->WriteIntoBuffer(&sLinData);
        }
    }

}

static void vValidateReceivedLinPID(STLIN_MSG& RxMsg)
{
    HRESULT ret_result = S_FALSE;

    /*STLIN_MSG asLinTxMsg[];*/
    if(asLinTxMsg[RxMsg.m_ucMsgID].m_ucMsgTyp == LIN_SLAVE_RESPONSE)
    {
        RxMsg.m_ucMsgTyp = asLinTxMsg[RxMsg.m_ucMsgID].m_ucMsgTyp;
        RxMsg.m_ucDataLen =  asLinTxMsg[RxMsg.m_ucMsgID].m_ucDataLen;

        //sg_EVE_LINMsg.m_ucDataType = TX_FLAG;


        for(int i=0; i<8; i++)
        {
            RxMsg.m_ucData[i] = asLinTxMsg[RxMsg.m_ucMsgID].m_ucData[i];
        }

        ret_result = g_pouDIL_ISOLAR_EVE_VLIN->LIN_SetSlaveRespData(RxMsg);
    }
    //else if(asLinTxMsg[RxMsg.m_ucMsgID].m_ucMsgTyp == LIN_SLAVE_SLAVE)
    //{
    //  RxMsg.m_ucMsgTyp = LIN_SLAVE_SLAVE;
    //}
    else
    {
        sg_EVE_LINMsg.m_ucDataType = RX_FLAG;
        RxMsg.m_ucMsgTyp = LIN_RESPONSE_NOT_CONFIGURED;
    }
}

static void vValidateReceivedLinMsg(STLIN_MSG& RxMsg)
{
    HRESULT ret_result = S_FALSE;

    if((RxMsg.m_ucMsgID == 0x3C)&&(RxMsg.m_ucData[0]==0x00))
    {
        RxMsg.m_ucMsgTyp = LIN_SLEEP_FRAME;
    }
    else
    {
        RxMsg.m_ucMsgTyp = LIN_MASTER_RESPONSE;
    }
}

/**
* \brief         Read thread procedure
* \param[in]     pVoid contains the CPARAM_THREADPROC class object
* \return        void
* \authors       Prince Varghese
* \date          09.04.2013 Created
*/
DWORD WINAPI LinMsgReadThreadProc_LIN_ISolar_Eve(LPVOID pVoid)
{
    USES_CONVERSION;

    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;

    /* Assign thread action to CREATE_TIME_MAP */
    pThreadParam->m_unActionCode = CREATE_TIME_MAP;

    /* Dummy action event */
    pThreadParam->m_hActionEvent = CreateEvent(NULL, false, false, NULL);

    bool bLoopON = true;

    while (bLoopON)
    {
        Sleep (10);
        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                static STLIN_FRAME RxFrame;
                static STLIN_MSG RxMsg;
                static int iRxLength;
                int iChecksum = 0;
                RxMsg.m_ucDataLen = 0;
                unsigned int uiDataType = ReceiveLINMessageFromClient(&RxFrame, &iRxLength);


                if (iRxLength == LIN_PID_LENGTH)
                {
                    RxMsg.m_ucMsgID   =  RxFrame.m_ucMsgID;
                    RxMsg.m_ucChksum  = 0;

                    vValidateReceivedLinPID(RxMsg);

                    /*sg_EVE_LINMsg.m_ucDataType = TX_FLAG;*/
                    sg_EVE_LINMsg.m_uDataInfo.m_sLINMsg = RxMsg;
                    RxMsg.m_ucChannel = LIN_CHANNEL_1;
                    //vWriteIntoClientsBuffer(sg_EVE_LINMsg);
                }
                else if (iRxLength > LIN_PID_LENGTH)
                {
                    RxMsg.m_ucMsgID   =  RxFrame.m_ucMsgID;
                    RxMsg.m_ucChksum  =  RxFrame.m_ucChksum;
                    RxMsg.m_ucDataLen =  iRxLength - (LIN_PID_LENGTH + LIN_CHECKSUM_LENGTH); // 2 (1 byte checksum and 1byte pid)

                    for(int i=0; i<8; i++)
                    {
                        RxMsg.m_ucData[i] = RxFrame.m_ucData[i];
                        iChecksum = iChecksum + RxFrame.m_ucData[i];
                        if (iChecksum > 255)
                        {
                            iChecksum = iChecksum - 255;
                        }
                    }
                    RxMsg.m_ucChksum = 255 - iChecksum;

                    if((RxMsg.m_ucChksum != RxFrame.m_ucChksum))
                    {
                        RxMsg.m_ucMsgTyp = LIN_CHECKSUM_ERROR;
                        RxMsg.m_ucChksum = RxFrame.m_ucChksum;
                    }
                    else
                    {
                        vValidateReceivedLinMsg(RxMsg);
                    }

                    if(uiDataType == TX_LOOPBACK_UDP_DATA)
                    {
                        sg_EVE_LINMsg.m_ucDataType = TX_FLAG;
                    }
                    else if((uiDataType == RX_UDP_PID)||(uiDataType == RX_UDP_DATA))
                    {
                        sg_EVE_LINMsg.m_ucDataType = RX_FLAG;
                    }

                    RxMsg.m_ucChannel = LIN_CHANNEL_1;

                    sg_EVE_LINMsg.m_uDataInfo.m_sLINMsg = RxMsg;
                    RxMsg.m_ucChannel = 1;
                    vWriteIntoClientsBuffer(sg_EVE_LINMsg);
                }
            }
            break;
            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;
            case CREATE_TIME_MAP:
            {
                SetEvent(pThreadParam->m_hActionEvent);
                pThreadParam->m_unActionCode = INVOKE_FUNCTION;
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
    pThreadParam->m_hActionEvent = NULL;

    return 0;
}




/**
* \brief         Returns the CDIL_ISOLAR_EVE_VLIN object
* \param[out]    ppvInterface, is void pointer to take back the reference to CDIL_ISOLAR_EVE_VLIN object
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          08.03.2013 Created
*/
USAGEMODE HRESULT GetIDIL_LIN_Controller(void** ppvInterface)
{
    HRESULT hResult = S_OK;
    if ( NULL == g_pouDIL_ISOLAR_EVE_VLIN )
    {
        if ((g_pouDIL_ISOLAR_EVE_VLIN = new CDIL_ISOLAR_EVE_VLIN) == NULL)
        {
            hResult = S_FALSE;
        }
    }
    *ppvInterface = (void*) g_pouDIL_ISOLAR_EVE_VLIN;  /* Doesn't matter even if g_pouDIL_ISOLAR_EVE_VLIN is null */

    return hResult;
}

/**************************** Helper APIs *****************************************/

/**
* \brief         This function will check if the client exists and gives back the client index.
* \param[in]     pcClientName, client name as TCHAR*
* \param[out]    Index, client index if found
* \return        TRUE if client name is found, else FALSE
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static BOOL bClientExist(std::string pcClientName, INT& Index)
{
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (!_tcscmp(pcClientName.c_str(), sg_asClientToBufMap[i].pacClientName))
        {
            Index = i;
            return TRUE;
        }
    }
    return FALSE;
}

/**
* \brief         This function will get available client slot
* \param         void
* \return        Returns the available client ID
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static DWORD dwGetAvailableClientSlot()
{
    DWORD nClientId = 2;
    for (int i = 0; i < MAX_CLIENT_ALLOWED; i++)
    {
        if (bClientIdExist(nClientId))
        {
            nClientId += 1;
        }
        else
        {
            i = MAX_CLIENT_ALLOWED; // break the loop
        }
    }

    return nClientId;
}

/**
* \brief         This function will remove the existing client ID
* \param[in]     dwClientId, client ID to be removed
* \return        Returns TRUE if client ID removal is success, else FALSE
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static BOOL bRemoveClient(DWORD dwClientId)
{
    BOOL bResult = FALSE;
    if (sg_unClientCnt > 0)
    {
        UINT unClientIndex = (UINT)-1;
        if (bGetClientObj(dwClientId, unClientIndex))
        {
            /* clear the client first */
            if (sg_asClientToBufMap[unClientIndex].hClientHandle != NULL)
            {
                HRESULT hResult = S_OK;//(*pfLIN_RemoveClient)(sg_asClientToBufMap[unClientIndex].hClientHandle);
                if (hResult == S_OK)
                {
                    sg_asClientToBufMap[unClientIndex].dwClientID = 0;
                    sg_asClientToBufMap[unClientIndex].hClientHandle = NULL;
                    memset (sg_asClientToBufMap[unClientIndex].pacClientName, 0, sizeof (char) * MAX_PATH);
                    for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
                    {
                        sg_asClientToBufMap[unClientIndex].pClientBuf[i] = NULL;
                    }
                    sg_asClientToBufMap[unClientIndex].unBufCount = 0;
                    bResult = TRUE;
                }
                else
                {
                    vRetrieveAndLog(hResult, __FILE__, __LINE__);
                }
            }
            else
            {
                sg_asClientToBufMap[unClientIndex].dwClientID = 0;
                memset (sg_asClientToBufMap[unClientIndex].pacClientName, 0, sizeof (char) * MAX_PATH);
                for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
                {
                    sg_asClientToBufMap[unClientIndex].pClientBuf[i] = NULL;
                }
                sg_asClientToBufMap[unClientIndex].unBufCount = 0;
                bResult = TRUE;

            }
            if (bResult == TRUE)
            {
                if ((unClientIndex + 1) < sg_unClientCnt)
                {
                    sg_asClientToBufMap[unClientIndex] = sg_asClientToBufMap[sg_unClientCnt - 1];
                }
                sg_unClientCnt--;
            }
        }
    }
    return bResult;
}

/**
* \brief         Function to remove exissting client buffer
* \param[in]     RootBufferArray, pointer to CBaseLINBufFSE class array
* \param[out]    unCount, indicates buffer count which will get reduced
* \param[in]     BufferToRemove, pointer to the buffer to be removed
* \return        TRUE if removed
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static BOOL bRemoveClientBuffer(CBaseLINBufFSE* RootBufferArray[MAX_BUFF_ALLOWED], UINT& unCount, CBaseLINBufFSE* BufferToRemove)
{
    BOOL bReturn = TRUE;
    for (UINT i = 0; i < unCount; i++)
    {
        if (RootBufferArray[i] == BufferToRemove)
        {
            if (i < (unCount - 1)) /* If not the last bufffer */
            {
                RootBufferArray[i] = RootBufferArray[unCount - 1];
            }
            unCount--;
        }
    }
    return bReturn;
}

/**
* \brief         Function to check if client buffer exists
* \param[in]     sClientObj, alias to SCLIENTBUFMAP object
* \param[in]     pBuf, pointer to CBaseLINBufFSE object
* \return        TRUE if buffer exists, else FALSE
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseLINBufFSE* pBuf)
{
    BOOL bExist = FALSE;
    for (UINT i = 0; i < sClientObj.unBufCount; i++)
    {
        if (pBuf == sClientObj.pClientBuf[i])
        {
            bExist = TRUE;
            i = sClientObj.unBufCount; // break the loop
        }
    }

    return bExist;
}

/**
* \brief         Function to retreive error occurred and log it
* \param[in]     File, pointer to log file
* \param[in]     Line, indicates line number in log file
* \return        void
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static void vRetrieveAndLog(DWORD /*dwErrorCode*/, char* File, int Line)
{
    USES_CONVERSION;

    char acErrText[MAX_PATH] = {'\0'};

    /* Get the error text for the corresponding error code */
    sg_pIlog->vLogAMessage(A2T(File), Line, A2T(acErrText));

    size_t nStrLen = strlen(acErrText);
    if (nStrLen > LIN_MAX_ERRSTR)
    {
        nStrLen = LIN_MAX_ERRSTR;
    }
    sg_acErrStr = acErrText;
}

/**
* \brief         This function will check if the client ID exists
* \param[in]     dwClientId, client ID to be checked for existance
* \return        Returns TRUE if client ID existance is success, else FALSE
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static BOOL bClientIdExist(const DWORD& dwClientId)
{
    BOOL bReturn = FALSE;
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (sg_asClientToBufMap[i].dwClientID == dwClientId)
        {
            bReturn = TRUE;
            i = sg_unClientCnt; // break the loop
        }
    }
    return bReturn;
}

/**
* \brief         This function will return the client index based on clientID
* \param[in]     dwClientId, client ID whose client index is needed
* \param[out]    unClientIndex, client index to be returned
* \return        Returns TRUE if client ID existance is success, else FALSE
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static BOOL bGetClientObj(DWORD dwClientID, UINT& unClientIndex)
{
    BOOL bResult = FALSE;
    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        if (sg_asClientToBufMap[i].dwClientID == dwClientID)
        {
            unClientIndex = i;
            i = sg_unClientCnt; // break the loop
            bResult = TRUE;
        }
    }
    return bResult;
}


/****************** Implementation for CDIL_ISOLAR_EVE_VLIN class ***********************/

/**
* \brief         Loads vendor's driver library
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       Prince Varghese
* \date          08.03.2013 Created
*/
HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_LoadDriverLibrary(void)
{
    InitializeLIN();
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_UnloadDriverLibrary(void)
{
    CleanUpLIN();
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_PerformInitOperations(void)
{
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_PerformClosureOperations(void)
{
    //Terminate the read thread
    sg_sParmRThread.bTerminateThread();
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* lQueryTickCount)
{
    memcpy(&CurrSysTime, &sg_CurrSysTime, sizeof(SYSTEMTIME));
    TimeStamp = sg_TimeStampRef;
    if(lQueryTickCount != NULL)
    {
        *lQueryTickCount = sg_QueryTickCount;
    }
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount)
{
    for (UINT i = 0; i < CHANNEL_ALLOWED; i++)
    {
        sSelHwInterface[i].m_dwIdInterface = 0x100;
        sSelHwInterface[i].m_acNameInterface = _("ISOLAR_EVE_LIN");
        sSelHwInterface[i].m_acDescription = _("A simulation engine to create a virtual bus system");
    }
    nCount = CHANNEL_ALLOWED;
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount)
{
    //SetCurrState(STATE_INITIALISED);
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_DeselectHwInterface(void)
{

    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_DisplayConfigDlg(PSCONTROLLER_DETAILS_LIN InitData, int& Length)
{

    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_SetConfigData(ClusterConfig& ouConfig)
{

    return S_OK;
}

static void vCreateTimeModeMapping()
{
    GetLocalTime(&sg_CurrSysTime);

    // Save the current value of the high-resolution performance counter,
    // associated to the saved system time to the closest proximity.
    QueryPerformanceCounter(&sg_lnCurrCounter);

    // Get frequency of the performance counter
    QueryPerformanceFrequency(&sg_lnFrequency);

    // Convert it to time stamp with the granularity of hundreds of microsecond
    if (sg_lnCurrCounter.QuadPart * 10000 > sg_lnCurrCounter.QuadPart)
    {
        sg_TimeStampRef = (sg_lnCurrCounter.QuadPart * 10000)
                          / sg_lnFrequency.QuadPart;
    }
    else
    {
        sg_TimeStampRef = (sg_lnCurrCounter.QuadPart
                           / sg_lnFrequency.QuadPart) * 10000;
    }
}
HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_PreStartHardware(void)
{
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_StartHardware(void)
{
    m_Ctrl_Status = NORMAL_ACTIVE;

    /* Transit into 'CREATE TIME MAP' state */
    sg_byCurrState = CREATE_MAP_TIMESTAMP;
    vCreateTimeModeMapping();
    if (sg_sParmRThread.bStartThread(LinMsgReadThreadProc_LIN_ISolar_Eve))
    {
        return S_OK;
    }
    else
    {
        return S_FALSE;
    }

}

HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_StopHardware(void)
{
    //Terminate the read thread
    sg_sParmRThread.bTerminateThread();

    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_ResetHardware(void)
{
    m_Ctrl_Status = INITIALISED;
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus)
{

    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_SendMsg(DWORD dwClientID, STLIN_MSG& sLinTxMsg)
{
    HRESULT ret_result = S_FALSE;
    unsigned char ucMsgID;

    ucMsgID = sLinTxMsg.m_ucMsgID;

    asLinTxMsg[ucMsgID].m_ucChannel     = sLinTxMsg.m_ucChannel;
    asLinTxMsg[ucMsgID].m_ucChksum      = sLinTxMsg.m_ucChksum;
    asLinTxMsg[ucMsgID].m_ucChksumTyp   = sLinTxMsg.m_ucChksumTyp;
    asLinTxMsg[ucMsgID].m_ucDataLen     = sLinTxMsg.m_ucDataLen;
    asLinTxMsg[ucMsgID].m_ucMsgID       = sLinTxMsg.m_ucMsgID;
    asLinTxMsg[ucMsgID].m_ucMsgTyp      = sLinTxMsg.m_ucMsgTyp;

    for(int j=0; j < 8; j++)
    {
        asLinTxMsg[ucMsgID].m_ucData[j] = sLinTxMsg.m_ucData[j];
    }

    return ret_result;
}

HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_SetSlaveRespData(STLIN_MSG stRespMsg)
{
    HRESULT ret_result = S_FALSE;
    static STLIN_FRAME sLinTxFrame;
    int iChecksum = 0;

    sLinTxFrame.m_ucMsgID  = stRespMsg.m_ucMsgID;

    for (int i=0; i<8; i++)
    {
        sLinTxFrame.m_ucData[i] = stRespMsg.m_ucData[i];
        iChecksum =  iChecksum + sLinTxFrame.m_ucData[i];
        if (iChecksum > 255)
        {
            iChecksum = iChecksum - 255;
        }
    }

    sLinTxFrame.m_ucChksum = 255 - iChecksum;
    stRespMsg.m_ucChksum = sLinTxFrame.m_ucChksum;

    if (SendLINMessageToClient(&sLinTxFrame) == 0 )
    {
        ret_result= S_OK;
    }

    return ret_result;
}
HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_ResetSlaveRespData(void)
{
    for(int i=0; i <= LIN_PID_MAX; i++)
    {
        asLinTxMsg[i].m_ucMsgTyp = LIN_INVALID_RESPONSE;
        asLinTxMsg[i].m_ucDataLen = 0;
    }

    return S_OK;
}
HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_DisableSlaveRespData(DWORD dwClientID, STLIN_MSG & sMessage)
{
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_GetLastErrorString(std::string & acErrorStr)
{
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_FilterFrames(FILTER_TYPE FilterType, TYPE_CHANNEL Channel, UINT * punMsgIds, UINT nLength)
{
    return S_OK;
}

/**
* \brief         Function to get Controller status
* \param[out]    StatusData, is s_STATUSMSG structure
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_GetCurrStatus(s_STATUSMSG & StatusData)
{
    StatusData.wControllerStatus = m_Ctrl_Status;

    return S_OK;
}

/**
* \brief         Gets the Tx queue configured.
* \param[out]    pouFlxTxMsgBuffer, is BYTE*
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_GetTxMsgBuffer(BYTE*& /*pouFlxTxMsgBuffer*/)
{
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_GetConfiguration(sCONTROLLERDETAILSLIN psControllerConfig[], INT & nSize)
{
    for ( int i = 0; i < 1; i++ )
    {
        psControllerConfig[i].m_strHwUri =  "ISOLAR EVE Channel 1";
        psControllerConfig[i].m_nBaudRate =  9600;
        psControllerConfig[i].m_strProtocolVersion = "LIN 1.3";
    }
    nSize = 1;

    return S_OK;
}


/**
* \brief         Gets the controller parametes of the channel based on the request.
* \param[out]    lParam, the value of the controller parameter requested.
* \param[in]     nChannel, indicates channel ID
* \param[in]     eContrParam, indicates controller parameter
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
{
    HRESULT hResult = S_OK;
    switch (eContrParam)
    {
        case NUMBER_HW:
        {
            lParam = 1;
        }
        break;
        case NUMBER_CONNECTED_HW:
        {
        }
        break;
        case DRIVER_STATUS:
        {
        }
        break;
        case HW_MODE:
        {
        }
        break;
        case CON_TEST:
        {

        }
        break;
        default:
        {
            hResult = S_FALSE;
        }
        break;

    }
    return hResult;
}

/**
* \brief         Sets the controller parametes of the channel based on the request
* \param[out]    lParam, the value of the controller parameter requested.
* \param[in]     Value
* \param[in]     eContrParam, indicates controller parameter
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_SetControllerParams(INT nValue, ECONTR_PARAM eContrparam)
{
    switch(eContrparam)
    {
        case HW_MODE:
        {
            switch(nValue)
            {
                case defMODE_ACTIVE:
                {
                    /* Activation code */
                }
                break;
                case defMODE_PASSIVE:
                {
                    /* De-activation code */
                }
                break;
            }
        }
    }
    return S_OK;
}

/**
* \brief         Gets the error counter for corresponding channel.
* \param[out]    sErrorCnt, is SERROR_CNT structure
* \param[in]     nChannel, indicates channel ID
* \param[in]     eContrParam, indicates controller parameter
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
{
    HRESULT hResult = S_OK;
    return hResult;
}

/**
* \brief         Sets the application params.
* \param[in]     hWndOwner, is the main frame HWND value
* \param[in]     pILog, is pointer to error logger object
* \return        S_OK (always)
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog)
{
    return S_OK;
}

/**
* \brief         Registers a client to the DIL.
* \param[in]     bRegister, if TRUE signifies 'Register', FALSE indicates 'Unregister'
* \param[out]    ClientID, is Client ID assigned, will be used for further client related calls
* \param[in]     pacClientName, is the client name
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)
{
    USES_CONVERSION;
    HRESULT hResult = S_FALSE;

    if (bRegister)
    {
        if (sg_unClientCnt < MAX_CLIENT_ALLOWED)
        {
            INT Index = 0;
            if (!bClientExist(pacClientName, Index))
            {
                /* Currently store the client information */
                if (_tcscmp(pacClientName, LIN_MONITOR_NODE) == 0)
                {
                    /* First slot is reserved to monitor node */
                    ClientID = 1;
                    _tcscpy(sg_asClientToBufMap[0].pacClientName, pacClientName);
                    sg_asClientToBufMap[0].dwClientID = ClientID;
                    sg_asClientToBufMap[0].unBufCount = 0;
                }
                else
                {
                    if (!bClientExist(LIN_MONITOR_NODE, Index))
                    {
                        Index = sg_unClientCnt + 1;
                    }
                    else
                    {
                        Index = sg_unClientCnt;
                    }
                    ClientID = dwGetAvailableClientSlot();
                    _tcscpy(sg_asClientToBufMap[Index].pacClientName, pacClientName);

                    sg_asClientToBufMap[Index].dwClientID = ClientID;
                    sg_asClientToBufMap[Index].unBufCount = 0;
                }
                sg_unClientCnt++;
                hResult = S_OK;
            }
            else
            {
                ClientID = sg_asClientToBufMap[Index].dwClientID;
                hResult = ERR_CLIENT_EXISTS;
            }
        }
        else
        {
            hResult = ERR_NO_MORE_CLIENT_ALLOWED;
        }
    }
    else
    {
        if (bRemoveClient(ClientID))
        {
            hResult = S_OK;
        }
        else
        {
            hResult = ERR_NO_CLIENT_EXIST;
        }
    }

    return hResult;
}

/**
* \brief         Registers the buffer pBufObj to the client ClientID
* \param[in]     bAction, contains one of the values MSGBUF_ADD or MSGBUF_CLEAR
* \param[in]     ClientID, is the client ID
* \param[in]     pBufObj, is pointer to CBaseLINBufFSE object
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_ISOLAR_EVE_VLIN::LIN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseLINBufFSE* pBufObj)
{
    HRESULT hResult = S_FALSE;
    if (ClientID != NULL)
    {
        UINT unClientIndex;
        if (bGetClientObj(ClientID, unClientIndex))
        {
            SCLIENTBUFMAP& sClientObj = sg_asClientToBufMap[unClientIndex];
            if (bAction == MSGBUF_ADD)
            {
                /* Add msg buffer */
                if (pBufObj != NULL)
                {
                    if (sClientObj.unBufCount < MAX_BUFF_ALLOWED)
                    {
                        if (bIsBufferExists(sClientObj, pBufObj) == FALSE)
                        {
                            sClientObj.pClientBuf[sClientObj.unBufCount++] = pBufObj;
                            hResult = S_OK;
                        }
                        else
                        {
                            hResult = ERR_BUFFER_EXISTS;
                        }
                    }
                }
            }
            else if (bAction == MSGBUF_CLEAR)
            {
                /* clear msg buffer */
                if (pBufObj != NULL)
                {
                    /* Remove only buffer mentioned */
                    bRemoveClientBuffer(sClientObj.pClientBuf, sClientObj.unBufCount, pBufObj);
                }
                else
                {
                    /* Remove all */
                    for (UINT i = 0; i < sClientObj.unBufCount; i++)
                    {
                        sClientObj.pClientBuf[i] = NULL;
                    }
                    sClientObj.unBufCount = 0;
                }
                hResult = S_OK;
            }
        }
        else
        {
            hResult = ERR_NO_CLIENT_EXIST;
        }
    }
    else
    {
        if (bAction == MSGBUF_CLEAR)
        {
            /* clear msg buffer */
            for (UINT i = 0; i < sg_unClientCnt; i++)
            {
                LIN_ManageMsgBuf(MSGBUF_CLEAR, sg_asClientToBufMap[i].dwClientID, NULL);
            }
            hResult = S_OK;
        }
    }

    return hResult;
}




