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
 * \file      LIN_Vector_XL.cpp
 * \brief     Source file for ISOLAR EVE VIRTUAL LIN DIL functions
 * \author    Manohar H
 * \copyright Copyright (c) 2011, ETAS GmbH. All rights reserved.
 */
// LIN_Vector_XL.cpp : Defines the initialization routines for the DLL.
//

#include "LIN_Vector_XL_stdafx.h"
#include "LIN_Vector_XL_Defines.h"

/* C++ includes */
#include <sstream>
#include <Windows.h>
#include <string>
#include <vector>

/* Project includes */
#include "LIN_Vector_XL.h"
#include "HardwareListing.h"
#include "Include/DIL_CommonDefs.h"
#include "DataTypes\Cluster.h"
#include "DIL_Interface\CommanDIL_LIN.h"
#define USAGE_EXPORT
#include "LIN_Vector_XL_Extern.h"

using namespace std;

// CISOLAR_EVE_VLIN

BEGIN_MESSAGE_MAP(CLIN_Vector_XL, CWinApp)
END_MESSAGE_MAP()


static STLINDATA sg_EVE_LINMsg;
class CDIL_LIN_VectorXL;
CDIL_LIN_VectorXL* g_pouDIL_LIN_Vector_XL = NULL;
/**
 * CLIN_Vector_XL construction
 */
CLIN_Vector_XL::CLIN_Vector_XL()
{
    // Place all significant initialization in InitInstance
}


// The one and only CLIN_Vector_XL object
CLIN_Vector_XL theApp;


/**
 * CLIN_Vector_XL initialization
 */

BOOL CLIN_Vector_XL::InitInstance()
{
    CWinApp::InitInstance();

    return TRUE;
}

/* CDIL_LIN_VectorXL class definition */
class CDIL_LIN_VectorXL : public CCommanDIL_LIN
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
    HRESULT LIN_Send(STLIN_MSG& sCanTxMsg);


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
    HRESULT LIN_GetLastErrorString(string& acErrorStr);
    HRESULT LIN_FilterFrames(FILTER_TYPE FilterType, TYPE_CHANNEL Channel, UINT* punMsgIds, UINT nLength);
    HRESULT LIN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT LIN_GetConfiguration(sCONTROLLERDETAILSLIN[], INT& nSize);

    //MVN
    HRESULT LIN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam);
    //~MVN
    HRESULT LIN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);

    // Specific function set
    HRESULT LIN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog);
    HRESULT LIN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);
    HRESULT LIN_LoadDriverLibrary(void);
    HRESULT LIN_UnloadDriverLibrary(void);

    static DWORD WINAPI LINMsgReadThreadProc_LIN_Vector_XL(LPVOID pVoid);
    void ProcessLINMsg(XLevent& xlEvent);


};


