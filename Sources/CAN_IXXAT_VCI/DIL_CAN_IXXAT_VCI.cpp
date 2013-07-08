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
 * @file  DIL_CAN_IXXAT_VCI.cpp
 *
 * @brief
 *      Implements the access to IXXAT VCI (Windows driver) based interfaces.
 * @remarks
 *    The available CAN channels will be stored in the m_arrInternalChannel array.
 *    This is necessary because the VCI works with 64 bit values and the Busmaster
 *    works with 32 bit values for the device access.
 */


#include "CAN_IXXAT_VCI_stdafx.h"
#include "DIL_CAN_IXXAT_VCI.h"
#include "vcinpldynl.h"
#include "DIL_CommonDefs.h"

#include "resource.h"

#include "CANControllerConfigDlg.h"
#include "CANSelectDlg.h"
#include "HardwareListing.h"


//C++ Includes
#include <sstream>
using namespace std;
/*
   Internal note
   --------------------------------------------
   The sequence of called functions is:
           CAN_SetAppParams
           CAN_LoadDriverLibrary
     2 x * CAN_PerformInitOperations
           CAN_ListHWInterface
           CAN_SetConfigData
           ----
    Cyclic:CAN_GetControllerParams
           ----
           CAN_PerformClosureOperations
           CAN_UnloadDriverLibrary
*/


/**
 *
 * @brief
 *   Default constructor. Set the member variables to a default value.
 *
 */
CDIL_CAN_IXXAT_VCI::CDIL_CAN_IXXAT_VCI()
    : CBaseDIL_CAN_Controller()
{
    m_bDriverAccessLoaded = FALSE;
    m_iNumberOfCANChannelsTotal = 0;

    m_pILog = NULL;
    m_hOwnerWndHandle = NULL;
    m_byCurrHardwareState = STATE_HARDWARE_NONE;


#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_Base::CDIL_CAN_IXXAT_VCI\n"));
#endif

}

/**
 *
 * @brief
 *  Destructor. Cleanup variables.
 *
 */
CDIL_CAN_IXXAT_VCI::~CDIL_CAN_IXXAT_VCI()
{
#ifdef _IXXAT_DEBUG
    // do not log at this time because all objects were deleted at this time
    // LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::~CDIL_CAN_IXXAT_VCI\n"));
#endif

    m_pILog = NULL;
    m_ClientList.DeleteAllEntries();
}


/**
 * @brief
 *  Perform some initialization operations.
 *  At this time, nothing to do here.
 *
 * @return
 *  Always S_OK.
 *
 */
HRESULT CDIL_CAN_IXXAT_VCI::CAN_PerformInitOperations(void)
{
#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_PerformInitOperations\n"));
#endif

    //Initialize the selected channel items array to -1
    for ( UINT i = 0; i< CHANNEL_ALLOWED; i++ )
    {
        m_anSelectedItems[i] = -1;
    }

    return S_OK;
}

/**
 * @brief
 *  Do operations before closing this class object.
 *  Close and release all opened CAN controllers.
 *
 * @return
 *  Always S_OK.
 *
 */
HRESULT CDIL_CAN_IXXAT_VCI::CAN_PerformClosureOperations(void)
{
#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_PerformClosureOperations\n"));
#endif

    for (int i=0; i < CHANNEL_CAN_MAX; i++)
    {
        m_arrIxxatCanChannels[i].ReleaseController();
    }
    return S_OK;
}

/**
* \brief         This function will popup hardware selection dialog and gets the user selection of channels.
* \param[in]     psInterfaces, is INTERFACE_HW structue
* \param[out]    pnSelList, contains channels selected array
* \param[out]    nCount, contains selected channel count
* \return        returns 0 if success, else -1
* \authors       Arunkumar Karri
* \date          11.07.2012 Created
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
 * @brief
 *  Create a list will all IXXAT CAN controllers.
 *
 * @todo
 *  At this time only the settings for the first CAN will be set
 *  and no dialog will shown.
 * *
 *
 * @param [in,out]  sSelHwInterface
 *  The selected hardware interface.
 * @param [in,out]  nCount
 *  Number of.
 *
 * @return
 *      S_OK - list successful filled
 *      E_POINTER - no access to the VCI drivers
 *      NO_HW_INTERFACE - no CAN interface found
 */
