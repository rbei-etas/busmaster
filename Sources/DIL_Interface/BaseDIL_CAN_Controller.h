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
 * \file      BaseDIL_CAN_Controller.h
 * \brief     Base class for CDIL_CAN class.
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Base class for CAN controller classes.
 */

#if !defined BASEDIL_CAN_CONTROLLER_H__INCLUDED_
#define BASEDIL_CAN_CONTROLLER_H__INCLUDED_

class CBaseDIL_CAN_Controller
{
public:
    /**
     * Performs intial operations.
     * Initializes filter, queue, controller config with default values.
     *
     * @return S_OK for success, S_FALSE for failure
     */
    virtual HRESULT CAN_PerformInitOperations(void) = 0;

    /**
     * Performs closure operations.
     *
     * @return S_OK for success, S_FALSE for failure
     */
    virtual HRESULT CAN_PerformClosureOperations(void) = 0;

    /**
     * Gets the time mode mapping of the hardware. CurrSysTime
     * will be updated with the system time ref.
     * TimeStamp will be updated with the corresponding timestamp.
     *
     * @return S_OK for success, S_FALSE for failure
     */
    virtual HRESULT CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER* QueryTickCount = NULL) = 0;

    /**
     * Lists the hardware interface available. sSelHwInterface
     * will contain the user selected hw interface.
     *
     * @return S_OK for success, S_FALSE for failure
     */
    virtual HRESULT CAN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount) = 0;

    /**
     * Selects the hardware interface selected by the user.
     *
     * @return S_OK for success, S_FALSE for failure
     */
    virtual HRESULT CAN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount) = 0;

    /**
     * Deselects the selected hardware interface.
     *
     * @return S_OK for success, S_FALSE for failure
     */
    virtual HRESULT CAN_DeselectHwInterface(void) = 0;

    /**
     * Displays the controller configuration dialog.
     * Fields are initialized with values supplied by InitData.
     * InitData will be updated with the user selected values.
     *
     * @return S_OK for success, S_FALSE for failure
     */
    virtual HRESULT CAN_DisplayConfigDlg(PSCONTROLLER_DETAILS InitData, int& Length) = 0;

    /**
     * Sets the controller configuration data supplied by InitData.
     *
     * @return S_OK for success, S_FALSE for failure
     */
    virtual HRESULT CAN_SetConfigData(PSCONTROLLER_DETAILS InitData, int Length) = 0;

    /**
     * Starts the controller.
     *
     * @return S_OK for success, S_FALSE for failure
     */
    virtual HRESULT CAN_StartHardware(void) = 0;

    /**
     * Stops the controller.
     *
     * @return S_OK for success, S_FALSE for failure
     */
    virtual HRESULT CAN_StopHardware(void) = 0;

    /**
     * Function to get Controller status
     *
     * @return S_OK for success, S_FALSE for failure
     */
    virtual HRESULT CAN_GetCurrStatus(s_STATUSMSG& StatusData) = 0;

    /**
     * Sends STCAN_MSG structure from the client dwClientID.
     */
    virtual HRESULT CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg) = 0;

    /**
     * Gets last occured error and puts inside acErrorStr.
     */
    virtual HRESULT CAN_GetLastErrorString(string& acErrorStr) = 0;

    /**
     * Gets the controller param eContrParam of the channel.
     * Value stored in lParam.
     */
    virtual HRESULT CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam) = 0;

    /**
     * Gets the error counter for corresponding channel.
     */
    virtual HRESULT CAN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam) = 0;

    virtual HRESULT CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam) = 0;

    /**
     * Sets the application params.
     *
     * @return Returns S_OK for success, S_FALSE for failure
     */
    virtual HRESULT CAN_SetAppParams(HWND hWndOwner, Base_WrapperErrorLogger* pILog) = 0;

    /**
     * Registers the buffer pBufObj to the client ClientID
     *
     * @return Returns S_OK for success, S_FALSE for failure
     */
    virtual HRESULT CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj) = 0;

    /**
     * Registers a client to the DIL. ClientID will have client id
     * which will be used for further client related calls
     *
     * @return S_OK for success, S_FALSE for failure
     */
    virtual HRESULT CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName) = 0;

    /**
     * Returns the controller status. hEvent will be registered
     * and will be set whenever there is change in the controller
     * status.
     *
     * @return S_OK for success, S_FALSE for failure
     */
    virtual HRESULT CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus) = 0;

    /**
     * Loads BOA related libraries. Updates BOA API pointers
     *
     * @return S_OK for success, S_FALSE for failure
     */
    virtual HRESULT CAN_LoadDriverLibrary(void) = 0;

    /**
     * Unloads the driver library.
     *
     * @return Returns S_OK for success, S_FALSE for failure
     */
    virtual HRESULT CAN_UnloadDriverLibrary(void) = 0;
};

#endif // BASEDIL_CAN_CONTROLLER_H__INCLUDED_