/**
* \brief         This will classify the messages, which can be one of Rx, Tx or
*                Error messages. In case of Err messages this identifies under
*                what broader category (Rx / Tx) does this occur.
* \param[in]     XLevent& xlEvent message polled from the bus in XLevent format
* \param[out]    sCanData Application specific data format
* \return        TRUE (always)
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static INT bClassifyMsgType(XLevent& xlEvent, STLINDATA& sLinData)
{
    INT nHresult = S_OK;
    sLinData.m_lTickCount.QuadPart = ((LONGLONG)xlEvent.timeStamp / 100000);

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
    sLinData.m_uDataInfo.m_sLINMsg.m_ucChannel = (UCHAR)sg_unDevChannelMap[xlEvent.chanIndex] + 1;
    sLinData.m_uDataInfo.m_sErrInfo.m_ucChannel = (UCHAR)sg_unDevChannelMap[xlEvent.chanIndex] + 1;
    sLinData.m_uDataInfo.m_sLINMsg.m_ulTimeStamp = sLinData.m_lTickCount.QuadPart;
    sLinData.m_uDataInfo.m_sErrInfo.m_ulTimeStamp = sLinData.m_lTickCount.QuadPart;


    sLinData.m_ucDataType   = (UCHAR)RX_FLAG;
    switch (xlEvent.tag)
    {
            // LIN events
        case XL_LIN_NOANS:
        {
            sLinData.m_eLinMsgType = LIN_EVENT;
            sLinData.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_ERRNOANS;
            sLinData.m_uDataInfo.m_sErrInfo.m_ucId = xlEvent.tagData.linMsgApi.linNoAns.id;
        }
        break;
        case XL_LIN_CRCINFO:
        {
            if ( sg_unDevChannelMap[xlEvent.chanIndex] >= 0 && sg_unDevChannelMap[xlEvent.chanIndex] < defNO_OF_CHANNELS )
            {
                sg_aodChannels[sLinData.m_uDataInfo.m_sLINMsg.m_ucChannel-1].m_nMapIdChecksumType.insert(map<int,int>::value_type(xlEvent.tagData.linMsgApi.linCRCinfo.id, xlEvent.tagData.linMsgApi.linCRCinfo.flags));
            }
            nHresult = S_FALSE;
        }
        break;
        case XL_LIN_MSG:
        {
            sLinData.m_eLinMsgType = LIN_MSG;

            sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgID   = xlEvent.tagData.linMsgApi.linMsg.id;
            map<int,int>::iterator itr = sg_aodChannels[sLinData.m_uDataInfo.m_sLINMsg.m_ucChannel-1].m_nMapIdChecksumType.find(sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgID);

            if ( itr != sg_aodChannels[sLinData.m_uDataInfo.m_sLINMsg.m_ucChannel-1].m_nMapIdChecksumType.end() )
            {
                sLinData.m_uDataInfo.m_sLINMsg.m_ucChksumTyp = LIN_CHECKSUM_ENHANCED;
                if ( itr->second == XL_LIN_CHECKSUM_CLASSIC )
                {
                    sLinData.m_uDataInfo.m_sLINMsg.m_ucChksumTyp = LIN_CHECKSUM_CLASSIC;
                }
            }
            else
            {
                sLinData.m_uDataInfo.m_sLINMsg.m_ucChksumTyp = LIN_CHECKSUM_CLASSIC;
            }





            /* Message Direction */
            if (xlEvent.tagData.linMsgApi.linMsg.flags & XL_LIN_MSGFLAG_TX)
            {
                sLinData.m_ucDataType   = (UCHAR)TX_FLAG;
            }
            else if ( xlEvent.tagData.linMsgApi.linMsg.flags & XL_LIN_MSGFLAG_CRCERROR )
            {
                sLinData.m_eLinMsgType = LIN_EVENT;
                sLinData.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_ERRCRC;
                sLinData.m_uDataInfo.m_sErrInfo.m_ucId = xlEvent.tagData.linMsgApi.linMsg.id;
                sLinData.m_uDataInfo.m_sErrInfo.m_ucCrc = xlEvent.tagData.linMsgApi.linMsg.crc;
            }
            else
            {
                sLinData.m_ucDataType   = (UCHAR)RX_FLAG;
            }

            /* Set channel ID */

            sLinData.m_uDataInfo.m_sLINMsg.m_ucChksum  = xlEvent.tagData.linMsgApi.linMsg.crc;

            /* Copy the message data */
            memcpy(sLinData.m_uDataInfo.m_sLINMsg.m_ucData,
                   xlEvent.tagData.linMsgApi.linMsg.data, xlEvent.tagData.linMsgApi.linMsg.dlc);

            sLinData.m_uDataInfo.m_sLINMsg.m_ucDataLen = xlEvent.tagData.linMsgApi.linMsg.dlc;


            if ( sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgID  == 0x3C )
            {
                int a = 0;
            }
            break;
        }

        case XL_LIN_SLEEP:
            sLinData.m_uDataInfo.m_sErrInfo.m_ucId = -1;
            sLinData.m_eLinMsgType = LIN_EVENT;
            sLinData.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_SLEEP;
            switch ( xlEvent.tagData.linMsgApi.linSleep.flag)
            {
                case XL_LIN_SET_SLEEPMODE:
                    sLinData.m_uDataInfo.m_sErrInfo.m_eSleepEvent = LIN_SLEEPMODE_SET;
                    break;
                case XL_LIN_COMESFROM_SLEEPMODE:
                    sLinData.m_uDataInfo.m_sErrInfo.m_eSleepEvent = LIN_SLEEPMODE_WAKEUP;
                    break;
                case XL_LIN_STAYALIVE:
                default:
                    sLinData.m_uDataInfo.m_sErrInfo.m_eSleepEvent = LIN_SLEEPMODE_STAYALIVE;
                    break;
            }
            nHresult = S_OK;
            break;

        case XL_LIN_ERRMSG:
            sLinData.m_uDataInfo.m_sErrInfo.m_ucId = -1;
            sLinData.m_eLinMsgType = LIN_EVENT;

            sLinData.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_ERRMSG;
            break;

        case XL_LIN_SYNCERR:
            sLinData.m_uDataInfo.m_sErrInfo.m_ucId = -1;
            sLinData.m_eLinMsgType = LIN_EVENT;
            sLinData.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_ERRSYNC;
            break;

        case XL_LIN_WAKEUP:
            sLinData.m_uDataInfo.m_sErrInfo.m_ucId = -1;
            sLinData.m_eLinMsgType = LIN_EVENT;
            sLinData.m_uDataInfo.m_sErrInfo.m_eEventType = EVENT_LIN_WAKEUP;
            break;
        default:
            int a = 0;
            break;
    }

    return nHresult;
}

/**
* \brief         Writes the message 'sLinData' to the corresponding clients buffer
* \param[out]    sLinData, is STLINDATA structure
* \param[out]    unClientIndex , is UINT containing client ID
* \return        void
* \authors       Arunkumar Karri
* \date          07.29.2013 Created
*/
/*static void vWriteIntoClientsBuffer(STLINDATA& sLinData, UINT unClientIndex)
{

    for (UINT j = 0; j < sg_asClientToBufMap[unClientIndex].unBufCount; j++)
    {
        sg_asClientToBufMap[unClientIndex].pClientBuf[j]->WriteIntoBuffer(&sLinData);
    }
}*/

