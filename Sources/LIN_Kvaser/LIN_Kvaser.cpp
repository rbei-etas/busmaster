/**
 * \file      LIN_Kvaser.cpp
 * \author    a4@uincore.net
 * \copyright Copyright (c) 2015, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#include "LIN_Kvaser_stdafx.h"
#include "LIN_Kvaser_Defines.h"

/* C++ includes */
#include <sstream>
#include <Windows.h>
#include <string>
#include <vector>

/* Project includes */
#include "LIN_Kvaser.h"

#include "HardwareListing.h"
#include "Include/DIL_CommonDefs.h"
#include "DataTypes\Cluster.h"
#include "DIL_Interface\CommanDIL_LIN.h"
#define USAGE_EXPORT
#include "LIN_Kvaser_Extern.h"

BEGIN_MESSAGE_MAP(CLIN_Kvaser, CWinApp)
END_MESSAGE_MAP()

static STLINDATA sg_EVE_LINMsg;
class CDIL_LIN_Kvaser;
CDIL_LIN_Kvaser* g_pouDIL_LIN_Kvaser = nullptr;

CLIN_Kvaser::CLIN_Kvaser(void)
{
}

// The one and only CLIN_Kvaser object
CLIN_Kvaser theApp;

BOOL CLIN_Kvaser::InitInstance()
{
    CWinApp::InitInstance();

    return TRUE;
}

/* CDIL_LIN_Kvaser class definition */
class CDIL_LIN_Kvaser : public CCommanDIL_LIN
{
private:
    HINSTANCE m_dllHandle,m_dllHandle_tx;
    ECONTR_STATUS m_Ctrl_Status;

public:
    /* STARTS IMPLEMENTATION OF THE INTERFACE FUNCTIONS... */
    HRESULT LIN_PerformInitOperations(void);
    HRESULT PerformClosureOperations(void);
    HRESULT LIN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount);
    HRESULT LIN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount);
    HRESULT LIN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount);
    HRESULT LIN_DeselectHwInterface(void);
    HRESULT LIN_DisplayConfigDlg(PSCONTROLLER_DETAILS_LIN InitData, int& Length);
    HRESULT SetConfigData(ClusterConfig& ouConfig);
    HRESULT StartHardware(void);
    HRESULT PreStartHardware(void);
    HRESULT StopHardware(void);
    HRESULT LIN_ResetHardware(void);
    HRESULT LIN_GetCurrStatus(s_STATUSMSG& StatusData);
    HRESULT LIN_GetTxMsgBuffer(BYTE*& pouFlxTxMsgBuffer);
    HRESULT LIN_Send(STLIN_MSG& sLinTxMsg);


    HRESULT LIN_SetSlaveRespData(const STLIN_MSG stRespMsg)
    {
        return S_OK;
    };
    HRESULT LIN_ResetSlaveRespData(void)
    {
        return S_OK;
    };
    HRESULT LIN_DisableSlave(STLIN_MSG& sMessage);
    HRESULT LIN_GetBusConfigInfo(BYTE* BusInfo);
    HRESULT LIN_GetLastErrorString(std::string& acErrorStr);
    HRESULT LIN_FilterFrames(FILTER_TYPE FilterType, TYPE_CHANNEL Channel, UINT* punMsgIds, UINT nLength);
    HRESULT LIN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT LIN_GetConfiguration(sCONTROLLERDETAILSLIN[], INT& nSize);
    HRESULT LIN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam);
    HRESULT LIN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT LIN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog);
    HRESULT LIN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);
    HRESULT LIN_LoadDriverLibrary(void);
    HRESULT LIN_UnloadDriverLibrary(void);

    static DWORD WINAPI LINMsgReadThreadProc_LIN_Kvaser(LPVOID pVoid);
    void ProcessLINMsg(LinMessageInfo msgInf,unsigned char* msg,unsigned int flags,unsigned int len,unsigned int id,int chnindex);
    static int nSetBaudRate();


};

LinHandle sg_arrReadHandles[CHANNEL_ALLOWED];