HRESULT CDIL_CAN_IXXAT_VCI::CAN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount)
{
#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_ListHwInterfaces\n"));
#endif
    HRESULT hResult = E_POINTER;

    UINT unDefaultChannelCnt = nCount;
    // default: no IXXAT interface available
    nCount = 0;
    m_iNumberOfCANChannelsTotal = 0;

    if (m_bDriverAccessLoaded)
    {
        HANDLE        hEnum;   // enumerator handle
        VCIDEVICEINFO sInfo;   // device info

        // open the device list
        hResult = DYNCALL(vciEnumDeviceOpen)(&hEnum);
        if (VCI_OK == hResult)
        {
            while (DYNCALL(vciEnumDeviceNext)(hEnum, &sInfo) == VCI_OK)
            {
                // increment the counter and add the device info to our list
                m_iNumberOfCANChannelsTotal += VciDeviceInfoAddToArray(nCount, &sInfo, m_sSelHwInterface);
            }
        }
        int nHwCount = m_iNumberOfCANChannelsTotal;
        //TODO: currently only 1 interface selected
        if (m_iNumberOfCANChannelsTotal > 0)
        {
            INT64 qiVCIDeviceID = 0;
            int iCANControllerNumber = 0;
            CClientList* pClientList = NULL;

            if ( m_iNumberOfCANChannelsTotal == 1 ) /* Only single channel available */
            {
                sSelHwInterface[0] = m_sSelHwInterface[0];

                /* Rearrange hardware parameters */
                m_arrTmpIxxatCanChannels[0].GetHardwareParams(&qiVCIDeviceID,
                        &iCANControllerNumber, (void**)&pClientList);
                m_arrIxxatCanChannels[0].SetHardwareParams(qiVCIDeviceID, iCANControllerNumber,
                        pClientList);
            }
            else    /* Multiple channels available */
            {
                /* If the default channel count parameter is set, prevent displaying the hardware selection dialog */
                if ( unDefaultChannelCnt && nHwCount >= unDefaultChannelCnt )
                {
                    for (UINT i = 0; i < unDefaultChannelCnt; i++)
                    {
                        m_anSelectedItems[i] = i;
                    }
                    nHwCount  = unDefaultChannelCnt;
                }
                else if ( ListHardwareInterfaces(m_hOwnerWndHandle, DRIVER_CAN_IXXAT, m_sSelHwInterface, m_anSelectedItems, nHwCount) != 0 )
                {
                    /* return if user cancels hardware selection */
                    return HW_INTERFACE_NO_SEL;
                }

                for ( int nCount = 0; nCount < nHwCount; nCount++ )
                {
                    /* Order the hardware information according to user selection */
                    sSelHwInterface[nCount] = m_sSelHwInterface[m_anSelectedItems[nCount]];

                    /* Rearrange hardware parameters */
                    m_arrTmpIxxatCanChannels[m_anSelectedItems[nCount]].GetHardwareParams(&qiVCIDeviceID,
                            &iCANControllerNumber, (void**)&pClientList);
                    m_arrIxxatCanChannels[nCount].SetHardwareParams(qiVCIDeviceID, iCANControllerNumber,
                            pClientList);
                }
                m_iNumberOfCANChannelsTotal = nHwCount;
            }
            nCount = m_iNumberOfCANChannelsTotal;
        }

        if (m_iNumberOfCANChannelsTotal == 0)
        {
            hResult = NO_HW_INTERFACE;
            LogMessage(FALSE, "No valid IXXAT CAN interface found.");
        }

        m_byCurrHardwareState = STATE_HW_INTERFACE_LISTED;
    }
    else
    {
        // the return value is already set to E_POINTER
        LogMessage(FALSE, "IXXAT VCI access not loaded or valid.");
    }

    return hResult;
}

/**
 * @brief
 *  Let the user select hardware interface.
 *  Shows an dialog to select all found IXXAT CAN controllers and let in
 *  this dialog the user select which one to use.
 *  At this time no dialog will shown and always the first can controller
 *  will be used. This is because this selection dialog will be moved to
 *  the busmaster application itself.
 * @param sSelHwInterface
 *   Array with the current available CAN controllers.
 * @param nCount [in]
 *  Number of valid entries in the controller array.
 * @todo
 *   At this time the first found CAN Controller will be used and no
 *   dialog will shown!
 *
 * @return
 *   Always S_OK.
 *
 */
HRESULT CDIL_CAN_IXXAT_VCI::CAN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount)
{
#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_SelectHwInterface\n"));
#endif

    // absolutely necessary to call this macro
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // show now the dialog and let the user select the interfaces he wants to use
    // CCANSelectDlg CANSelectDlg(CWnd::FromHandle(m_hOwnerWndHandle));
    // CANSelectDlg.DoModal();

    if (m_iNumberOfCANChannelsTotal > 0)
    {
        for (int i = 0 ; i < nCount ; i++)
        {
            m_arrIxxatCanChannels[i].Selected(TRUE);
        }
#ifdef _IXXAT_DEBUG
        LogMessage( TRUE, _T("------> Selected CAN controller: %s - %s (%s)\n")
                    , sSelHwInterface[0].m_acDeviceName.c_str()
                    , sSelHwInterface[0].m_acNameInterface.c_str()
                    , sSelHwInterface[0].m_acDescription.c_str() );
#endif
    }
    m_byCurrHardwareState = STATE_HW_INTERFACE_LISTED;

    return S_OK;
}

/**
 * @brief
 *  Release the selected interfaces.
 *  At this time nothing is done here because we always use
 *  the first found CAN controller.
 *
 * @return
 *  Always S_OK.
 *
 * @remarks  TODO: Don't forget to fill in the remarks!
 */