/**
* \brief         Processing of the received packets from bus
* \param[in]     XLevent& xlEvent message polled from the bus in XLevent format
* \param[in]     unClientIndex is the client index
* \return        void
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
void CDIL_LIN_VectorXL::ProcessLINMsg(XLevent& xlEvent)
{
    if ( S_OK == bClassifyMsgType(xlEvent, sg_asLINMsg) )
    {
        vWriteIntoClientsBuffer(sg_asLINMsg);
    }
}

/**
* \brief         Read thread procedure
* \param[in]     pVoid contains the CPARAM_THREADPROC class object
* \return        void
* \authors       Arunkumar Karri
* \date          07.19.2013 Created
*/
DWORD WINAPI CDIL_LIN_VectorXL::LINMsgReadThreadProc_LIN_Vector_XL(LPVOID pVoid)
{
    USES_CONVERSION;

    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    XLstatus xlStatus = XL_SUCCESS;

    /* Assign thread action to CREATE_TIME_MAP */
    pThreadParam->m_unActionCode = CREATE_TIME_MAP;

    /* Set the event to LIN_Vector_XL driver for wakeup and frame arrival notification */
    //for (UINT i = 0; i < sg_unClientCnt; i++)
    {
        xlStatus = xlSetNotification (g_xlPortHandle[0], &g_hDataEvent[0], 1);
    }

    /* Set the thread action event to first notification event handle */
    if (g_hDataEvent[0] != NULL)
    {
        pThreadParam->m_hActionEvent = g_hDataEvent[0];
    }


    /* Get the handle to the controller and validate it */
    VALIDATE_POINTER_RETURN_VALUE_LOG(pThreadParam->m_hActionEvent, (DWORD)-1);

    DWORD dwResult = 0;
    unsigned int msgsrx;
    XLevent  xlEvent;

    bool bLoopON = true;

    while (bLoopON)
    {
        WaitForSingleObject(g_hDataEvent[0], INFINITE);
        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                bool bMoreDataExist;
                msgsrx = RECEIVE_EVENT_SIZE;
                xlStatus = xlReceive(g_xlPortHandle[0], &msgsrx, &xlEvent);
                switch (xlStatus)
                {
                    case XL_SUCCESS:
                    {
                        if ( g_pouDIL_LIN_Vector_XL != NULL )
                        {
                            g_pouDIL_LIN_Vector_XL->ProcessLINMsg(xlEvent);
                            bMoreDataExist = true;
                        }
                    }
                    break;

                    default:
                        break;
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
    SetEvent(pThreadParam->hGetExitNotifyEvent());

    {
        ResetEvent(g_hDataEvent[0]);
        g_hDataEvent[0] = NULL;
    }
    pThreadParam->m_hActionEvent = NULL;

}



/**
* \brief         Returns the CDIL_LIN_VectorXL object
* \param[out]    ppvInterface, is void pointer to take back the reference to CDIL_LIN_VectorXL object
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          08.03.2013 Created
*/
USAGEMODE HRESULT GetIDIL_LIN_Controller(void** ppvInterface)
{
    HRESULT hResult = S_OK;
    if ( NULL == g_pouDIL_LIN_Vector_XL )
    {
        if ((g_pouDIL_LIN_Vector_XL = new CDIL_LIN_VectorXL) == NULL)
        {
            hResult = S_FALSE;
        }
    }
    *ppvInterface = (void*) g_pouDIL_LIN_Vector_XL;  /* Doesn't matter even if g_pouDIL_LIN_Vector_XL is null */

    return hResult;
}

/**************************** Helper APIs *****************************************/

/**
* \brief         Function to create time mode mapping
* \param[in]     hDataEvent, is HANDLE
* \return        void
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
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
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nGetNoOfConnectedHardware(void)
{
    int nResult = 0;
    XLstatus xlStatus = XL_SUCCESS;
    unsigned int    hwType;
    unsigned int    hwIndex;
    unsigned int    hwChannel;
    int             channelIndex;

    // ------------------------------------
    // get the hardware configuration
    // ------------------------------------
    if ( XL_SUCCESS == xlStatus )
    {
        xlStatus = xlGetDriverConfig(&g_xlDrvConfig);
    }

    if (XL_SUCCESS == xlStatus)
    {
        // ------------------------------------
        // select the wanted channels
        // ------------------------------------
        for (UINT i=0; i < g_xlDrvConfig.channelCount; i++)
        {
            /*xlStatus = xlGetApplConfig(g_AppName, i, &hwType, &hwIndex, &hwChannel, XL_BUS_TYPE_LIN);

            if (xlStatus != XL_SUCCESS)
            {
                hwChannel = i;
                // check PC for hardware with LINCabs or LINPiggy's
                if (g_xlDrvConfig.channel[i].channelBusCapabilities & XL_BUS_ACTIVE_CAP_LIN)
                {
                    hwType = g_xlDrvConfig.channel[i].hwType;

                    xlStatus = xlSetApplConfig(                 // Registration of Application with default settings
                        g_AppName, // Application Name
                        i,     // Application channel 0 or 1
                        hwType,         // hwType  (CANcardXL...)
                        hwIndex,        // Index of hardware (slot) (0,1,...)
                        hwChannel,      // Index of channel (connector) (0,1,...)
                        XL_BUS_TYPE_LIN);       // the application is for LIN.
                }
            }

            channelIndex = xlGetChannelIndex(hwType, hwIndex, hwChannel);*/

            // check if we have a valid LIN cab/piggy
            if (g_xlDrvConfig.channel[i].channelBusCapabilities & XL_BUS_ACTIVE_CAP_LIN)
            {
                // and check the right hardwaretype
                //if (g_xlDrvConfig.channel[i].hwType==hwType)
                {
                    nResult++;
                }
            }
        }
        if (!nResult)
        {
            _tcscpy(sg_omErrStr, _T(_("No available channels found! (e.g. no LINcabs...)\r\n")));
            xlStatus = XL_ERROR;
        }
    }
    else
    {
        _tcscpy(sg_omErrStr, _T(_("Problem Finding Device!")));
        nResult = -1;
    }
    /* Return the operation result */
    return nResult;
}