/**
* \brief         This will classify the messages, which lin be one of Rx, Tx or
*                Error messages. In case of Err messages this identifies under
*                what broader category (Rx / Tx) does this occur.
* \param[in]     XLevent& xlEvent message polled from the bus in XLevent format
* \param[out]    sLinData Application specific data format
* \return        TRUE (always)
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
static INT bClassifyMsgType(LinMessageInfo* msgInf, STLINDATA& sLinData,unsigned char* msg,unsigned int flags,unsigned int len,unsigned int id,int chnindex)
{
    INT nHresult = S_OK;
    sLinData.m_lTickCount.QuadPart = ((LONGLONG)msgInf->timestamp / 100000);

    if (CREATE_MAP_TIMESTAMP == sg_byCurrState)
    {
        LARGE_INTEGER g_QueryTickCount;
        QueryPerformanceCounter(&g_QueryTickCount);
        UINT64 unConnectionTime;
        unConnectionTime = ((g_QueryTickCount.QuadPart * 10000) / sg_lnFrequency.QuadPart) - sg_TimeStamp;
        //Time difference should be +ve value
        if(sLinData.m_lTickCount.QuadPart >= unConnectionTime)
        {
            sg_TimeStamp  = (LONGLONG)(sLinData.m_lTickCount.QuadPart - unConnectionTime);
        }
        else
        {
            sg_TimeStamp  = (LONGLONG)(unConnectionTime - sLinData.m_lTickCount.QuadPart);
        }

        sg_byCurrState = CALC_TIMESTAMP_READY;
    }
    sLinData.m_lTickCount.QuadPart = (LONGLONG)(msgInf->timestamp * 10);
    sLinData.m_uDataInfo.m_sLINMsg.m_ucChannel = (UCHAR)chnindex +1;
    sLinData.m_uDataInfo.m_sErrInfo.m_ucChannel = (UCHAR)chnindex +1;
    sLinData.m_uDataInfo.m_sLINMsg.m_ulTimeStamp = sLinData.m_lTickCount.QuadPart;
    sLinData.m_uDataInfo.m_sErrInfo.m_ulTimeStamp = sLinData.m_lTickCount.QuadPart;
    sLinData.m_ucDataType   = (UCHAR)RX_FLAG;

    switch (flags)
    {
            // LIN events
        case LIN_NODATA:
        {
            sLinData.m_eLinMsgType = LIN_EVENT;
            sLinData.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_ERRNOANS;
            // a4@uincore.net
            // sLinData.m_uDataInfo.m_sErrInfo.m_ucId = xlEvent.tagData.linMsgApi.linNoAns.id;
        }
        break;
        case LIN_WAKEUP_FRAME:
        {
            sLinData.m_uDataInfo.m_sErrInfo.m_ucId = -1;
            sLinData.m_eLinMsgType = LIN_EVENT;
            sLinData.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_WAKEUP;
            // a4@uincore.net
            // sLinData.m_uDataInfo.m_sErrInfo.m_ucId = xlEvent.tagData.linMsgApi.linNoAns.id;
        }
        break;
        case LIN_RX:
        case LIN_TX:
        {
            sLinData.m_eLinMsgType = LIN_MSG;
            sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgID   = id;
            if(flags == LIN_RX)
            {
                sLinData.m_ucDataType   = (UCHAR)RX_FLAG;
            }
            else
            {
                sLinData.m_ucDataType   = (UCHAR)TX_FLAG;
            }
            /* Copy the message data */
            memcpy(sLinData.m_uDataInfo.m_sLINMsg.m_ucData,
                   msg, len);
            sLinData.m_uDataInfo.m_sLINMsg.m_ucDataLen = len;
            break;
        }

        case LIN_PARITY_ERROR:
            sLinData.m_uDataInfo.m_sErrInfo.m_ucId = -1;
            sLinData.m_eLinMsgType = LIN_EVENT;

            sLinData.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_ERRMSG;
            break;

        case LIN_SYNCH_ERROR:
            sLinData.m_uDataInfo.m_sErrInfo.m_ucId = -1;
            sLinData.m_eLinMsgType = LIN_EVENT;
            sLinData.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_ERRSYNC;
            break;

        case LIN_CSUM_ERROR:
            sLinData.m_uDataInfo.m_sErrInfo.m_ucId = -1;
            sLinData.m_eLinMsgType = LIN_EVENT;

            sLinData.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_ERRCRC;
            break;

        case LIN_BIT_ERROR:
            sLinData.m_uDataInfo.m_sErrInfo.m_ucId = -1;
            sLinData.m_eLinMsgType = LIN_EVENT;

            sLinData.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_ERRBIT;
            break;
        default:
            int a = 0;
            break;
    }

    return nHresult;
}