HRESULT CDIL_CAN_IXXAT_VCI::CAN_DeselectHwInterface(void)
{
#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_DeselectHwInterface\n"));
#endif
    return S_OK;
}

/**
 * @brief
 *  Display the CAN controller configuration dialog.
 *
 * @param [in,out]  InitData
 *  Information describing the CAN interfaces,
 *  to be filled with additional information by the dialog.
 * @param [in,out]  Length
 *  The length of the InitData structure.
 * @todo
 *  At this time only the settings for the first CAN will be set
 *  and no dialog will shown.
 *
 * @return
 *  E_POINTER - no IXXAT CAN hardware available.
 *  E_ABORT - aborted by user
 *  S_OK - baudrate set correct to first CAN controller.
 *
 */
HRESULT CDIL_CAN_IXXAT_VCI::CAN_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length)
{
    //TODO: original line, but here only the first available CAN controller is used
    // VALIDATE_VALUE_RETURN_VAL(m_byCurrHardwareState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);

    VALIDATE_VALUE_RETURN_VAL(m_byCurrHardwareState, STATE_HW_INTERFACE_LISTED, ERR_IMPROPER_STATE);
    VALIDATE_POINTER_RETURN_VAL(InitData, S_FALSE);

    // absolutely necessary to call this macro
    // else you will run directly into MFC hell
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_DisplayConfigDlg\n"));
#endif

    HRESULT hrRes = E_POINTER;

    if (m_iNumberOfCANChannelsTotal > 0)
    {
        // The InitData is an pointer to an array with the controller details
        // int iNumOfCtrlEntries = Length / sizeof(SCONTROLER_DETAILS); -> no need this time
        SCONTROLLER_DETAILS* pCtrlArray = (SCONTROLLER_DETAILS*)InitData;

        INT_PTR iDialogResult = -1;
        CCANControllerConfigDlg CANControllerConfigDlg(pCtrlArray[0].m_nBTR0BTR1, CWnd::FromHandle(m_hOwnerWndHandle));
        iDialogResult = CANControllerConfigDlg.DoModal();
        if (IDOK == iDialogResult)
        {
            pCtrlArray[0].m_omStrBaudrate = CANControllerConfigDlg.GetBitTimingName();
            pCtrlArray[0].m_nBTR0BTR1 = CANControllerConfigDlg.GetBitTimingValue();
            m_arrIxxatCanChannels[0].SetControllerParameter(&pCtrlArray[0]);

            hrRes = S_OK;
        }
        else
        {
            // aborted because the user had pressed the abort button
            hrRes = E_ABORT;
        }
    }

    return hrRes;
}

/**
 * @brief Can set configuration data.
 *
 * @param pInitData [in, out]
 *  Information describing the init data for each controller. This
 *  is a pointer to an array with the struct SCONTROLLER_DETAILS.
 * @param Length [in]
 *  The length in bytes of the valid part of the given data structure.
 *
 * @return
 *  Always S_OK.
 */
HRESULT CDIL_CAN_IXXAT_VCI::CAN_SetConfigData(PSCONTROLLER_DETAILS pInitData, int Length)
{
#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_SetConfigData\n"));
#endif

    // VALIDATE_VALUE_RETURN_VAL(sg_bCurrState, STATE_HW_INTERFACE_SELECTED, ERR_IMPROPER_STATE);
    // USES_CONVERSION;

    // int iNumOfCtrlEntries = Length / sizeof(SCONTROLLER_DETAILS); -> no need this time
    SCONTROLLER_DETAILS* pCtrlArray = (SCONTROLLER_DETAILS*)pInitData;

    int iMaxNumHardware = m_iNumberOfCANChannelsTotal;
    if (m_iNumberOfCANChannelsTotal > defNO_OF_CHANNELS)
    {
        iMaxNumHardware = defNO_OF_CHANNELS;
    }
    // Fill the hardware description details

    for (int iCount = 0; iCount < iMaxNumHardware; iCount++)
    {
        // this can be done in one line, but better readable when we use two lines
        SCONTROLLER_DETAILS* pCtrlDetails = (SCONTROLLER_DETAILS*) &pCtrlArray[iCount];
        pCtrlDetails->m_omHardwareDesc = "VCI CAN Interface";

        // now set the controller parameter
        m_arrIxxatCanChannels[iCount].SetControllerParameter(&pCtrlArray[iCount]);
    }
    return S_OK;
}

/**
 * @brief
 *  Open and initialize the first CAN controller.
 *  If success start then the CAN access.
 *
 * @return
 *  E_POINTER - no IXXAT CAN hardware available.
 *  ERR_LOAD_HW_INTERFACE - Failed to open the hardware in the driver.
 *  S_OK - Success.
 *
 */
