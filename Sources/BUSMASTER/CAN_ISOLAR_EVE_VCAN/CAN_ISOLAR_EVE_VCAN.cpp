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


#include "CAN_ISOLAR_EVE_VCAN_stdafx.h"
#include "CAN_ISOLAR_EVE_VCAN_Defines.h"

/* C++ includes */
#include <sstream>
#include <Windows.h>
#include <string>
#include <vector>

/* Project includes */
#include "CAN_ISOLAR_EVE_VCAN.h"
//#include "DIL_Interface/BaseDIL_CAN_Controller.h"
#include "BaseDIL_CAN_Controller.h"
#include "DILPluginHelperDefs.h"
#define USAGE_EXPORT
#include "CAN_ISOLAR_EVE_VCAN_Extern.h"

BEGIN_MESSAGE_MAP(CISOLAR_EVE_VCAN, CWinApp)
END_MESSAGE_MAP()

static STCANDATA sg_EVE_CANMsg;

CISOLAR_EVE_VCAN::CISOLAR_EVE_VCAN()
{
    // Place all significant initialization in InitInstance
}

/** The one and only CISOLAR_EVE_VCAN object */
CISOLAR_EVE_VCAN EVE_VCAN_OBJ_0;


/**
 * CISOLAR_EVE_VCAN initialization
 */

BOOL CISOLAR_EVE_VCAN::InitInstance()
{
    CWinApp::InitInstance();

    return TRUE;
}

/* CDIL_ISOLAR_EVE_VCAN class definition */
class CDIL_ISOLAR_EVE_VCAN : public CBaseDIL_CAN_Controller
{
private:
    HINSTANCE m_dllHandle,m_dllHandle_tx;
    ECONTR_STATUS m_Ctrl_Status;

public:
    /* STARTS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
    HRESULT CAN_PerformInitOperations(void);
    HRESULT CAN_PerformClosureOperations(void);
    HRESULT CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount);
    HRESULT CAN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount, PSCONTROLLER_DETAILS InitData);
    HRESULT CAN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount);
    HRESULT CAN_DeselectHwInterface(void);
    HRESULT CAN_SetConfigData(PSCONTROLLER_DETAILS InitData, int Length);
    HRESULT CAN_StartHardware(void);
    HRESULT CAN_StopHardware(void);
    HRESULT CAN_ResetHardware(void);
    HRESULT CAN_GetCurrStatus(STATUSMSG& StatusData);
    HRESULT CAN_GetTxMsgBuffer(BYTE*& pouFlxTxMsgBuffer);
    HRESULT CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg);
    HRESULT CAN_GetBusConfigInfo(BYTE* BusInfo);
    HRESULT CAN_GetLastErrorString(std::string& acErrorStr);
    HRESULT CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT CAN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam);
    HRESULT CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT CAN_SetAppParams(HWND hWndOwner);
    HRESULT CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj);
    HRESULT CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);
    HRESULT CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);
    HRESULT CAN_LoadDriverLibrary(void);
    HRESULT CAN_UnloadDriverLibrary(void);
    HRESULT CAN_SetHardwareChannel(PSCONTROLLER_DETAILS,DWORD dwDriverId,bool bIsHardwareListed, unsigned int unChannelCount);
};

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
* \brief         Writes the message 'sCanData' to the corresponding clients buffer
* \param[out]    sCanData, is STCANDATA structure
* \return        void
* \authors       Arunkumar Karri
* \date          12.10.2011 Created
*/
static void vWriteIntoClientsBuffer(STCANDATA& sCanData)
{
    //Write into the client's buffer and Increment message Count
    static SACK_MAP sAckMap;
    sAckMap.m_Channel = sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel;
    sAckMap.m_MsgID = sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID;

    for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        for (UINT j = 0; j < sg_asClientToBufMap[i].unBufCount; j++)
        {
            sg_asClientToBufMap[i].pClientBuf[j]->WriteIntoBuffer(&sCanData);
        }
    }

}