/**
* \brief         This function will popup hardware selection dialog and gets the user selection of channels.
* \param[in]     psInterfaces, is INTERFACE_HW structue
* \param[out]    pnSelList, contains channels selected array
* \param[out]    nCount, contains selected channel count
* \return        returns 0 if success, else -1
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
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
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nCreateMultipleHardwareNetwork(UINT unDefaultChannelCnt = 0)
{
    int nHwCount = sg_ucNoOfHardware;
    int nChannels = 0;
    // Get Hardware Network Map
    for (unsigned int nCount = 0; nCount < g_xlDrvConfig.channelCount; nCount++)
    {
        // we take all hardware we found and
        // check that we have only CAN cabs/piggy's
        // at the moment there is no VN8910 XLAPI support!
        if ( /*(g_xlDrvConfig.channel[nCount].channelBusCapabilities & XL_BUS_ACTIVE_CAP_CAN)
              && */(g_xlDrvConfig.channel[nCount].hwType != XL_HWTYPE_VN8900) )
        {
            if ( /*(g_xlDrvConfig.channel[nCount].hwType == XL_HWTYPE_CANCASEXL) &&*/
                !(g_xlDrvConfig.channel[nCount].channelBusCapabilities & XL_BUS_ACTIVE_CAP_LIN) )
            {
                continue;
            }
            sg_HardwareIntr[nChannels].m_dwIdInterface = nCount;
            sg_HardwareIntr[nChannels].m_dwVendor = g_xlDrvConfig.channel[nCount].serialNumber;
            /*_stprintf(acTempStr, _T("SN: %d, Port ID: %d"), sg_HardwareIntr[nChannels].m_dwVendor,
                                                                    sg_HardwareIntr[nChannels].m_dwIdInterface);*/
            sg_HardwareIntr[nChannels].m_acDescription = g_xlDrvConfig.channel[nCount].name;
            nChannels++;
        }
    }
    nHwCount = nChannels;   //Reassign hardware count according to final list of channels supported.

    /* If the default channel count parameter is set, prevent displaying the hardware selection dialog */
    if ( unDefaultChannelCnt && nChannels >= unDefaultChannelCnt )
    {
        for (UINT i = 0; i < unDefaultChannelCnt; i++)
        {
            sg_anSelectedItems[i] = i;
        }
        nHwCount = unDefaultChannelCnt;
    }
    else if ( ListHardwareInterfaces(sg_hOwnerWnd, DRIVER_CAN_VECTOR_XL, sg_HardwareIntr, sg_anSelectedItems, nHwCount) != 0 )
    {
        return HW_INTERFACE_NO_SEL;
    }
    sg_ucNoOfHardware = (UCHAR)nHwCount;
    sg_nNoOfChannels = (UINT)nHwCount;
    g_xlChannelMask = 0;
    //Reorder hardware interface as per the user selection
    for (int nCount = 0; nCount < sg_ucNoOfHardware; nCount++)
    {
        sg_aodChannels[nCount].m_pXLChannelInfo  = &g_xlDrvConfig.channel[sg_anSelectedItems[nCount]];
        g_xlChannelMask |= sg_aodChannels[nCount].m_pXLChannelInfo->channelMask;
    }
    g_xlPermissionMask = g_xlChannelMask;

    return defERR_OK;
}