HRESULT CDIL_CAN_IXXAT_VCI::CAN_StartHardware(void)
{
#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_StartHardware\n"));
#endif

    HRESULT hResult = E_POINTER;

    if (m_iNumberOfCANChannelsTotal > 0)
    {
        for (int i = 0 ; i < m_iNumberOfCANChannelsTotal ; i++)
        {
            hResult = m_arrIxxatCanChannels[i].AccessController(i+1);
            if ( (VCI_OK == hResult) || (HW_INTERFACE_ALREADY_SELECTED == hResult) )
            {
                hResult = m_arrIxxatCanChannels[i].InitController();
            }
            if ( (VCI_OK == hResult) || (INFO_INITDAT_CONFIRM_CONFIG == hResult) )
            {
                hResult = m_arrIxxatCanChannels[i].StartController();
            }
        }
        /* Get connection time */
        GetLocalTime(&m_sCurrSysTime);
    }

    return hResult;
}

/**
 * @brief
 *  Stop and release the access to an CAN controller.
 *
 * @return
 *  E_FAIL - no IXXAT CAN hardware available.
 *  HW_INTERFACE_NO_SEL - No open CAN controller.
 *  ERROR_OTHER - Error message from VCI.
 *  S_OK - Success.
 *
 */
HRESULT CDIL_CAN_IXXAT_VCI::CAN_StopHardware(void)
{
#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_StopHardware\n"));
#endif

    HRESULT hResult = E_POINTER;
    if (m_iNumberOfCANChannelsTotal > 0)
    {
        for (int i = 0 ; i < m_iNumberOfCANChannelsTotal ; i++)
        {
            hResult = m_arrIxxatCanChannels[i].StopController();
            if (VCI_OK == hResult)
            {
                hResult = m_arrIxxatCanChannels[i].ReleaseController();
            }
        }
    }

    return hResult;
}

/**
 * @brief
 *  Fill the given structure with the current state
 *  of the CAN controller.
 *
 * @param [out]  StatusData
 *  Information describing the status.
 *
 * @return
 *  E_POINTER - No IXXAT CAN hardware available.
 *  HW_INTERFACE_NO_SEL - No open CAN controller.
 *  else an VCI result (see vcierr.h").
 */
HRESULT CDIL_CAN_IXXAT_VCI::CAN_GetCurrStatus(s_STATUSMSG& StatusData)
{
#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_GetCurrStatus\n"));
#endif

    HRESULT hResult = E_POINTER;
    if (m_iNumberOfCANChannelsTotal > 0)
    {
        hResult = m_arrIxxatCanChannels[0].GetStatus(&StatusData);
    }
    return hResult;
}

/**
 * @brief
 *  Should fill the transmit queue to the parameter.
 *  Empty function.
 *
 * @param [in,out]  pouFlxTxMsgBuffer
 *  If non-null, buffer for transmit message buffer.
 *
 * @return
 *   Always E_NOTIMPL.
 *
 */
//HRESULT CDIL_CAN_IXXAT_VCI::CAN_GetTxMsgBuffer(BYTE*& pouFlxTxMsgBuffer)
//{
//#ifdef _IXXAT_DEBUG
//  LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_GetTxMsgBuffer\n"));
//#endif
//  return  E_NOTIMPL;
//}

/**
 * @brief
 *  Transmit a single CAN message.
 *
 * @param dwClientID
 *  Identifier for the client.
 * @param sCanTxMsg
 *  Message describing the can transmit.
 *
 * @return
 *  E_POINTER - No IXXAT CAN hardware available.
 *  HW_INTERFACE_NO_SEL - No open CAN controller.
 *  ERR_NO_CLIENT_EXIST - No client registered with this ID.
 *  else an VCI result (see vcierr.h").
 *
 */
HRESULT CDIL_CAN_IXXAT_VCI::CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg)
{
#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_SendMsg\n"));
#endif

    HRESULT hResult = E_POINTER;
    if ( sCanTxMsg.m_ucChannel > 0 && sCanTxMsg.m_ucChannel <= m_iNumberOfCANChannelsTotal)
    {
        hResult = m_arrIxxatCanChannels[sCanTxMsg.m_ucChannel-1].SendMessage(dwClientID, sCanTxMsg);
    }
    return hResult;
}

/**
 * @brief
 *  Fill the given struct with the device information.
 *
 * @todo
 *  no sample what to do here, so return not implemented.
 *
 * @param [in,out]  BoardInfo
 *  Structure to fill with information describing the board.
 *
 * @return
 *  Always E_NOTIMPL;
 *
 */
//HRESULT CDIL_CAN_IXXAT_VCI::CAN_GetBoardInfo(s_BOARDINFO& BoardInfo)
//{
//#ifdef _IXXAT_DEBUG
//  LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_GetBoardInfo\n"));
//#endif
//  return  E_NOTIMPL;
//}

/**
 * @brief
 *  Fill the given struct with the CAN configuration information.
 *  TODO: no sample what to do here, so return not implemented.
 *
 * @param [in,out]  BusInfo
 *  Structure to fill with information describing the CAN configuration.
 *
 * @return
 *  Always E_NOTIMPL;
 *
 */