/**
* \brief         Processing of the received packets from bus
* \param[in]     LinMessageInfo msgInf message polled from the bus in LinMessageInfo format
* \param[in]     unClientIndex is the client index
* \return        void
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
void CDIL_LIN_Kvaser::ProcessLINMsg(LinMessageInfo msgInf,unsigned char* msg,unsigned int flags,unsigned int len,unsigned int id,int chnindex)
{
    if ( S_OK == bClassifyMsgType(&msgInf, sg_asLINMsg,msg,flags,len,id, chnindex) )
    {
        vWriteIntoClientsBuffer(sg_asLINMsg);
    }
}

/**
* \brief         Read thread procedure
* \param[in]     pVoid contains the CPARAM_THREADPROC class object
* \return        void
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
DWORD WINAPI CDIL_LIN_Kvaser::LINMsgReadThreadProc_LIN_Kvaser(LPVOID pVoid)
{
    USES_CONVERSION;

    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    LinStatus xlStatus = linOK;

    /* Assign thread action to CREATE_TIME_MAP */
    pThreadParam->m_unActionCode = CREATE_TIME_MAP;

    //get Lin - eventHandles
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        sg_arrReadHandles[i] = linBusOn(sg_aodChannels[i].m_hnd);
    }

    /* Get the handle to the controller and validate it */
    // VALIDATE_POINTER_RETURN_VALUE_LOG(pThreadParam->m_hActionEvent, (DWORD)-1);

    DWORD dwResult = 0;
    unsigned int msgsrx;
    unsigned int id;
    unsigned char msg[8];
    unsigned int len;
    unsigned int flags;
    LinMessageInfo msgInfo;

    bool bLoopON = true;

    while (bLoopON)
    {
        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                bool bMoreDataExist;
                msgsrx = RECEIVE_EVENT_SIZE;
                for (UINT i = 0; i < sg_nNoOfChannels; i++)
                {
                    xlStatus = linReadMessageWait(sg_aodChannels[i].m_hnd, &id, msg, &len, &flags, &msgInfo, 1);
                    while (xlStatus == linOK )
                    {
                        switch (xlStatus)
                        {
                            case linOK:
                            {
                                if ( g_pouDIL_LIN_Kvaser != nullptr )
                                {
                                    g_pouDIL_LIN_Kvaser->ProcessLINMsg(msgInfo,msg,flags,len,id,i);
                                    bMoreDataExist = true;
                                }
                            }
                            break;

                            default:
                                break;
                        }
                        xlStatus = linReadMessageWait(sg_aodChannels[i].m_hnd, &id, msg, &len, &flags, &msgInfo, 1);
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
                //vCreateTimeModeMapping(pThreadParam->m_hActionEvent);
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
    pThreadParam->m_hActionEvent = nullptr;

    return 0;
}

/**
* \brief         Returns the CDIL_LIN_Kvaser object
* \param[out]    ppvInterface, is void pointer to take back the reference to CDIL_LIN_Kvaser object
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
USAGEMODE HRESULT GetIDIL_LIN_Controller(void** ppvInterface)
{
    HRESULT hResult = S_OK;
    if ( nullptr == g_pouDIL_LIN_Kvaser )
    {
        if ((g_pouDIL_LIN_Kvaser = new CDIL_LIN_Kvaser) == nullptr)
        {
            hResult = S_FALSE;
        }
    }
    *ppvInterface = (void*) g_pouDIL_LIN_Kvaser;  /* Doesn't matter even if g_pouDIL_LIN_Kvaser is null */

    return hResult;
}

/**************************** Helper APIs *****************************************/

/**
* \brief         Function to create time mode mapping
* \param[in]     hDataEvent, is HANDLE
* \return        void
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
static void vCreateTimeModeMapping(HANDLE hDataEvent)
{
    //WaitForSingleObject(hDataEvent, INFINITE);
    GetLocalTime(&sg_CurrSysTime);
    /*Query Tick Count*/
    QueryPerformanceCounter(&sg_QueryTickCount);
}

/**
* \brief         Finds the number of hardware connected.
* \param         void
* \return        defERR_OK if successful otherwise corresponding Error code.
*                0, Query successful, but no device found
*                > 0, Number of devices found
*                < 0, query for devices unsuccessful
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
static int nGetNoOfConnectedHardware(void)
{
    int nChannelCount = 0;
    canStatus nStatus;

    // ------------------------------------
    // get number of present channels
    // ------------------------------------
    nStatus = canGetNumberOfChannels((int*)&nChannelCount);

    if (nStatus != canOK )
    {
        _tcscpy(sg_omErrStr, _("Problem Finding Device!"));
        nChannelCount = -1;
    }
    /* Return the channel count */
    return nChannelCount;
}

/**
* \brief         This function will popup hardware selection dialog and gets the user selection of channels.Not supported now.
* \param[in]     psInterfaces, is INTERFACE_HW structue
* \param[out]    pnSelList, contains channels selected array
* \param[out]    nCount, contains selected channel count
* \return        returns 0 if success, else -1
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
int ListHardwareInterfaces(HWND hParent, DWORD /*dwDriver*/, INTERFACE_HW* psInterfaces, int* pnSelList, int& nCount)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CWnd objMainWnd;
    objMainWnd.Attach(hParent);
    CHardwareListing HwList(psInterfaces, nCount, pnSelList, &objMainWnd);
    INT nRet = HwList.DoModal();
    objMainWnd.Detach();

    if ( nRet == IDOK)
    {
        nCount = HwList.nGetSelectedList(pnSelList);
        return 0;
    }
    else
    {
        return -1;
    }
}