/**
* \brief         This function will create a single network with available single hardware.
* \param         void
* \return        returns defERR_OK (always)
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nCreateSingleHardwareNetwork()
{
    g_xlChannelMask = 0;

    // Set the number of channels as 1
    sg_ucNoOfHardware = (UCHAR)1;
    sg_nNoOfChannels = 1;

    for (UINT i=0; i < g_xlDrvConfig.channelCount; i++)
    {
        // we take all hardware we found and
        // check that we have only CAN cabs/piggy's
        // at the moment there is no VN8910 XLAPI support!
        if ( (g_xlDrvConfig.channel[i].channelBusCapabilities & XL_BUS_ACTIVE_CAP_LIN)
                && (g_xlDrvConfig.channel[i].hwType != XL_HWTYPE_VN8900) )                  //TODO
        {
            sg_aodChannels[0].m_pXLChannelInfo  = &g_xlDrvConfig.channel[i];
            g_xlChannelMask |= sg_aodChannels[0].m_pXLChannelInfo->channelMask;
            break;
        }
    }

    g_xlPermissionMask = g_xlChannelMask;

    return defERR_OK;
}

/**
* \brief         This function will find number of hardwares connected.
*                It will create network as per hardware count.
*                This will popup hardware selection dialog in case there are more hardwares present.
* \param         unDefaultChannelCnt
* \return        Operation Result. 0 incase of no errors. Failure Error codes otherwise.
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
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
    char acNo_Of_Hw[MAX_STRING] = {0};
    _stprintf(acNo_Of_Hw, _T(_("Number of Vector hardwares Available: %d")), nChannelCount);

    /* No Hardware found */
    if( nChannelCount == 0 )
    {
        //MessageBox(NULL,sg_omErrStr, NULL, MB_OK | MB_ICONERROR);
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


/****************** Implementation for CDIL_LIN_VectorXL class ***********************/

/**
* \brief         Loads vendor's driver library
* \param         void
* \return        S_OK for success, S_FALSE for failure
* \authors       Prince Varghese
* \date          08.03.2013 Created
*/
HRESULT CDIL_LIN_VectorXL::LIN_LoadDriverLibrary(void)
{
    HRESULT hResult = S_OK;

    if (hxlDll != NULL)
    {
        sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T(_("vxlapi.dll already loaded")));
        hResult = DLL_ALREADY_LOADED;
    }

    if (hResult == S_OK)
    {
        hxlDll = LoadLibrary("vxlapi.dll");
        if (hxlDll == NULL)
        {
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T(_("vxlapi.dll loading failed")));
            hResult = ERR_LOAD_DRIVER;
        }
        else
        {
            xlDllOpenDriver           = (XLOPENDRIVER)                GetProcAddress(hxlDll,"xlOpenDriver");
            xlDllCloseDriver          = (XLCLOSEDRIVER)               GetProcAddress(hxlDll,"xlCloseDriver");

            /* bus independed functions */
            xlGetApplConfig            = (XLGETAPPLCONFIG)            GetProcAddress(hxlDll,"xlGetApplConfig");
            xlSetApplConfig            = (XLSETAPPLCONFIG)            GetProcAddress(hxlDll,"xlSetApplConfig");
            xlGetDriverConfig          = (XLGETDRIVERCONFIG)          GetProcAddress(hxlDll,"xlGetDriverConfig");
            xlGetChannelIndex          = (XLGETCHANNELINDEX)          GetProcAddress(hxlDll,"xlGetChannelIndex");
            xlGetChannelMask           = (XLGETCHANNELMASK)           GetProcAddress(hxlDll,"xlGetChannelMask");
            xlOpenPort                 = (XLOPENPORT)                 GetProcAddress(hxlDll,"xlOpenPort");
            xlSetTimerRate             = (XLSETTIMERRATE)             GetProcAddress(hxlDll,"xlSetTimerRate");
            xlResetClock               = (XLRESETCLOCK)               GetProcAddress(hxlDll,"xlResetClock");
            xlSetNotification          = (XLSETNOTIFICATION)          GetProcAddress(hxlDll,"xlSetNotification");
            xlFlushReceiveQueue        = (XLFLUSHRECEIVEQUEUE)        GetProcAddress(hxlDll,"xlFlushReceiveQueue");
            xlGetReceiveQueueLevel     = (XLGETRECEIVEQUEUELEVEL)     GetProcAddress(hxlDll,"xlGetReceiveQueueLevel");
            xlActivateChannel          = (XLACTIVATECHANNEL)          GetProcAddress(hxlDll,"xlActivateChannel");
            xlReceive                  = (XLRECEIVE)                  GetProcAddress(hxlDll,"xlReceive");
            xlGetEventString           = (XLGETEVENTSTRING)           GetProcAddress(hxlDll,"xlGetEventString");
            xlGetErrorString           = (XLGETERRORSTRING)           GetProcAddress(hxlDll,"xlGetErrorString");
            xlGenerateSyncPulse        = (XLGENERATESYNCPULSE)        GetProcAddress(hxlDll,"xlGenerateSyncPulse");
            xlGetSyncTime              = (XLGETSYNCTIME)              GetProcAddress(hxlDll,"xlGetSyncTime");
            xlPopupHwConfig            = (XLPOPUPHWCONFIG)            GetProcAddress(hxlDll,"xlPopupHwConfig");
            xlDeactivateChannel        = (XLDEACTIVATECHANNEL)        GetProcAddress(hxlDll,"xlDeactivateChannel");
            xlClosePort                = (XLCLOSEPORT)                GetProcAddress(hxlDll,"xlClosePort");
            xlSetGlobalTimeSync        = (XLSETGLOBALTIMESYNC)        GetProcAddress(hxlDll, "xlSetGlobalTimeSync");


            /* LIN specific functions */
            xlLinSetChannelParams      = (XLLINSETCHANNELPARAMS)    GetProcAddress(hxlDll,"xlLinSetChannelParams");
            xlLinSetDLC                = (XLLINSETDLC)              GetProcAddress(hxlDll,"xlLinSetDLC");
            xlLinSetSlave              = (XLLINSETSLAVE)            GetProcAddress(hxlDll,"xlLinSetSlave");
            xlLinSendRequest           = (XLLINSENDREQUEST)         GetProcAddress(hxlDll,"xlLinSendRequest");
            xlLinSetSleepMode          = (XLLINSETSLEEPMODE)        GetProcAddress(hxlDll,"xlLinSetSleepMode");
            xlLinWakeUp                = (XLLINWAKEUP)              GetProcAddress(hxlDll,"xlLinWakeUp");
            xlLinSetChecksum           = (XLLINSETCHECKSUM)         GetProcAddress(hxlDll,"xlLinSetChecksum");
            xlLinSwitchSlave           = (XLLINSWITCHSLAVE)         GetProcAddress(hxlDll,"xlLinSwitchSlave");

            /* check for error */
            if (!xlDllOpenDriver || !xlDllCloseDriver ||
                    !xlGetApplConfig || !xlSetApplConfig ||
                    !xlGetDriverConfig || !xlGetChannelIndex ||
                    !xlGetChannelMask || !xlOpenPort ||
                    !xlSetTimerRate || !xlResetClock ||
                    !xlSetNotification || !xlFlushReceiveQueue ||
                    !xlGetReceiveQueueLevel || !xlActivateChannel ||
                    !xlReceive || !xlGetEventString ||
                    !xlGetErrorString || !xlGenerateSyncPulse ||
                    !xlGetSyncTime || !xlPopupHwConfig ||
                    !xlDeactivateChannel || !xlClosePort ||
                    !xlLinSetChannelParams || !xlLinSetDLC ||
                    !xlLinSetSlave || !xlLinSendRequest ||
                    !xlLinSetSleepMode || !xlLinWakeUp ||
                    !xlLinSetChecksum || !xlLinSwitchSlave)
            {
                FreeLibrary(hxlDll);
                sg_pIlog->vLogAMessage(A2T(__FILE__),
                                       __LINE__, _T(_("Getting Process address of the APIs failed")));
                hResult = ERR_LOAD_DRIVER;
            }
        }
    }

    return hResult;
}

HRESULT CDIL_LIN_VectorXL::LIN_UnloadDriverLibrary(void)
{
    if (hxlDll != NULL)
    {
        FreeLibrary(hxlDll);
        hxlDll = NULL;
    }

    return S_OK;
}

HRESULT CDIL_LIN_VectorXL::LIN_PerformInitOperations(void)
{
    HRESULT hResult = S_FALSE;
    /* Register Monitor client */
    DWORD dwClientID = 0;
    LIN_RegisterClient(TRUE, dwClientID, LIN_MONITOR_NODE);

    // ------------------------------------
    // open the driver
    // ------------------------------------
    if (xlDllOpenDriver() == XL_SUCCESS)
    {
        hResult = S_OK;
    }

    //Initialize the selected channel items array to -1
    for ( UINT i = 0; i< CHANNEL_ALLOWED; i++ )
    {
        sg_anSelectedItems[i] = -1;
    }
    return hResult;
}

HRESULT CDIL_LIN_VectorXL::LIN_PerformClosureOperations(void)
{
    return S_OK;
}