//HRESULT CDIL_CAN_IXXAT_VCI::CAN_GetBusConfigInfo(BYTE* BusInfo)
//{
//#ifdef _IXXAT_DEBUG
//  LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_GetBusConfigInfo\n"));
//#endif
//  return  E_NOTIMPL;
//}

/**
 * @brief
 *  Get version information about the CAN.
 *  TODO: no sample what to do here, so return not implemented.
 *
 * @param [in,out]  sVerInfo
 *  Structure to be filled here with CAN information.
 *
 * @return
 *  Always E_NOTIMPL
 *
 */
//HRESULT CDIL_CAN_IXXAT_VCI::CAN_GetVersionInfo(VERSIONINFO& sVerInfo)
//{
//#ifdef _IXXAT_DEBUG
//  LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_GetVersionInfo\n"));
//#endif
//  return  E_NOTIMPL;
//}

/**
 * @brief
 *  Fill the given string with the last error code.
 *  Can get last error string.
 *  TODO: no sample what to do here, so return not implemented.
 *
 * @param [in,out]  acErrorStr
 *  If non-null, the string where the data should be copied to.
 * @param nLength
 *  The max. length of the string.
 *
 * @return
 *  Always E_NOTIMPL
 *
 */
//HRESULT CDIL_CAN_IXXAT_VCI::CAN_GetLastErrorString(CHAR* acErrorStr, int nLength)
//{
//#ifdef _IXXAT_DEBUG
//  LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_GetLastErrorString\n"));
//#endif
//  return  E_NOTIMPL;
//}

HRESULT CDIL_CAN_IXXAT_VCI::CAN_GetLastErrorString(string& acErrorStr)
{
#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_GetLastErrorString\n"));
#endif
    return  E_NOTIMPL;
}

/**
 * @brief
 *  Get CAN controller information.
 *  TODO: only copied from another CAN DLL. Check if this is right here.
 *
 * @param [out]  lParam
 *  The wished information parameter.
 * @param nChannel
 *  The channel.
 * @param eContrParam
 *  The type parameter which should written to lParam.
 *
 * @return
 *  S_FALSE - No valid control value.
 *  S_OK - lParam successful assigned.
 */
HRESULT CDIL_CAN_IXXAT_VCI::CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam)
{
#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_GetControllerParams\n"));
#endif
    HRESULT hResult = S_OK;
    switch (eContrParam)
    {

        case NUMBER_HW:
        {
            lParam = m_iNumberOfCANChannelsTotal;
        }
        break;
        case NUMBER_CONNECTED_HW:
        {
            lParam = CHANNEL_ALLOWED;
        }
        break;
        case DRIVER_STATUS:
        {
            lParam = (m_bDriverAccessLoaded) ? 0 : 1;
        }
        break;
        case HW_MODE:
        {
            lParam = 0;
        }
        break;
        case CON_TEST:
        {
            lParam = (LONG)-1;
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

HRESULT CDIL_CAN_IXXAT_VCI::CAN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam)
{
    return S_OK;
}

/**
 * @brief Can get error count.
 *
 * @param [in,out]  sErrorCnt
 *  Number of errors.
 *  IXXAT support only counting of received error frames. No transmit
 *  error or oeak errors.
 * @param nChannel
 *  The CAN channel number.
 * @param eContrParam
 *  The parameter to indicate which type of error the calling function want.
 *
 * @return
 *  HW_INTERFACE_NO_SEL - No open CAN controller.
 *  S_OK - Success.
 *
 */
HRESULT CDIL_CAN_IXXAT_VCI::CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam)
{
#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_GetErrorCount\n"));
#endif
    HRESULT hrRes = HW_INTERFACE_NO_SEL;
    if (nChannel < CHANNEL_CAN_MAX)
    {
        if (m_arrIxxatCanChannels[nChannel].IsSelected())
        {
            if (eContrParam == ERR_CNT)
            {
                sErrorCnt.m_ucTxErrCount = 0;
                // TODO: only up to 255 errors?
                sErrorCnt.m_ucRxErrCount = (UCHAR) m_arrIxxatCanChannels[nChannel].GetRxErrorFrameCounter();
            }
            else if (eContrParam == PEAK_ERR_CNT)
            {
                sErrorCnt.m_ucTxErrCount = 0;
                sErrorCnt.m_ucRxErrCount = 0;
            }
            hrRes = S_OK;
        }
    }
    return hrRes;
}


/**
 * @brief
 *  Set the application parameters.
 *
 * @param hWndOwner
 *  Is the main frame HWND value.
 * @param [in,out]  pILog
 *  Is pointer to error logger object.
 *
 * @return
 *  Always S_OK.
 */
HRESULT CDIL_CAN_IXXAT_VCI::CAN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog)
{
    m_pILog = pILog;
    m_hOwnerWndHandle = hWndOwner;

#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_SetAppParams\n"));
#endif
    /* Initialise both the time parameters */
    GetLocalTime(&m_sCurrSysTime);
    m_qwTimeStamp = 0;

    /* Query Tick Count */
    m_qwQueryTickCount = 0;

    return S_OK;
}


