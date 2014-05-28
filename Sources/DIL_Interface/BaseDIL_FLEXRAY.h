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
 * @brief Base class for CDIL_FLEXRAY class.
 * @author Arunkumar Karri
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Base class for CDIL_FLEXRAY class.
 */

#pragma once

#include "DataTypes/DIL_DataTypes.h"
#include "DataTypes/FLEXRAY_Datatypes.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "Datatypes/Base_WrapperErrorLogger.h"
#include "DataTypes/Cluster.h"

class CBaseDIL_FLEXRAY
{
public:
    /**
     * Constructor
     */
    CBaseDIL_FLEXRAY() {};

    /**
     * Destructor
     */
    ~CBaseDIL_FLEXRAY() {};

    /* Based on the parameter this function renders number of the driver interface
    layers supported or available. If 'bAvailable' is true, this returns number of
    the DILs implemented; else the list of the DILs supported by the existing
    license will be returned. If List is NULL, only number is returned. */
    virtual DWORD DILF_GetDILList(bool bAvailable, FLEXRAY_DILLIST* List) = 0;

    /* This function selects a driver abstraction layer (DAL). If support for the
    intended one isn't allowed with the present license, this returns NO_LICENSE.
    A list for DALs (or bDriverID) is shown below:
    DRIVER_STUB         : DAL for Stub
    DRIVER_ES520        : DAL for ES520
    DRIVER_TZMCYCLONE   : DAL for TZMCyclone
    DAL_NONE            : Dummy DAL */
    virtual HRESULT DILF_SelectDriver(DWORD dwDriverID, HWND hWndParent, Base_WrapperErrorLogger* pILog) = 0;

    /**
     * DILF_RegisterClient registers/unregisters the client.
     * bRegister = TRUE for registeration
     * bRegister = FALSE for unregisteration
     * Only registered client's buffer will be updated on receive of a msg in the bus.

     * Following are the return values
     * S_OK -->registeration successful
     * ERR_CLIENT_EXISTS -->Client already registered. Client Id will be stored in ClientId.
     * ERR_NO_CLIENT_EXIST --> No client exists. Usually this happens if we try to remove non existing client.
     * ERR_NO_MORE_CLIENT_ALLOWED -->No more clients are allowed to register.
     */
    virtual HRESULT DILF_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName) = 0;

    /**
     * This function manages the target message buffer list. The two combinations
     * are the following:
     * bAction = MSGBUF_ADD, add pBufObj to the target message buffer list.
     * bAction = MSGBUF_CLEAR, clear the list. pBufObj is ignored.
     * At present maximum number of entries in the list is kept as 8.
     */
    virtual HRESULT DILF_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseFLEXBufFSE* pBufObj) = 0;


    /* This returns ID of the driver interface layer presently selected. */
    virtual DWORD DILF_GetSelectedDriver(void) = 0;

    /* Call for all initialisation operations. */
    virtual HRESULT DILF_PerformInitOperations(void) = 0;

    /* Call for all uninitialisation operations */
    virtual HRESULT DILF_PerformClosureOperations(void) = 0;

    /* Call this function to get a system time and the time stamp associated with it*/
    virtual HRESULT DILF_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp) = 0;

    /* Call this function to list the hardware interfaces available and receive
    user's choice. */
    virtual HRESULT DILF_ListHwInterfaces(FLEXRAY_INTERFACE_HW& sSelHwInterface, FLEXRAY_CONFIG_FILES& sAvailableConfigFiles) = 0;

    /* Call this function to select a particular hardware interface. */
    virtual HRESULT DILF_SelectHwInterfaces(const FLEXRAY_INTERFACE_HW_LIST& sSelHwInterface) = 0;

    /* Call this function to deselect the selected hardware interface. */
    virtual HRESULT DILF_DeselectHwInterfaces(void) = 0;

    /* Function to display the configuration dialog box for the selected DIL. If
    the dialog box needs to be displayed been initialised, pass the relevant data
    as InitData. If it is null, the dialog box is uninitialised. This also contains
    the user's choice as OUT parameter */
    virtual HRESULT DILF_DisplayConfigDlg(PCHAR& InitData, int& Length) = 0;

    /* To set the configuration data for the currently selected DIL. Caller must
    release the memory. */
    //virtual HRESULT DILF_SetConfigData(PCHAR pInitData,  ABS_CLUSTER_INFO   asClusterinfo, int Length, ABS_DEVICE_CONFIG  asDeviceConfig) = 0;
    virtual HRESULT DILF_SetConfigData(CHANNEL_CONFIG& ouAbsSFibexContainer) = 0;

    /* Start the controller*/
    virtual HRESULT DILF_StartHardware(void) = 0;

    /* Stop the controller*/
    virtual HRESULT DILF_StopHardware(void) = 0;

    /* Reset Hardware*/
    virtual HRESULT DILF_ResetHardware(void) = 0;

    /* Get the current controller status*/
    virtual HRESULT DILF_GetCurrStatus(s_FLXSTATUSMSG& StatusData) = 0;

    /* Call to receive list of the transmittable messages*/
    virtual HRESULT DILF_GetFlxTxMsgBuffer(CFlxTxMsgBuffer*& pouFlxTxMsgBuffer) = 0;

    /* Send Flx messages*/
    virtual HRESULT DILF_SendFlxMsg(DWORD dwClientID, s_FLXTXMSG* pouFlxTxMsg) = 0;

    /* Send Flx messages*/
    virtual HRESULT DILF_DeleteFlxMsg(DWORD dwClientID, s_FLXTXMSG* pouFlxTxMsg) = 0;


    /* Get basic info of the board */
    virtual HRESULT DILF_GetBoardInfo(s_BOARDINFO& BoardInfo) = 0;

    /* Get salient informations on current bus configuration */
    virtual HRESULT DILF_GetFlexRayInfo(s_FLXINFO& FlexRayInfo) = 0;

    /* Call to receive the version informations */
    virtual HRESULT DILF_GetVersionInfo(VERSIONINFO& sVerInfo) = 0;

    /* Call to get descriptive string of the last error occurred */
    virtual HRESULT DILF_GetLastErrorString(std::string acErrorStr, HRESULT& nError ) = 0;

    /* Call to set PASS/STOP filter */
    virtual HRESULT DILF_FilterFrames(FILTER_TYPE FilterType, FLEX_CHANNEL FlexChannel, UINT* punFrames, UINT nLength) = 0;

    /* Call to Configure Message buffer */
    virtual HRESULT DILF_ConfigMsgBuf(s_MSGBUF sMsgBuf, UINT* punAddress) = 0;

    virtual HRESULT DILF_GetControllerCapabilities(s_FLXControllerCapabilities& ouFlexRayCapabilities) = 0;

};