/**
* \brief         This function will get the hardware selection from the user
*                and will create essential networks.
* \param         unDefaultChannelCnt
* \return        returns defERR_OK (always)
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
static int nCreateMultipleHardwareNetwork(UINT unDefaultChannelCnt = 0)
{
    int nHwCount = sg_ucNoOfHardware;
    DWORD dwFirmWare[2];
    char chBuffer[512] = "";
    std::string name;
    int i = 0;
    // Get Hardware Network Map
    for (int nCount = 0; nCount < nHwCount; nCount++)
    {
        canGetChannelData(nCount, canCHANNELDATA_CHANNEL_NAME,
                          chBuffer, sizeof(chBuffer));
        name = chBuffer;

        if(name.find(" LIN ",0)!=std::string::npos)
        {
            sg_HardwareIntr[nCount].m_acDescription = chBuffer;

            sg_HardwareIntr[nCount].m_dwIdInterface = i;

            canGetChannelData(nCount, canCHANNELDATA_CARD_SERIAL_NO,
                              chBuffer, sizeof(chBuffer));
            sscanf_s( chBuffer, "%ld", &sg_HardwareIntr[nCount].m_dwVendor );

            //Get Firmware info
            canGetChannelData(nCount, canCHANNELDATA_CARD_FIRMWARE_REV, dwFirmWare, sizeof(dwFirmWare));

            sprintf_s(chBuffer, sizeof(chBuffer), "0x%08lx 0x%08lx", dwFirmWare[0], dwFirmWare[1]);
            sg_HardwareIntr[nCount].m_acDeviceName = chBuffer;
            //sprintf(sg_HardwareIntr[nCount].m_acDeviceName,"0x%08lx 0x%08lx", dwFirmWare[0], dwFirmWare[1]);

            canGetChannelData(nCount, canCHANNELDATA_CHANNEL_FLAGS, chBuffer, sizeof(chBuffer));
            i++;
        }
    }
    nHwCount = i;

    /* If the default channel count parameter is set, prevent displaying the hardware selection dialog */
    if ( unDefaultChannelCnt && nHwCount >= unDefaultChannelCnt )
    {
        for (UINT i = 0; i < unDefaultChannelCnt; i++)
        {
            sg_anSelectedItems[i] = i;
        }
        nHwCount = unDefaultChannelCnt;
    }
    else if ( ListHardwareInterfaces(sg_hOwnerWnd, DRIVER_LIN_KVASER, sg_HardwareIntr, sg_anSelectedItems, nHwCount) != 0 )
    {
        return HW_INTERFACE_NO_SEL;
    }
    sg_ucNoOfHardware = (UCHAR)nHwCount;
    sg_nNoOfChannels = (UINT)nHwCount;

    //Reorder hardware interface as per the user selection
    for (int nCount = 0; nCount < sg_ucNoOfHardware; nCount++)
    {
        sg_aodChannels[nCount].m_nChannel = sg_HardwareIntr[sg_anSelectedItems[nCount]].m_dwIdInterface;
        sprintf(sg_aodChannels[nCount].m_strName , _("Kvaser - %s, Serial Number- %ld, Firmware- %s"),
                sg_HardwareIntr[sg_anSelectedItems[nCount]].m_acDescription.c_str(),
                sg_HardwareIntr[sg_anSelectedItems[nCount]].m_dwVendor,
                sg_HardwareIntr[sg_anSelectedItems[nCount]].m_acDeviceName.c_str());
    }

    return defERR_OK;
}

/**
* \brief         This function will create a single network with available single hardware.
* \param         void
* \return        returns defERR_OK (always)
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
static int nCreateSingleHardwareNetwork()
{
    /* Set the number of channels as 1 */
    sg_ucNoOfHardware = (UCHAR)1;
    sg_nNoOfChannels = 1;

    sg_aodChannels[0].m_nChannel = 0;

    /* Update channel info */

    //char acVendor[MAX_CHAR_LONG];
    char chBuffer[512];
    DWORD dwFirmWare[2];

    canGetChannelData(0, canCHANNELDATA_CARD_SERIAL_NO,
                      chBuffer, sizeof(chBuffer));
    sscanf_s( chBuffer, "%ld", &sg_HardwareIntr[0].m_dwVendor );

    canGetChannelData(0, canCHANNELDATA_CHANNEL_NAME,
                      chBuffer, sizeof(chBuffer));

    sg_HardwareIntr[0].m_acDescription = chBuffer;

    if(sg_HardwareIntr[0].m_acDescription.find(" LIN ",0)!=std::string::npos)
    {

        /* Get Firmware info */
        canGetChannelData(0, canCHANNELDATA_CARD_FIRMWARE_REV, dwFirmWare, sizeof(dwFirmWare));

        sprintf(chBuffer,"0x%08lx 0x%08lx", dwFirmWare[0], dwFirmWare[1]);
        sg_HardwareIntr[0].m_acDeviceName = chBuffer;

        sprintf(sg_aodChannels[0].m_strName , _("%s, Serial Number: %ld, Firmware: %s"),
                sg_HardwareIntr[0].m_acDescription.c_str(),
                sg_HardwareIntr[0].m_dwVendor,
                sg_HardwareIntr[0].m_acDeviceName.c_str());
    }
    else
    {
        sg_ucNoOfHardware = (UCHAR)0;
        sg_nNoOfChannels = 0;
        return HW_INTERFACE_NO_SEL;
    }


    return defERR_OK;
}