/**
 * @brief
 *  Register a client.
 *
 * @param bRegister
 *  TRUE to register, FALSE to unregister.
 * @param [in, out]  ClientID
 *  When register this var will be set to the unique client identifier,
 *  when unregister this value will be used to find and unregister the client.
 * @param [in,out]  pacClientName
 *  Only needed when bRegister is TRUE to add a new client to the list.
 *
 * @return
  * ERR_CLIENT_EXISTS - a client with this name already exits.
 *  ERR_NO_MORE_CLIENT_ALLOWED - the max. number of clients is reached.
 *  ERR_NO_CLIENT_EXIST - A client with this ID did not exists (while unregistering).
 *  S_OK - client registered or unregistered successful.
 */
HRESULT CDIL_CAN_IXXAT_VCI::CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, TCHAR* pacClientName)
{
#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_RegisterClient - %s\n"), pacClientName);
#endif

    HRESULT hResult;
    if (bRegister)
    {
        hResult = m_ClientList.RegisterClient(ClientID, pacClientName);
    }
    else
    {
        hResult = m_ClientList.RemoveClient(ClientID);
    }
    return hResult;
}



/**
 * @brief Can get time mode mapping.
 *   Gets the time mode mapping of the hardware. CurrSysTime
 *   will be updated with the system time ref.
 *   TimeStamp will be updated with the corresponding timestamp.
 * @param [out]  CurrSysTime
 *      CurrSysTime, is SYSTEMTIME structure
 * @param [out]  TimeStamp
 *    TimeStamp, is UINT64
 * @param [out]  QueryTickCount
 *    QueryTickCount, is LARGE_INTEGER
 *
 * @return
 *      Always S_OK.
 */
HRESULT CDIL_CAN_IXXAT_VCI::CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount)
{
#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_GetTimeModeMapping\n"));
#endif

    memcpy(&CurrSysTime, &m_sCurrSysTime, sizeof(SYSTEMTIME));
    TimeStamp = m_qwTimeStamp;
    //TimeStamp = 0;
    if(QueryTickCount != NULL)
    {
        QueryTickCount->QuadPart = m_qwQueryTickCount;
    }

    return S_OK;
}

/**
 * @brief
 *  Registers the buffer pBufObj to the client ClientID.
 *
 * @param byAction
 *  contains one of the values MSGBUF_ADD or MSGBUF_CLEAR.
 * @param ClientID
 *  ClientID, is the client ID.
 * @param [in,out]  pBufObj
 *  pBufObj, is pointer to CBaseCANBufFSE object.
 *
 * @return
 *  S_FALSE for failure.
 *  S_OK for success,
 *
 * @remarks  TODO: Don't forget to fill in the remarks!
 */
HRESULT CDIL_CAN_IXXAT_VCI::CAN_ManageMsgBuf(BYTE byAction, DWORD ClientID, CBaseCANBufFSE* pBufObj)
{
#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_ManageMsgBuf  - Action: %u  ClientID: %u\n"), byAction, ClientID);
#endif
    HRESULT hResult = S_FALSE;
    if (pBufObj)
    {
        CClientBuffer* pClientBuf = m_ClientList.GetClientByID(ClientID);
        if (pClientBuf)
        {
            if (byAction == MSGBUF_ADD)
            {
                /* Add msg buffer */
                hResult = pClientBuf->AddMsgBuf(pBufObj);
            }
            else if (byAction == MSGBUF_CLEAR)
            {
                /* clear msg buffer */
                if (pBufObj != NULL)
                {
                    hResult = pClientBuf->RemoveMsgBuf(pBufObj);
                }
                else
                {
                    // remove all message buffers
                    hResult = pClientBuf->RemoveAllMsgBuf();
                }
            }
        }
        else
        {
            hResult = ERR_NO_CLIENT_EXIST;
        }
    }
    else
    {
        if (byAction == MSGBUF_CLEAR)
        {
            /* clear msg buffer */
            hResult = m_ClientList.RemoveAllMsgBufOnAllClients();
        }
    }
    return hResult;
}


/**
 * @brief
 *  Write the current CAN controller status to the given parameter.
 *  TODO: no sample what to do here.
 *
 * @param hEvent
 *  The event.
 * @param [out]  unCntrlStatus
 *  The variable to write the controller status into.
 *
 * @return
 *  Always E_NOTIMPL.
 *
 */
HRESULT CDIL_CAN_IXXAT_VCI::CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus)
{
#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_GetCntrlStatus\n"));
#endif
    return E_NOTIMPL;
}