HRESULT CDIL_LIN_VectorXL::LIN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount)
{
    memcpy(&CurrSysTime, &sg_CurrSysTime, sizeof(SYSTEMTIME));
    TimeStamp = sg_TimeStamp;
    if(QueryTickCount != NULL)
    {
        *QueryTickCount = sg_QueryTickCount;
    }

    return S_OK;
}

HRESULT CDIL_LIN_VectorXL::LIN_ListHwInterfaces(INTERFACE_HW_LIST& asSelHwInterface, INT& nCount)
{
    USES_CONVERSION;
    HRESULT hResult = S_FALSE;

    hResult = nInitHwNetwork(nCount);
    if ( hResult == 0)
    {
        nCount = sg_nNoOfChannels;
        for (UINT i = 0; i < sg_nNoOfChannels; i++)
        {
            asSelHwInterface[i].m_dwIdInterface = i;
            unsigned int serialNumber = sg_aodChannels[i].m_pXLChannelInfo->serialNumber;
            ostringstream oss;
            oss << dec << serialNumber;
            string strTemp =  oss.str();
            asSelHwInterface[i].m_acDescription = strTemp;
            //_stprintf(asSelHwInterface[i].m_acDescription, _T("%d"), serialNumber);
            ostringstream oss1;
            oss1 << "Vector - " << sg_aodChannels[i].m_pXLChannelInfo->name << " SN - " <<serialNumber;
            oss1 << "Channel Index - " <<(int)sg_aodChannels[i].m_pXLChannelInfo->channelIndex;
            sg_ControllerDetails[i].m_omHardwareDesc = oss1.str();

            sg_bCurrState = STATE_HW_INTERFACE_LISTED;
        }
        hResult = S_OK;
    }
    else
    {
        //sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T(_("Error connecting to driver")));
    }
    return hResult;
}

HRESULT CDIL_LIN_VectorXL::LIN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount)
{
    USES_CONVERSION;

    VALIDATE_POINTER_RETURN_VAL(hxlDll, S_FALSE);

    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_LISTED, ERR_IMPROPER_STATE);

    /* Check for the success */
    sg_bCurrState = STATE_HW_INTERFACE_SELECTED;

    return S_OK;
}

HRESULT CDIL_LIN_VectorXL::LIN_DeselectHwInterface(void)
{

    return S_OK;
}

HRESULT CDIL_LIN_VectorXL::LIN_DisplayConfigDlg(PSCONTROLLER_DETAILS_LIN InitData, int& Length)
{

    return S_OK;
}

UINT nGetProtocolVersion(string& strProtocol)
{
    UINT unVer = XL_LIN_VERSION_2_1;
    if ( strProtocol == "LIN 1.1" || strProtocol == "LIN 1.2" || strProtocol == "LIN 1.1" || strProtocol == "LIN 1.3" )
    {
        unVer = XL_LIN_VERSION_1_3;
    }
    else if ( strProtocol == "LIN 2.0" )
    {
        unVer = XL_LIN_VERSION_2_0;
    }
    /*else  ( strProtocol == "LIN 2.1"  )
    {
        unVer = XL_LIN_VERSION_2_1;
    }*/
    return unVer;
}


HRESULT CDIL_LIN_VectorXL::LIN_SetConfigData(ClusterConfig& ouConfig)
{
    for ( int i = 0 ; i < ouConfig.m_nChannelsConfigured; i++ )
    {
        sg_aodChannels[i].m_unBaudrate = ouConfig.m_ouFlexChannelConfig[i].m_ouLinParams.m_nBaudRate;
        sg_aodChannels[i].m_unLINVersion = nGetProtocolVersion(ouConfig.m_ouFlexChannelConfig[i].m_ouLinParams.m_strProtocolVersion);
        sg_aodChannels[i].m_strLinVersion = ouConfig.m_ouFlexChannelConfig[i].m_ouLinParams.m_strProtocolVersion;
        sg_aodChannels[i].m_unLINMode = XL_LIN_SLAVE ;//XL_LIN_MASTER;
    }


    return S_OK;
}

