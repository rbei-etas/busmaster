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
 * \file      DIL_Interface/DIL_FLEXRAY.h
 * \brief     Definition file for CDIL_FLEXRAY class.
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for CDIL_FLEXRAY class.
 */

#pragma once

#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "Include/Basedefs.h"
#include "BaseDIL_FLEXRAY.h"
#include "BaseDIL_FLEXRAY_Controller.h"

class CDIL_FLEXRAY : public CBaseDIL_FLEXRAY
{
public:
    /** Constructor */
    CDIL_FLEXRAY();

    /* Destructor */
    ~CDIL_FLEXRAY();

    BOOL InitInstance(void);
    int ExitInstance(void);

    /* Variable to maintain currently selected Driver ID */
    DWORD m_dwDriverID;
    /* Variable to store previously selected Driver ID */
    DWORD m_dwOldDriverID;

    /* member variable to hold the pointer of currently selected controller interface */
    CBaseDIL_FLEXRAY_Controller* m_pBaseDILFLEXRAY_Controller;
    /* member variable to hold the previously selected controller interface */
    CBaseDIL_FLEXRAY_Controller* m_pOldBaseDILFLEXRAY_Controller;

    /* Variable to hold handle to currently selected controller DIL */
    HMODULE m_hDriverDll;
    /* Variable to hold handle to previously selected controller DIL */
    HMODULE m_hOldDriverDll;

    /* Based on the parameter this function renders number of the driver interface
    layers supported or available. If 'bAvailable' is true, this returns number of
    the DILs implemented; else the list of the DILs supported by the existing
    license will be returned. If List is nullptr, only number is returned. */
    DWORD DILF_GetDILList(bool bAvailable, FLEXRAY_DILLIST* List) ;

    /* Check and load support for latest available ETAS BOA version. */
    HMODULE vLoadEtasBoaLibrary(Base_WrapperErrorLogger* pILog);

    /* This function selects a driver abstraction layer (DAL).
    A list for DILs (or bDriverID) is shown below:
    FLEXRAY_DRIVER_STUB         : DIL for Stub
    FLEXRAY_DRIVER_ES520        : DIL for ES520
    FLEXRAY_DRIVER_TZMCYCLONE   : DIL for TZMCyclone
    FLEXRAY_DIL_NONE            : Dummy DIL */
    HRESULT DILF_SelectDriver(DWORD dwDriverID, HWND hWndParent, Base_WrapperErrorLogger* pILog) ;

    /**
     * This function registers the client. Only registered client's buffer will be
     * updated on receive of a msg in the bus.
     * Following are the return values
     * -1 registeration failed
     *  1 registeration successful
     *  2 Client already registered
     * -2 No more clients are allowed to register*/
    HRESULT DILF_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);

    /**
     * This function manages the target message buffer list. The two combinations
     * are the following:
     * bAction = MSGBUF_ADD, add pBufObj to the target message buffer list.
     * bAction = MSGBUF_CLEAR, clear the list. pBufObj is ignored.
     * At present maximum number of entries in the list is kept as 8.
     */
    HRESULT DILF_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseFLEXBufFSE* pBufObj);


    /* This returns ID of the driver interface layer presently selected. */
    DWORD DILF_GetSelectedDriver(void) ;

    /* Call for all initialisation operations. */
    HRESULT DILF_PerformInitOperations(void) ;

    /* Call for all uninitialisation operations */
    HRESULT DILF_PerformClosureOperations(void) ;

    /* Call this function to get a system time and the time stamp associated with it*/
    HRESULT DILF_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp) ;

    /* Call this function to list the hardware interfaces available and receive
    user's choice. */
    HRESULT DILF_ListHwInterfaces(FLEXRAY_INTERFACE_HW& sSelHwInterface, FLEXRAY_CONFIG_FILES& sAvailableConfigFiles) ;

    /* Call this function to select a particular hardware interface. */
    HRESULT DILF_SelectHwInterfaces(const FLEXRAY_INTERFACE_HW_LIST& sSelHwInterface) ;

    /* Call this function to deselect the selected hardware interface. */
    HRESULT DILF_DeselectHwInterfaces(void) ;

    /* Function to display the configuration dialog box for the selected DIL. If
    the dialog box needs to be displayed been initialised, pass the relevant data
    as InitData. If it is null, the dialog box is uninitialised. This also contains
    the user's choice as OUT parameter */
    HRESULT DILF_DisplayConfigDlg(PCHAR& InitData, int& Length) ;

    /* To set the configuration data for the currently selected DIL. Caller must
    release the memory. */
    //HRESULT DILF_SetConfigData(PCHAR pInitData,  ABS_CLUSTER_INFO   asClusterinfo, int Length, ABS_DEVICE_CONFIG  asDeviceConfig) ;
    HRESULT DILF_SetConfigData(CHANNEL_CONFIG& ouAbsSFibexContainer);
    /* Start the controller*/
    HRESULT DILF_StartHardware(void) ;

    /* Stop the controller*/
    HRESULT DILF_StopHardware(void) ;

    /* Reset Hardware*/
    HRESULT DILF_ResetHardware(void) ;

    /* Get the current controller status*/
    HRESULT DILF_GetCurrStatus(s_FLXSTATUSMSG& StatusData) ;

    /* Call to receive list of the transmittable messages*/
    HRESULT DILF_GetFlxTxMsgBuffer(CFlxTxMsgBuffer*& pouFlxTxMsgBuffer) ;

    /* Send Flx messages*/
    HRESULT DILF_SendFlxMsg(DWORD dwClientID, s_FLXTXMSG* pouFlxTxMsg) ;

    HRESULT DILF_DeleteFlxMsg(DWORD dwClientID, s_FLXTXMSG* pouFlxTxMsg) ;

    /* Get basic info of the board */
    HRESULT DILF_GetBoardInfo(s_BOARDINFO& BoardInfo) ;

    /* Get salient informations on current bus configuration */
    HRESULT DILF_GetFlexRayInfo(s_FLXINFO& FlexRayInfo) ;

    /* Call to receive the version informations */
    HRESULT DILF_GetVersionInfo(VERSIONINFO& sVerInfo) ;

    /* Call to get descriptive string of the last error occurred */
    HRESULT DILF_GetLastErrorString(std::string acErrorStr, HRESULT& nError ) ;

    /* Call to set PASS/STOP filter */
    HRESULT DILF_FilterFrames(FILTER_TYPE FilterType, FLEX_CHANNEL FlexChannel, UINT* punFrames, UINT nLength) ;

    /* Call to Configure Message buffer */
    HRESULT DILF_ConfigMsgBuf(s_MSGBUF sMsgBuf, UINT* punAddress) ;

    virtual HRESULT DILF_GetControllerCapabilities(s_FLXControllerCapabilities& ouFlexRayCapabilities);

    /* HELPER FUNCTIONS START */
    void vSelectInterface_Dummy(void);
};