/**
 * @brief
 *  Load the DLL for the hardware access. In this function
 *  the VCI V3 functions will by loaded dynamically.
 * @remarks
 *  Don't forget the unload the DLL with the function
 *  CAN_UnloadDriverLibrary().
 *
 * @return
 *  An HRESULT value (S_OK - success, else an error occurred).
 *
 */
HRESULT CDIL_CAN_IXXAT_VCI::CAN_LoadDriverLibrary(void)
{
#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_LoadDriverLibrary\n"));
#endif

    // Load VCI V3 Library
    HRESULT  hResult = LoadVciNplLib();

    // Map general VCI functions
    if(hResult == S_OK)
    {
        hResult = MapGeneralVciFunctions();
    }

    // Map device manager functions
    if(hResult == S_OK)
    {
        hResult = MapDeviceManagerFunctions();
    }

    // Map device functions
    if(hResult == S_OK)
    {
        hResult = MapDeviceFunctions();
    }

    // Map CAN controller functions
    if(hResult == S_OK)
    {
        hResult = MapCANControllerFunctions();
    }

    // Map CAN channel functions
    if(hResult == S_OK)
    {
        hResult = MapCANMessageChannelFunctions();
    }

    if (VCI_OK == hResult)
    {
        m_bDriverAccessLoaded = TRUE;
        m_byCurrHardwareState = STATE_DRIVER_LOADED;
    }

    return hResult;
}

/**
 * @brief
 *   Release the DLL and the functions for
 *   the IXXAT driver access.
 *
 * @return
 *   Always S_OK.
 */
HRESULT CDIL_CAN_IXXAT_VCI::CAN_UnloadDriverLibrary(void)
{
#ifdef _IXXAT_DEBUG
    LogMessage(TRUE, _T("------> CDIL_CAN_IXXAT_VCI::CAN_UnloadDriverLibrary\n"));
#endif

    m_bDriverAccessLoaded = FALSE;
    m_byCurrHardwareState = STATE_HARDWARE_NONE;

    // Unmaps all VCI functions
    UnmapGeneralVciFunctions();
    UnmapDeviceManagerFunctions();
    UnmapDeviceFunctions();
    UnmapCANControllerFunctions();
    UnmapCANMessageChannelFunctions();

    // the function FreeVciNplLib() returns always S_OK
    FreeVciNplLib();

    return S_OK;
}

/**
 * @brief
 *      Add the CAN controller information of all IXXAT VCI
 *      devices to the array with the hardware information.
 *
 * @param [in] iStartPosArray
 *  The start position array where to write the data into.
 * @param [in]  pVciDevInfo
 *  If non-null, information describing the current VCI device
 *  found by the device enumerator.
 * @param [out]  sSelHwInterface
 *  The array with the information of all hardware interfaces.
 *
 * @return
 *      The number of found CAN controllers. The calling function
 *      must use this value to increment the current position in
 *      the hardware array.
 */