/**
* \brief         This will close the connection with the driver. This will be
*                called before deleting HI layer. This will be called during
*                application close.
* \param         void
* \return        Operation Result. 0 incase of no errors. Failure Error codes(-1) otherwise.
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nDisconnectFromDriver()
{
    int nReturn = 0;
    XLstatus xlStatus;

    if (g_xlPortHandle[0] != XL_INVALID_PORTHANDLE)
    {
        xlStatus = xlDeactivateChannel( g_xlPortHandle[0], g_xlChannelMask );
        xlStatus = xlClosePort(g_xlPortHandle[0]);
        //SSH + fix for Issue# 393 - cannot disconnect bus master in some scenarios.
        //g_xlPortHandle[i] = XL_INVALID_PORTHANDLE;
        //SSH -
    }
    else
    {
        nReturn = -1;
    }
    sg_bCurrState = STATE_HW_INTERFACE_SELECTED;

    return nReturn;
}

/**
* \brief         Function to map device channels with BUSMASTER channel order
* \param         void
* \return        void
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static void vMapDeviceChannelIndex()
{
    //Reset previous channel ID assignment if any
    for (UINT i = 0; i < sg_nNoOfChannels; i++)
    {
        sg_unDevChannelMap[sg_aodChannels[i].m_pXLChannelInfo->channelIndex] = i;
    }
}

/**
* \brief         Function to set the channel baud rate configured by user
* \param         void
* \return        canOK if succeeded, else respective error code
* \authors       Arunkumar Karri
* \date          07.26.2013 Created
*/
static int nSetBaudRate()
{
    XLstatus xlStatus;
    XLaccess xlChanMaskTx = 0;
    XLlinStatPar xlLinStatusVar;
    static int nLINMode = XL_LIN_SLAVE;
    //if ( nLINMode > XL_LIN_SLAVE )
    //  nLINMode = XL_LIN_MASTER;

    /* Set baud rate to all available hardware */
    for ( UINT unIndex = 0; unIndex < sg_nNoOfChannels; unIndex++)
    {
        xlStatus = 0;
        // Get Current channel reference
        CChannel& odChannel = sg_aodChannels[ unIndex ];
        xlLinStatusVar.baudrate     = odChannel.m_unBaudrate;
        xlLinStatusVar.LINMode      = odChannel.m_unLINMode;
        xlLinStatusVar.LINVersion   = odChannel.m_unLINVersion;

        //Get channel mask
        xlChanMaskTx = sg_aodChannels[unIndex].m_pXLChannelInfo->channelMask;
        sg_aodChannels[unIndex].m_nMapIdChecksumType.clear();
        // Set the baud rate
        xlStatus = xlLinSetChannelParams(g_xlPortHandle[unIndex], xlChanMaskTx, xlLinStatusVar);

        /*unsigned char checksum[60];
        for (int i = 0; i < 60; i++)
        {
            checksum[i] = XL_LIN_CHECKSUM_CLASSIC;
        }
        xlStatus = xlLinSetChecksum(g_xlPortHandle[unIndex], xlChanMaskTx, checksum);*/

        // ---------------------------------------
        // Setup the Master DLC's
        // ---------------------------------------
        unsigned char   DLC[64];
        // set the DLC for all ID's to DEFAULT_LIN_DLC
        for (int i=0; i<64; i++)
        {
            DLC[i] = DEFAULT_LIN_DLC;
        }

        //xlStatus = xlLinSetDLC(g_xlPortHandle[unIndex], xlChanMaskTx, DLC);


        //if ( nLINMode++ == XL_LIN_SLAVE )
        {
            /* Set Slave IDs */
            unsigned char data[8];
            unsigned char id = 0x00;
            unsigned char dlc = 8;
            for (int i = 0; i <= 63; i++)
            {
                id      = i;
                memset(data, 0, dlc);

                /*if ( id >= 60 && id <= 63 )
                {
                    xlStatus = xlLinSetSlave(g_xlPortHandle[unIndex],xlChanMaskTx,
                    id, data, dlc, XL_LIN_CHECKSUM_CLASSIC);
                }
                else
                {
                    /*if ( odChannel.m_unLINVersion == XL_LIN_VERSION_1_3 )
                    {
                        xlStatus = xlLinSetSlave(g_xlPortHandle[unIndex],xlChanMaskTx,
                            id, data, dlc, XL_LIN_CALC_CHECKSUM_ENHANCED);
                    }
                    else
                    {
                        xlStatus = xlLinSetSlave(g_xlPortHandle[unIndex],xlChanMaskTx,
                            id, data, dlc, XL_LIN_CALC_CHECKSUM);
                    }

                    xlStatus = xlLinSetSlave(g_xlPortHandle[unIndex],xlChanMaskTx,
                            id, data, dlc, XL_LIN_CALC_CHECKSUM_ENHANCED);

                }*/
                //xlStatus = xlLinSetSlave(g_xlPortHandle[unIndex],xlChanMaskTx,
                //          0x29, data, 8, XL_LIN_CALC_CHECKSUM_ENHANCED);
                //xlStatus = xlLinSetSlave(g_xlPortHandle[unIndex],xlChanMaskTx,
                //      0x12, data, 2, XL_LIN_CALC_CHECKSUM_ENHANCED);
                if ( xlStatus !=  XL_SUCCESS )
                {
                    return 0;
                }

                //xlLinSwitchSlave(g_xlPortHandle[unIndex], xlChanMaskTx, 0X12, XL_LIN_SLAVE_ON);
                //xlLinSwitchSlave(g_xlPortHandle[unIndex], xlChanMaskTx, 0X29, XL_LIN_SLAVE_ON);

            }
        }

        /* Check for failure */
        if( xlStatus != XL_SUCCESS )
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
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
static int nConnect(BOOL bConnect)
{
    int nReturn = -1;
    XLstatus xlStatus;

    if (!sg_bIsConnected && bConnect) // Disconnected and to be connected
    {
        /* Set the permission mask for all channel access */
        g_xlPermissionMask = g_xlChannelMask;


        xlStatus = xlOpenPort(&g_xlPortHandle[0], g_AppName, g_xlChannelMask, &g_xlPermissionMask,
                              RX_QUEUE_SIZE, XL_INTERFACE_VERSION, XL_BUS_TYPE_LIN);

        if (XL_SUCCESS == xlStatus)
        {
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

            /* Transit into 'CREATE TIME MAP' state */
            sg_byCurrState = CREATE_MAP_TIMESTAMP;
            vMapDeviceChannelIndex();
            sg_bIsConnected = bConnect;

            xlFlushReceiveQueue(g_xlPortHandle[0]);
            /* Set LIN channel Params */
            nReturn = nSetBaudRate();
        }
        else
        {
            return S_FALSE;
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

    }
    else
    {
        DeleteCriticalSection(&sg_CritSectForWrite);
    }

    return nReturn;
}
HRESULT CDIL_LIN_VectorXL::LIN_StartHardware(void)
{
    // ------------------------------------
    // go with all selected channels on bus
    // ------------------------------------
    XLstatus xlStatus = XL_ERROR;
    HRESULT nReturn = S_FALSE;

    xlStatus = xlActivateChannel(g_xlPortHandle[0], g_xlChannelMask, XL_BUS_TYPE_LIN, XL_ACTIVATE_RESET_CLOCK);

    if(xlStatus == XL_SUCCESS)
    {
        nReturn = defERR_OK;
        //nSetApplyConfiguration();
    }
    return nReturn;
}
HRESULT CDIL_LIN_VectorXL::LIN_PreStartHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    USES_CONVERSION;
    HRESULT hResult = S_OK;

    //Connect to the network
    hResult = nConnect(TRUE);
    if (hResult == defERR_OK)
    {
        hResult = S_OK;
        sg_bCurrState = STATE_CONNECTED;
        vCreateTimeModeMapping(g_hDataEvent[0]);
    }
    else
    {
        //log the error for open port failure
        //vRetrieveAndLog(hResult, __FILE__, __LINE
        for ( int i = 0; i < 16; i++ )
        {
            sg_aodChannels[i].m_nMapIdChecksumType.clear();
            m_MapIdClient[i].clear();
        }

        hResult = ERR_LOAD_HW_INTERFACE;
    }

    //If everything is ok start the read thread
    if (hResult == S_OK)
    {
        sg_sParmRThread.m_pBuffer = this;
        if (sg_sParmRThread.bStartThread(LINMsgReadThreadProc_LIN_Vector_XL))
        {
            hResult = S_OK;
        }
        else
        {
            sg_pIlog->vLogAMessage(A2T(__FILE__), __LINE__, _T(_("Could not start the read thread") ));
        }
    }

    return hResult;
}

HRESULT CDIL_LIN_VectorXL::LIN_StopHardware(void)
{
    VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_CONNECTED, ERR_IMPROPER_STATE);

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

HRESULT CDIL_LIN_VectorXL::LIN_ResetHardware(void)
{
    m_Ctrl_Status = INITIALISED;
    return S_OK;
}

HRESULT CDIL_LIN_VectorXL::LIN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus)
{

    return S_OK;
}

static int nGetCalcChecksum(int nChannel, int nId )
{
    int nChecksum = XL_LIN_CALC_CHECKSUM_ENHANCED;

    if ((nChannel >= 0) && (nChannel < sg_nNoOfChannels))
    {
        if ( sg_aodChannels[nChannel ].m_unLINVersion <= XL_LIN_VERSION_1_3 || ( nId >= 60 && nId <= 63 ) )
        {
            nChecksum = XL_LIN_CALC_CHECKSUM;
        }
    }

    return nChecksum;
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
* \authors       Arunkumar Karri
* \date          07.26.2013 Created
*/
static int nWriteMessage(STLIN_MSG sMessage, DWORD dwClientID)
{
    int nReturn = -1;
    XLaccess xlChanMaskTx = 0;
    UINT unClientIndex = (UINT)-1;
    XLstatus xlStatus;

    if ((sMessage.m_ucChannel > 0) &&
            (sMessage.m_ucChannel <= sg_nNoOfChannels))
    {
        //Get channel mask
        xlChanMaskTx = sg_aodChannels[sMessage.m_ucChannel - 1].m_pXLChannelInfo->channelMask;
        //Transmit message

        xlLinSwitchSlave(g_xlPortHandle[0], xlChanMaskTx, sMessage.m_ucMsgID, XL_LIN_SLAVE_ON);
        //xlLinSwitchSlave(g_xlPortHandle[unClientIndex], xlChanMaskTx, 0X12, XL_LIN_SLAVE_ON);

        xlLinSetSlave(g_xlPortHandle[0], xlChanMaskTx, sMessage.m_ucMsgID, sMessage.m_ucData, sMessage.m_ucDataLen, nGetCalcChecksum(sMessage.m_ucChannel - 1, sMessage.m_ucMsgID));
        //xlLinSendRequest(g_xlPortHandle[unClientIndex], xlChanMaskTx, sMessage.m_ucMsgID, 0);

        //set result
        nReturn = xlStatus;
    }

    return S_OK;
}
HRESULT CDIL_LIN_VectorXL::LIN_DisableSlave(STLIN_MSG& sMessage)
{
    HRESULT hResult = S_FALSE;
    UINT unClientIndex = 0;
    XLaccess xlChanMaskTx = sg_aodChannels[sMessage.m_ucChannel - 1].m_pXLChannelInfo->channelMask;
    xlLinSwitchSlave(g_xlPortHandle[unClientIndex], xlChanMaskTx, sMessage.m_ucMsgID, XL_LIN_SLAVE_OFF);
    return 0;
}

HRESULT CDIL_LIN_VectorXL::LIN_Send( STLIN_MSG& sMessage)
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

HRESULT CDIL_LIN_VectorXL::LIN_GetLastErrorString(string& acErrorStr)
{

    return S_OK;
}

HRESULT CDIL_LIN_VectorXL::LIN_FilterFrames(FILTER_TYPE FilterType, TYPE_CHANNEL Channel, UINT* punMsgIds, UINT nLength)
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
HRESULT CDIL_LIN_VectorXL::LIN_GetCurrStatus(s_STATUSMSG& StatusData)
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
HRESULT CDIL_LIN_VectorXL::LIN_GetTxMsgBuffer(BYTE*& /*pouFlxTxMsgBuffer*/)
{
    return S_OK;
}
HRESULT CDIL_LIN_VectorXL::LIN_GetConfiguration(sCONTROLLERDETAILSLIN psControllerConfig[], INT& nSize)
{
    for ( int i = 0; i < sg_nNoOfChannels; i++ )
    {
        psControllerConfig[i].m_strHwUri =  sg_HardwareIntr[i].m_acDescription.c_str();
        psControllerConfig[i].m_nBaudRate =  sg_aodChannels[i].m_unBaudrate;
        psControllerConfig[i].m_strProtocolVersion = sg_aodChannels[i].m_strLinVersion;
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
* \authors       Arunkumar Karri
* \date          07.10.2011 Created
*/
HRESULT CDIL_LIN_VectorXL::LIN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
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
HRESULT CDIL_LIN_VectorXL::LIN_SetControllerParams(INT nValue, ECONTR_PARAM eContrparam)
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
HRESULT CDIL_LIN_VectorXL::LIN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
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
HRESULT CDIL_LIN_VectorXL::LIN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog)
{
    return S_OK;
}