/**
* \brief         Read thread procedure
* \param[in]     pVoid contains the CPARAM_THREADPROC class object
* \return        void
* \authors       Prince Varghese
* \date          09.04.2013 Created
*/
DWORD WINAPI CanMsgReadThreadProc_CAN_ISolar_Eve(LPVOID pVoid)
{
    USES_CONVERSION;

    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;

    /* Assign thread action to CREATE_TIME_MAP */
    pThreadParam->m_unActionCode = CREATE_TIME_MAP;

    bool bLoopON = true;

    while (bLoopON)
    {
        Sleep (10); // To avoid continuous loop as there is no event mechanism
        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {

                int functionType = 0;
                unsigned int dataSize = 0;

                if (ReceiveCommandFromClient(&functionType, &dataSize) >= 0)
                {
                    switch (functionType)
                    {
                        case SEND_MESSAGE:
                        {
                            STCAN_MSG RxMsg;
                            int receiveResult = ReceiveCANMessageFromClient(&RxMsg);
                            if (receiveResult > 0)
                            {
                                sg_EVE_CANMsg.m_uDataInfo.m_sCANMsg.m_bCANFD = false;
                                sg_EVE_CANMsg.m_ucDataType = receiveResult;
                                sg_EVE_CANMsg.m_uDataInfo.m_sCANMsg = RxMsg;

                                RxMsg.m_ucChannel = 1;
                                vWriteIntoClientsBuffer(sg_EVE_CANMsg);
                            }
                        }
                        break;

                        default:
                            break;
                    }
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
    pThreadParam->m_hActionEvent = nullptr;

    return 0;
}

CDIL_ISOLAR_EVE_VCAN* g_pouDIL_ISOLAR_EVE_VCAN = nullptr;

/**
* \brief         Returns the CDIL_ISOLAR_EVE_VCAN object
* \param[out]    ppvInterface, is void pointer to take back the reference to CDIL_ISOLAR_EVE_VCAN object
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          08.03.2013 Created
*/
USAGEMODE HRESULT GetIDIL_CAN_Controller(void** ppvInterface)
{
    HRESULT hResult = S_OK;
    if ( nullptr == g_pouDIL_ISOLAR_EVE_VCAN )
    {
        if ((g_pouDIL_ISOLAR_EVE_VCAN = new CDIL_ISOLAR_EVE_VCAN) == nullptr)
        {
            hResult = S_FALSE;
        }
    }
    *ppvInterface = (void*) g_pouDIL_ISOLAR_EVE_VCAN;  /* Doesn't matter even if g_pouDIL_ISOLAR_EVE_VCAN is null */

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
            if (sg_asClientToBufMap[unClientIndex].hClientHandle != 0)
            {
                HRESULT hResult = S_OK;//(*pfCAN_RemoveClient)(sg_asClientToBufMap[unClientIndex].hClientHandle);
                if (hResult == S_OK)
                {
                    sg_asClientToBufMap[unClientIndex].dwClientID = 0;
                    sg_asClientToBufMap[unClientIndex].hClientHandle = 0;
                    memset (sg_asClientToBufMap[unClientIndex].pacClientName, 0, sizeof (char) * MAX_PATH);
                    for (int i = 0; i < MAX_BUFF_ALLOWED; i++)
                    {
                        sg_asClientToBufMap[unClientIndex].pClientBuf[i] = nullptr;
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
                    sg_asClientToBufMap[unClientIndex].pClientBuf[i] = nullptr;
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
* \param[in]     RootBufferArray, pointer to CBaseCANBufFSE class array
* \param[out]    unCount, indicates buffer count which will get reduced
* \param[in]     BufferToRemove, pointer to the buffer to be removed
* \return        TRUE if removed
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static BOOL bRemoveClientBuffer(CBaseCANBufFSE* RootBufferArray[MAX_BUFF_ALLOWED], UINT& unCount, CBaseCANBufFSE* BufferToRemove)
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
* \param[in]     pBuf, pointer to CBaseCANBufFSE object
* \return        TRUE if buffer exists, else FALSE
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static BOOL bIsBufferExists(const SCLIENTBUFMAP& sClientObj, const CBaseCANBufFSE* pBuf)
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
static void vRetrieveAndLog(DWORD /*dwErrorCode*/, char* /*File*/, int /*Line*/)
{
    USES_CONVERSION;

    char acErrText[MAX_PATH] = {'\0'};

    size_t nStrLen = strlen(acErrText);
    if (nStrLen > CAN_MAX_ERRSTR)
    {
        nStrLen = CAN_MAX_ERRSTR;
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


/****************** Implementation for CDIL_ISOLAR_EVE_VCAN class ***********************/

/**
* \brief         Loads vendor's driver library
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       Prince Varghese
* \date          08.03.2013 Created
*/
HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_LoadDriverLibrary(void)
{
    HRESULT hResult = S_FALSE;

    /***************************************** Receive Controller **********************************************************************/

    m_dllHandle = LoadLibrary("Controller_1.dll");
    if (m_dllHandle != nullptr)
    {
        Initialize = (LPFNDLLFUNC_Initialize)GetProcAddress(m_dllHandle, "Initialize");
        if(Initialize)
        {
            if (Initialize() != 0)
            {
                //Trace("Failed to Initialize");
                FreeLibrary(m_dllHandle);
            }
            else
            {
                CleanUp = (LPFNDLLFUNC_CleanUp) GetProcAddress(m_dllHandle, "CleanUp");
                if (!CleanUp)
                {
                    //Trace("Function address for 'CleanUp()' not found in DLL");
                    FreeLibrary(m_dllHandle);
                    exit(1);
                }

                ReceiveCANMessageFromClient = (LPFNDLLFUNC_ReceiveCANMessageFromClient) GetProcAddress(m_dllHandle, "ReceiveCANMessageFromClient");
                if (!ReceiveCANMessageFromClient)
                {
                    //Trace("Function address for 'ReceiveCANMessageFromClient()' not found in DLL");
                    CleanUp();
                    FreeLibrary(m_dllHandle);
                    exit(1);
                }

                SendCANMessageToClient = (LPFNDLLFUNC_SendCANMessageToClient) GetProcAddress(m_dllHandle, "SendCANMessageToClient");
                if (!SendCANMessageToClient)
                {
                    //Trace("Function address for 'SendCANMessageToClient()' not found in DLL");
                    CleanUp();
                    FreeLibrary(m_dllHandle);
                    exit(1);
                }

                ReceiveCommandFromClient = (LPFNDLLFUNC_ReceiveCommandFromClient) GetProcAddress(m_dllHandle, "ReceiveCommandFromClient");
                if (!ReceiveCommandFromClient)
                {
                    //Trace("Function address for 'ReceiveCommandFromClient()' not found in DLL");
                    CleanUp();
                    FreeLibrary(m_dllHandle);
                    exit(1);
                }

                SendCommandToClient = (LPFNDLLFUNC_SendCommandToClient) GetProcAddress(m_dllHandle, "SendCommandToClient");
                if (!SendCommandToClient)
                {
                    //Trace("Function address for 'SendCommandToClient()' not found in DLL");
                    CleanUp();
                    FreeLibrary(m_dllHandle);
                    exit(1);
                }

                ReceiveTraceFromClient = (LPFNDLLFUNC_ReceiveTraceFromClient) GetProcAddress(m_dllHandle, "ReceiveTraceFromClient");
                if (!ReceiveTraceFromClient)
                {
                    //Trace("Function address for 'ReceiveTraceFromClient()' not found in DLL");
                    CleanUp();
                    FreeLibrary(m_dllHandle);
                    exit(1);
                }

                ReceiveLogFromClient = (LPFNDLLFUNC_ReceiveLogFromClient) GetProcAddress(m_dllHandle, "ReceiveLogFromClient");
                if (!ReceiveLogFromClient)
                {
                    //Trace("Function address for 'ReceiveLogFromClient()' not found in DLL");
                    CleanUp();
                    FreeLibrary(m_dllHandle);
                    exit(1);
                }

                hResult = S_OK;
                m_Ctrl_Status = INITIALISED;
            }
        }
        else
        {
            //Trace("Function address for 'Initialize()' not found in DLL");
            FreeLibrary(m_dllHandle);
            exit(1);
        }
    }
    else
    {
        //Trace("Failed to load Controller_0.dll");
        Sleep(10000);
        exit(1);
    }

    /***************************************** Transmit Controller **********************************************************************/

    HRESULT hResult_tx = S_FALSE;
    m_dllHandle_tx = LoadLibrary("Controller_0.dll");
    if (m_dllHandle_tx != nullptr)
    {
        Initialize_tx = (LPFNDLLFUNC_Initialize_tx)GetProcAddress(m_dllHandle_tx, "Initialize");
        if(Initialize)
        {
            if (Initialize_tx() != 0)
            {
                //Trace("Failed to Initialize");
                FreeLibrary(m_dllHandle_tx);
            }
            else
            {
                CleanUp_tx = (LPFNDLLFUNC_CleanUp_tx) GetProcAddress(m_dllHandle_tx, "CleanUp");
                if (!CleanUp_tx)
                {
                    //Trace("Function address for 'CleanUp()' not found in DLL");
                    FreeLibrary(m_dllHandle_tx);
                    exit(1);
                }

                ReceiveCANMessageFromClient_tx = (LPFNDLLFUNC_ReceiveCANMessageFromClient_tx) GetProcAddress(m_dllHandle_tx, "ReceiveCANMessageFromClient");
                if (!ReceiveCANMessageFromClient_tx)
                {
                    //Trace("Function address for 'ReceiveCANMessageFromClient()' not found in DLL");
                    CleanUp_tx();
                    FreeLibrary(m_dllHandle_tx);
                    exit(1);
                }

                SendCANMessageToClient_tx = (LPFNDLLFUNC_SendCANMessageToClient_tx) GetProcAddress(m_dllHandle_tx, "SendCANMessageToClient");
                if (!SendCANMessageToClient_tx)
                {
                    //Trace("Function address for 'SendCANMessageToClient()' not found in DLL");
                    CleanUp_tx();
                    FreeLibrary(m_dllHandle_tx);
                    exit(1);
                }

                ReceiveCommandFromClient_tx = (LPFNDLLFUNC_ReceiveCommandFromClient_tx) GetProcAddress(m_dllHandle_tx, "ReceiveCommandFromClient");
                if (!ReceiveCommandFromClient_tx)
                {
                    //Trace("Function address for 'ReceiveCommandFromClient()' not found in DLL");
                    CleanUp_tx();
                    FreeLibrary(m_dllHandle_tx);
                    exit(1);
                }

                SendCommandToClient_tx = (LPFNDLLFUNC_SendCommandToClient_tx) GetProcAddress(m_dllHandle_tx, "SendCommandToClient");
                if (!SendCommandToClient_tx)
                {
                    //Trace("Function address for 'SendCommandToClient()' not found in DLL");
                    CleanUp_tx();
                    FreeLibrary(m_dllHandle_tx);
                    exit(1);
                }

                ReceiveTraceFromClient_tx = (LPFNDLLFUNC_ReceiveTraceFromClient_tx) GetProcAddress(m_dllHandle_tx, "ReceiveTraceFromClient");
                if (!ReceiveTraceFromClient_tx)
                {
                    //Trace("Function address for 'ReceiveTraceFromClient()' not found in DLL");
                    CleanUp_tx();
                    FreeLibrary(m_dllHandle_tx);
                    exit(1);
                }

                ReceiveLogFromClient_tx = (LPFNDLLFUNC_ReceiveLogFromClient_tx) GetProcAddress(m_dllHandle_tx, "ReceiveLogFromClient");
                if (!ReceiveLogFromClient_tx)
                {
                    //Trace("Function address for 'ReceiveLogFromClient()' not found in DLL");
                    CleanUp_tx();
                    FreeLibrary(m_dllHandle_tx);
                    exit(1);
                }

                hResult_tx = S_OK;
                m_Ctrl_Status = INITIALISED;
            }
        }
        else
        {
            //Trace("Function address for 'Initialize()' not found in DLL");
            FreeLibrary(m_dllHandle_tx);
            exit(1);
        }
    }
    else
    {
        //Trace("Failed to load Controller_0.dll");
        Sleep(10000);
        exit(1);
    }

    if( (hResult&&hResult_tx)==S_OK)
    {
        return S_OK;
    }
    else
    {
        return S_FALSE;
    }

}

HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_UnloadDriverLibrary(void)
{
    if (m_dllHandle)
    {
        //FreeLibrary(m_dllHandle);
        //Trace("Failed to unload Controller_0.dll");
    }

    if (m_dllHandle_tx)
    {
        //FreeLibrary(m_dllHandle_tx);
        //Trace("Failed to unload Controller_0.dll");
    }

    //Sleep(5000);
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_PerformInitOperations(void)
{
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_PerformClosureOperations(void)
{
    //Terminate the read thread
    sg_sParmRThread.bTerminateThread();
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_GetTimeModeMapping(SYSTEMTIME& /* CurrSysTime */, UINT64& /* TimeStamp */, LARGE_INTEGER& /* QueryTickCount */)
{
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_ListHwInterfaces(INTERFACE_HW_LIST& /* sSelHwInterface */, INT& /* nCount */, PSCONTROLLER_DETAILS InitData)
{
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_SelectHwInterface(const INTERFACE_HW_LIST& /* sSelHwInterface */, INT /* nCount */)
{
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_DeselectHwInterface(void)
{
    return S_OK;
}


HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_SetConfigData(PSCONTROLLER_DETAILS /* InitData */, int /* Length */)
{
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_StartHardware(void)
{
    m_Ctrl_Status = NORMAL_ACTIVE;
    if (sg_sParmRThread.bStartThread(CanMsgReadThreadProc_CAN_ISolar_Eve))
    {
        return S_OK;
    }
    else
    {
        return S_FALSE;
    }
}

HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_StopHardware(void)
{
    sg_sParmRThread.m_unActionCode = EXIT_THREAD;
    SetEvent(sg_sParmRThread.m_hActionEvent);

    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_ResetHardware(void)
{
    m_Ctrl_Status = INITIALISED;
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_GetCntrlStatus(const HANDLE& /* hEvent */, UINT& /* unCntrlStatus */)
{
    return S_OK;
}

HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_SendMsg(DWORD /* dwClientID */, const STCAN_MSG& sCanTxMsg)
{
    HRESULT ret_result = S_FALSE;
    STCAN_MSG sCanTxMsg1;

    sCanTxMsg1.m_bCANFD     = sCanTxMsg.m_bCANFD;
    sCanTxMsg1.m_ucChannel  = sCanTxMsg.m_ucChannel;
    memcpy(sCanTxMsg1.m_ucData, sCanTxMsg.m_ucData,64);
    sCanTxMsg1.m_ucDataLen  = sCanTxMsg.m_ucDataLen;
    sCanTxMsg1.m_ucEXTENDED = sCanTxMsg.m_ucEXTENDED;
    sCanTxMsg1.m_ucRTR      = sCanTxMsg.m_ucRTR;
    sCanTxMsg1.m_unMsgID    = sCanTxMsg.m_unMsgID;

    if (SendCommandToClient_tx(RECEIVE_MESSAGE_ASYNC))
    {
        if (SendCANMessageToClient_tx(&sCanTxMsg1))
        {
            ret_result= S_OK;
        }
    }

    return ret_result;
}

HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_GetLastErrorString(std::string& /* acErrorStr */)
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
HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_GetCurrStatus(STATUSMSG& StatusData)
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
HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_GetTxMsgBuffer(BYTE*& /*pouFlxTxMsgBuffer*/)
{
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
HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_GetControllerParams(LONG& lParam, UINT /* nChannel */, ECONTR_PARAM eContrParam)
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
HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_SetControllerParams(INT nValue, ECONTR_PARAM eContrparam)
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
HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_GetErrorCount(SERROR_CNT& /* sErrorCnt */, UINT /* nChannel */, ECONTR_PARAM /* eContrParam */)
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
HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_SetAppParams(HWND /* hWndOwner */)
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
HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName)
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
                if (_tcscmp(pacClientName, CAN_MONITOR_NODE) == 0)
                {
                    /* First slot is reserved to monitor node */
                    ClientID = 1;
                    _tcscpy(sg_asClientToBufMap[0].pacClientName, pacClientName);
                    sg_asClientToBufMap[0].dwClientID = ClientID;
                    sg_asClientToBufMap[0].unBufCount = 0;
                }
                else
                {
                    //if (!bClientExist(CAN_MONITOR_NODE, Index))
                    //{
                    //    Index = sg_unClientCnt + 1;
                    //}
                    //else
                    //{
                    //    /*Index = sg_unClientCnt;*/
                    //}
                    Index = sg_unClientCnt;
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
* \param[in]     pBufObj, is pointer to CBaseCANBufFSE object
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_ISOLAR_EVE_VCAN::CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj)
{
    HRESULT hResult = S_FALSE;
    if (ClientID != 0)
    {
        UINT unClientIndex;
        if (bGetClientObj(ClientID, unClientIndex))
        {
            SCLIENTBUFMAP& sClientObj = sg_asClientToBufMap[unClientIndex];
            if (bAction == MSGBUF_ADD)
            {
                /* Add msg buffer */
                if (pBufObj != nullptr)
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
                if (pBufObj != nullptr)
                {
                    /* Remove only buffer mentioned */
                    bRemoveClientBuffer(sClientObj.pClientBuf, sClientObj.unBufCount, pBufObj);
                }
                else
                {
                    /* Remove all */
                    for (UINT i = 0; i < sClientObj.unBufCount; i++)
                    {
                        sClientObj.pClientBuf[i] = nullptr;
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
                CAN_ManageMsgBuf(MSGBUF_CLEAR, sg_asClientToBufMap[i].dwClientID, nullptr);
            }
            hResult = S_OK;
        }
    }

    return hResult;
}

HRESULT  CDIL_ISOLAR_EVE_VCAN::CAN_SetHardwareChannel(PSCONTROLLER_DETAILS,DWORD /*dwDriverId*/,bool /*bIsHardwareListed*/, unsigned int /*unChannelCount*/)
{
    return S_OK;
}