/**
* \brief         This function will find number of hardwares connected.
*                It will create network as per hardware count.
*                This will popup hardware selection dialog in case there are more hardwares present.
* \param         unDefaultChannelCnt
* \return        Operation Result. 0 incase of no errors. Failure Error codes otherwise.
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
static int nInitHwNetwork(UINT unDefaultChannelCnt)
{
    int nChannelCount = 0;
    int nResult = NO_HW_INTERFACE;

    /* Select Hardware */
    nChannelCount = nGetNoOfConnectedHardware();
    if(nChannelCount == -1)
    {
        nResult = -1;
        return nResult;
    }

    // Assign the channel count
    sg_ucNoOfHardware = (UCHAR)nChannelCount;

    /* Capture only Driver Not Running event
     * Take action based on number of Hardware Available
     */

    /* No Hardware found */
    if( nChannelCount == 0 )
    {
        //MessageBox(nullptr,sg_omErrStr, nullptr, MB_OK | MB_ICONERROR);
        nChannelCount = -1;
    }
    /* Available hardware is lesser then the supported channels */
    else
    {
        // Check whether channel selection dialog is required
        if( nChannelCount > 1)
        {
            // Get the selection from the user. This will also
            // create and assign the networks
            nResult = nCreateMultipleHardwareNetwork(unDefaultChannelCnt);
        }
        else
        {
            // Use available one hardware
            nResult = nCreateSingleHardwareNetwork();
        }
    }
    return nResult;
}


/**
* \brief         Function to retreive error occurred and log it
* \param[in]     File, pointer to log file
* \param[in]     Line, indicates line number in log file
* \return        void
* \authors       a4@uincore.net
* \date          05.29.2015 Created
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


/****************** Implementation for CDIL_LIN_Kvaser class ***********************/

/**
* \brief         Loads vendor's driver library
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_LoadDriverLibrary(void)
{
    return S_OK;
}

/**
* \brief         UnLoads vendor's driver library
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_UnloadDriverLibrary(void)
{
    return S_OK;
}

/**
* \brief         Init
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_PerformInitOperations(void)
{
    HRESULT hResult = S_FALSE;

    // ------------------------------------
    // Initialize the linlib driver libray
    // ------------------------------------
    try
    {
        linInitializeLibrary();
    }
    catch (...)
    {
        //AfxMessageBox("Initialize failed");
    }

    //Initialize the selected channel items array to -1
    for ( UINT i = 0; i< CHANNEL_ALLOWED; i++ )
    {
        sg_anSelectedItems[i] = -1;
    }

    hResult = S_OK;
    return hResult;
}

/**
* \brief         Closure
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::PerformClosureOperations(void)
{
    return S_OK;
}

/**
* \brief         Time Mode Mapping
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount)
{
    CurrSysTime = sg_CurrSysTime;
    TimeStamp = sg_TimeStamp;
    QueryTickCount = sg_QueryTickCount;

    return S_OK;
}

/**
* \brief         List hardware
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_ListHwInterfaces(INTERFACE_HW_LIST& asSelHwInterface, INT& nCount)
{
    USES_CONVERSION;
    HRESULT hResult = S_FALSE;

    hResult = nInitHwNetwork(nCount);
    if ( hResult == 0)
    {
        nCount = sg_ucNoOfHardware;
        hResult = S_OK;
    }
    else
    {
        //sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("Error connecting to driver"));
    }
    return hResult;
}

/**
* \brief         Select hardware
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount)
{
	if(nCount==0)
	{
		return ERR_LOAD_DRIVER;
	}
    return S_OK;
}

/**
* \brief         Deselect hardware
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_DeselectHwInterface(void)
{

    return S_OK;
}

/**
* \brief         Display config dialog
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_DisplayConfigDlg(PSCONTROLLER_DETAILS_LIN InitData, int& Length)
{
    return S_OK;
}

/**
* \brief         Get Protocol Version
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
UINT nGetProtocolVersion(std::string& strProtocol)
{
    UINT unVer = LIN_VERSION_2_1;
    if ( strProtocol == "LIN 1.1" || strProtocol == "LIN 1.2" || strProtocol == "LIN 1.1" || strProtocol == "LIN 1.3" )
    {
        unVer = LIN_VERSION_1_3;
    }
    else if ( strProtocol == "LIN 2.0" )
    {
        unVer = LIN_VERSION_2_0;
    }
    /*else  ( strProtocol == "LIN 2.1"  )
    {
        unVer = XL_LIN_VERSION_2_1;
    }*/
    return unVer;
}