int CDIL_CAN_IXXAT_VCI::VciDeviceInfoAddToArray(int iStartPosArray, VCIDEVICEINFO* pVciDevInfo, INTERFACE_HW_LIST& sSelHwInterface)
{
    int iNumOfCANController = 0;
    HANDLE hDevice = NULL;

    VCIDEVICECAPS sVciDeviceCaps;

    // try to open the VCI device
    if ( VCI_OK == DYNCALL(vciDeviceOpen)(pVciDevInfo->VciObjectId, &hDevice) )
    {
        // try to get the capabilities of the current open device to check
        // the number and type of the CAN controller to add it to the
        // program global device array.
        if ( VCI_OK == DYNCALL(vciDeviceGetCaps)(hDevice, &sVciDeviceCaps) )
        {
            for (int i=0; i < sVciDeviceCaps.BusCtrlCount; i++)
            {
                if (VCI_BUS_CAN == VCI_BUS_TYPE(sVciDeviceCaps.BusCtrlTypes[i]))
                {
                    iNumOfCANController++;

                    // store the current information in our class internal structure
                    m_arrTmpIxxatCanChannels[iStartPosArray].SetHardwareParams(pVciDevInfo->VciObjectId.AsInt64, i, &m_ClientList);

                    sSelHwInterface[iStartPosArray].m_dwIdInterface = iStartPosArray;
                    sSelHwInterface[iStartPosArray].m_bytNetworkID = (BYTE) i;    ///< Controller number inside this device.
                    sSelHwInterface[iStartPosArray].m_dwVendor = 0; // always 0

                    //strcpy_s(sSelHwInterface[iStartPosArray].m_acDeviceName, MAX_CHAR_SHORT, pVciDevInfo->Description); // the name of the device
                    sSelHwInterface[iStartPosArray].m_acDeviceName = pVciDevInfo->Description; // the name of the device


                    ostringstream oss;
                    oss<<"CAN "<<i;
                    //sprintf_s(sSelHwInterface[iStartPosArray].m_acNameInterface, MAX_CHAR_SHORT, _T("CAN %u"), i);
                    sSelHwInterface[iStartPosArray].m_acNameInterface = oss.str();

                    // if the cantype.h from IXXAT was enhanced then add the new hardware descriptions here
                    switch (VCI_CTL_TYPE(sVciDeviceCaps.BusCtrlTypes[i]))
                    {
                        case CAN_CTRL_82527:
                            //strcpy_s( sSelHwInterface[iStartPosArray].m_acDescription, MAX_CHAR_LONG, _T("Intel 82527") );
                            sSelHwInterface[iStartPosArray].m_acDescription = "Intel 82527";
                            break;
                        case CAN_CTRL_82C200:
                            //strcpy_s( sSelHwInterface[iStartPosArray].m_acDescription, MAX_CHAR_LONG, _T("Intel 82C200") );
                            sSelHwInterface[iStartPosArray].m_acDescription = "Intel 82C200";
                            break;
                        case CAN_CTRL_82C90:
                            //strcpy_s( sSelHwInterface[iStartPosArray].m_acDescription, MAX_CHAR_LONG, _T("Intel 82C90") );
                            sSelHwInterface[iStartPosArray].m_acDescription = "Intel 82C90";
                            break;
                        case CAN_CTRL_82C92:
                            //strcpy_s( sSelHwInterface[iStartPosArray].m_acDescription, MAX_CHAR_LONG, _T("Intel 82C92") );
                            sSelHwInterface[iStartPosArray].m_acDescription = "Intel 82C92";
                            break;
                        case CAN_CTRL_SJA1000:
                            //strcpy_s( sSelHwInterface[iStartPosArray].m_acDescription, MAX_CHAR_LONG, _T("Philips SJA 1000") );
                            sSelHwInterface[iStartPosArray].m_acDescription = "Philips SJA 1000";
                            break;
                        case CAN_CTRL_82C900:
                            //strcpy_s( sSelHwInterface[iStartPosArray].m_acDescription, MAX_CHAR_LONG, _T("Infinion 82C900 (TwinCAN)") );
                            sSelHwInterface[iStartPosArray].m_acDescription = "Infinion 82C900 (TwinCAN)";
                            break;
                        case CAN_CTRL_TOUCAN:
                            //strcpy_s( sSelHwInterface[iStartPosArray].m_acDescription, MAX_CHAR_LONG, _T("Motorola TOUCAN") );
                            sSelHwInterface[iStartPosArray].m_acDescription = "Motorola TOUCAN";
                            break;
                        case CAN_CTRL_IFI:
                            //strcpy_s( sSelHwInterface[iStartPosArray].m_acDescription, MAX_CHAR_LONG, _T("IFI-CAN") );
                            sSelHwInterface[iStartPosArray].m_acDescription = "IFI-CAN";
                            break;
                        default:
                            //strcpy_s( sSelHwInterface[iStartPosArray].m_acDescription, MAX_CHAR_LONG, _T("unknown CAN controller") );
                            sSelHwInterface[iStartPosArray].m_acDescription = "unknown CAN controller";
                            break;
                    }
                    // jump to the next array entry
                    iStartPosArray++;
                    if (iStartPosArray >= CHANNEL_CAN_MAX)
                    {
                        LogMessage(FALSE, _T("Too many IXXAT CAN controllers found. Abort filling hardware list!"));
                        break;
                    }
                }
            }
        }
        // close the formerly opened device for later use
        DYNCALL(vciDeviceClose)(hDevice);
        hDevice = NULL;
    }

    // return the number of found CAN controllers
    return iNumOfCANController;
}


/**
 * @brief
 *  Write a message to the debug output view and/or to the program log window.
 *
 * @param [in]  bShowOnlyInDebug
 *  true to show the message only in debug mode else show it only in release mode without using
 *  OutputDebugString.
 * @param [in]  pFormat
 *  Describes the format to use.
 *
 */
void CDIL_CAN_IXXAT_VCI::LogMessage(BOOL bShowOnlyInDebug, LPCTSTR pFormat, ...)
{
    BOOL bIsInDebugMode = FALSE;
#ifdef _DEBUG
    bIsInDebugMode = TRUE;
#endif

    // resolve the arguments and use a special printf
    va_list args;
    va_start(args, pFormat);
    TCHAR buffer [255];
    _vsntprintf_s(buffer, 255, pFormat, args);


    // when the flag is set to show it only in debug mode
    if ( (bShowOnlyInDebug) && (bIsInDebugMode) )
    {
        OutputDebugString(buffer);
        if (NULL != m_pILog)
        {
            m_pILog->vLogAMessage(A2T(__FILE__), __LINE__, buffer);
        }
    }

    // this message will shown only when we are in release mode
    // and the message should shown only in release mode
    // then no OutputDebugString will called
    if ( (!bShowOnlyInDebug) && (!bIsInDebugMode) )
    {
        if (NULL != m_pILog)
        {
            m_pILog->vLogAMessage(A2T(__FILE__), __LINE__, buffer);
        }
    }

    va_end(args);
}