/**
* \brief         SetConfigData
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::SetConfigData(ClusterConfig& ouConfig)
{
    for ( int i = 0 ; i < ouConfig.m_nChannelsConfigured; i++ )
    {
        sg_aodChannels[i].m_unBaudrate = ouConfig.m_ouFlexChannelConfig[i].m_ouLinParams.m_nBaudRate;
        sg_aodChannels[i].m_unLINVersion = nGetProtocolVersion(ouConfig.m_ouFlexChannelConfig[i].m_ouLinParams.m_strProtocolVersion);
        sg_aodChannels[i].m_strLinVersion = ouConfig.m_ouFlexChannelConfig[i].m_ouLinParams.m_strProtocolVersion;
        sg_aodChannels[i].m_unLINMode = ouConfig.m_ouFlexChannelConfig[i].m_ouLinParams.m_bIsMasterMode ? LIN_MASTER:LIN_SLAVE ;//XL_LIN_MASTER;
    }
    return S_OK;
}

/**
* \brief         This will close the connection with the driver. This will be
*                called before deleting HI layer. This will be called during
*                application close.
* \param         void
* \return        Operation Result. 0 incase of no errors. Failure Error codes(-1) otherwise.
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
static int nDisconnectFromDriver()
{
    int nReturn = 0;
    LinStatus xlStatus;

    for ( UINT i = 0; i< sg_nNoOfChannels; i++ )
    {
        xlStatus = linBusOff(sg_aodChannels[i].m_hnd);
        if (xlStatus < 0)
        {
            nReturn = -1;
        }
        xlStatus = linClose(sg_aodChannels[i].m_hnd);
        if (xlStatus < 0)
        {
            nReturn = -1;
        }
    }

    sg_bCurrState = STATE_HW_INTERFACE_SELECTED;

    return nReturn;
}

/**
* \brief         Function to set the channel baud rate configured by user
* \param         void
* \return        linOK if succeeded, else respective error code
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
static int nSetBaudRate()
{
    LinStatus xlStatus = linERR_NOMSG;

    /* Set baud rate to all available hardware */
    for ( UINT unIndex = 0; unIndex < sg_nNoOfChannels; unIndex++)
    {
        // Get Current channel reference
        CChannel& odChannel = sg_aodChannels[ unIndex ];
        if( odChannel.m_hnd >= 0 )
        {
            unsigned int rate = odChannel.m_unBaudrate;
            xlStatus = linSetBitrate(odChannel.m_hnd,rate);
        }
        else
        {
            vRetrieveAndLog(xlStatus, __FILE__, __LINE__);
            // Invalid Hardware Handle. Could be a simulation network.
            // Wrong call
        }
        // Check for failure
        if( xlStatus != linOK )
        {
            // break the loop
            unIndex = sg_nNoOfChannels;
        }
    }

    return xlStatus;
}

/**
* \brief         This function will connect the tool with hardware. This will
*                establish the data link between the application and hardware.
* \param[in]     bConnect TRUE to Connect, FALSE to Disconnect
* \return        Returns defERR_OK if successful otherwise corresponding Error code.
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
static int nConnect(BOOL bConnect)
{
    int nReturn = -1;
    LinStatus xlStatus;
    LinHandle  hnd;

    if (!sg_bIsConnected && bConnect) // Disconnected and to be connected
    {
        /* Set the permission mask for all channel access */
        for( UINT i = 0; i < sg_nNoOfChannels; i++)
        {
            //open LIN channel
            hnd =  linOpenChannel(sg_aodChannels[i].m_nChannel,sg_aodChannels[i].m_unLINMode);

            if (hnd >= 0)
            {
                sg_aodChannels[i].m_hnd = hnd;
            }
            else
            {
                return S_FALSE;
            }
        }
        nReturn = nSetBaudRate();
        if(nReturn == defERR_OK)
        {
            for( UINT i = 0; i < sg_nNoOfChannels; i++)
            {
                xlStatus = linBusOn(sg_aodChannels[i].m_hnd);
                if (xlStatus < 0)
                {
                    return S_FALSE;
                }
                else
                {
                    sg_aodChannels[i].m_nIsOnBus = 1;
                }
            }
            // Update configuration to restore the settings
            /* Transit into 'CREATE TIME MAP' state */
            sg_byCurrState = CREATE_MAP_TIMESTAMP;
            sg_bIsConnected = bConnect;
        }
    }
    else if (sg_bIsConnected && !bConnect) // Connected & to be disconnected
    {
        sg_bIsConnected = bConnect;
        Sleep(0); // Let other threads run for once
        nReturn = nDisconnectFromDriver();
    }
    else
    {
        nReturn = defERR_OK;
    }
    if ( sg_bIsConnected )
    {
        InitializeCriticalSection(&sg_CritSectForWrite);
        //Calculate connected Timestamp
        QueryPerformanceCounter(&sg_QueryTickCount);
        // Get frequency of the performance counter
        QueryPerformanceFrequency(&sg_lnFrequency);
        // Convert it to time stamp with the granularity of hundreds of microsecond
        //if (sg_QueryTickCount.QuadPart * 10000 > sg_QueryTickCount.QuadPart)
        if ((sg_QueryTickCount.QuadPart * 10000) > sg_lnFrequency.QuadPart)
        {
            sg_TimeStamp = (sg_QueryTickCount.QuadPart * 10000) / sg_lnFrequency.QuadPart;
        }
        else
        {
            sg_TimeStamp = (sg_QueryTickCount.QuadPart / sg_lnFrequency.QuadPart) * 10000;
        }
    }
    else
    {
        DeleteCriticalSection(&sg_CritSectForWrite);
    }

    return nReturn;
}

/**
* \brief         StartHardware
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::StartHardware(void)
{
    // VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    // USES_CONVERSION;
    HRESULT hResult = S_OK;

    //Connect to the network
    hResult = nConnect(TRUE);
    if (hResult == defERR_OK)
    {
        hResult = S_OK;
        sg_bCurrState = STATE_CONNECTED;
        // SetEvent(g_hDataEvent);
        vCreateTimeModeMapping(0);
    }
    else
    {
        //log the error for open port failure
        //vRetrieveAndLog(hResult, __FILE__, __LINE
        for ( int i = 0; i < 16; i++ )
        {
            // a4@uincore.net
            // sg_aodChannels[i].m_nMapIdChecksumType.clear();
            m_MapIdClient[i].clear();
        }

        hResult = ERR_LOAD_HW_INTERFACE;
    }

    //If everything is ok start the read thread
    if (hResult == S_OK)
    {
        sg_sParmRThread.m_pBuffer = this;
        if (sg_sParmRThread.bStartThread(LINMsgReadThreadProc_LIN_Kvaser))
        {
            hResult = S_OK;
        }
        else
        {
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _("Could not start the read thread"));
        }
    }

    return hResult;
}

/**
* \brief         PreStartHardware
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::PreStartHardware(void)
{
    return S_OK;
}

/**
* \brief         StopHardware
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::StopHardware(void)
{
    // VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_OK;

    //Terminate the read thread
    sg_sParmRThread.bTerminateThread();
    hResult = nConnect(FALSE);
    if (hResult == defERR_OK)
    {
        hResult = S_OK;
        sg_bCurrState = STATE_HW_INTERFACE_SELECTED;
    }
    else
    {
        //log the error for open port failure
        vRetrieveAndLog(hResult, __FILE__, __LINE__);
        hResult = ERR_LOAD_HW_INTERFACE;
    }

    return hResult;
}

/**
* \brief         ResetHardware
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_ResetHardware(void)
{
    m_Ctrl_Status = INITIALISED;
    return S_OK;
}

/**
* \brief         GetCntrlStatus
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus)
{

    return S_OK;
}

/**
* \brief         This will send a LIN message to the driver. In case of USB
*                this will write the message in to the driver buffer and will
*                return. In case if parallel port mode this will write the
*                message and will wait for the ACK event from the driver. If
*                the event fired this will return 0. Otherwise this will return
*                wait time out error. In parallel port it is a blocking call
*                and in case of failure condition this will take 2 seconds.
* \param[in]     sMessage Message to Transmit
* \return        Operation Result. 0 incase of no errors. Failure Error codes otherwise.
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
static int nWriteMessage(STLIN_MSG sMessage, DWORD dwClientID)
{
    int nReturn = -1;
    if ((sMessage.m_ucChannel > 0) &&
            (sMessage.m_ucChannel <= sg_nNoOfChannels))
    {
        //Get channel mask
        if ( sMessage.m_ucMsgTyp == LIN_SLAVE_RESPONSE )
        {
            LinStatus xlStatus = linUpdateMessage(sg_aodChannels[sMessage.m_ucChannel-1].m_hnd,sMessage.m_ucMsgID,sMessage.m_ucData, sMessage.m_ucDataLen);
            //set result
            nReturn = xlStatus;
        }
        else
        {
            LinStatus xlStatus = linWriteMessage(sg_aodChannels[sMessage.m_ucChannel-1].m_hnd,sMessage.m_ucMsgID,sMessage.m_ucData, sMessage.m_ucDataLen);
            //set result
            nReturn = xlStatus;
        }

    }

    return nReturn;
}

/**
* \brief         LIN_DisableSlave
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_DisableSlave(STLIN_MSG& sMessage)
{
    // linBusOff(g_ChannelHandel);
    // g_ChannelHandel = 0;
    LinStatus xlStatus;
    xlStatus = linClose(sg_aodChannels[sMessage.m_ucChannel].m_hnd);
    if(xlStatus<0)
    {
        return S_FALSE;
    }
    linOpenChannel(sg_aodChannels[sMessage.m_ucChannel].m_hnd,LIN_MASTER);
    if(sg_aodChannels[sMessage.m_ucChannel].m_hnd <0)
    {
        return S_FALSE;
    }
    return S_OK;
}

/**
* \brief         LIN_Send
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_Send( STLIN_MSG& sMessage)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

    HRESULT hResult = S_FALSE;
    if (true)
    {
        if (sMessage.m_ucChannel <= sg_nNoOfChannels)
        {
            EnterCriticalSection(&sg_CritSectForWrite); // Lock the buffer
            if (nWriteMessage(sMessage, 0) == defERR_OK)
            {
                hResult = S_OK;
            }
            LeaveCriticalSection(&sg_CritSectForWrite); // Unlock the buffer
        }
        else
        {
            hResult = ERR_INVALID_CHANNEL;
        }
    }
    else
    {
        hResult = ERR_NO_CLIENT_EXIST;
    }

    return hResult;
}

/**
* \brief         LIN_GetLastErrorString
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_GetLastErrorString(std::string& acErrorStr)
{

    return S_OK;
}

/**
* \brief         LIN_FilterFrames
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_FilterFrames(FILTER_TYPE FilterType, TYPE_CHANNEL Channel, UINT* punMsgIds, UINT nLength)
{
    return S_OK;
}

/**
* \brief         Function to get Controller status
* \param[out]    StatusData, is s_STATUSMSG structure
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_GetCurrStatus(s_STATUSMSG& StatusData)
{
    StatusData.wControllerStatus = m_Ctrl_Status;

    return S_OK;
}

/**
* \brief         Gets the Tx queue configured.
* \param[out]    pouFlxTxMsgBuffer, is BYTE*
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_GetTxMsgBuffer(BYTE*& /*pouFlxTxMsgBuffer*/)
{
    return S_OK;
}
/**
* \brief         LIN_GetConfiguration
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_GetConfiguration(sCONTROLLERDETAILSLIN psControllerConfig[], INT& nSize)
{
    for ( int i = 0; i < sg_nNoOfChannels; i++ )
    {
        psControllerConfig[i].m_strHwUri =  sg_HardwareIntr[i].m_acDescription.c_str();
        psControllerConfig[i].m_nBaudRate =  sg_aodChannels[i].m_unBaudrate;
        // a4@uincore.net
        // psControllerConfig[i].m_strProtocolVersion = sg_aodChannels[i].m_strLinVersion;
    }
    nSize = sg_nNoOfChannels;
    return S_OK;
}

/**
* \brief         Gets the controller parametes of the channel based on the request.
* \param[out]    lParam, the value of the controller parameter requested.
* \param[in]     nChannel, indicates channel ID
* \param[in]     eContrParam, indicates controller parameter
* \return        S_OK for success, S_FALSE for failure
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
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

            if (nChannel < sg_nNoOfChannels)
            {
                lParam = defMODE_ACTIVE;
            }
            else
            {
                //unknown
                lParam = defCONTROLLER_BUSOFF + 1;
            }


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
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_SetControllerParams(INT nValue, ECONTR_PARAM eContrparam)
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
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
{
    HRESULT hResult = S_OK;
    return hResult;
}

/**
* \brief         Sets the application params.
* \param[in]     hWndOwner, is the main frame HWND value
* \param[in]     pILog, is pointer to error logger object
* \return        S_OK (always)
* \authors       a4@uincore.net
* \date          05.29.2015 Created
*/
HRESULT CDIL_LIN_Kvaser::LIN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog)
{
    sg_pIlog = pILog;
    return S_OK;
}

