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
 * \file      CAN_Vector_XL.cpp
 * \brief     Source file for Vector XL DIL functions
 * \author    Tobias Lorenz
 * \copyright Copyright (c) 2011, ETAS GmbH. All rights reserved.
 *
 * Source file for Vector XL DIL functions
 */
// CAN_Vector_XL.cpp : Defines the initialization routines for the DLL.
//

#include "CAN_Vector_XL_stdafx.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "Include/BaseDefs.h"
#include "Include/DIL_CommonDefs.h"
#include "EXTERNAL_INCLUDE/vxlapi.h"
#include "Include/CanUsbDefs.h"
#include "Include/Can_Error_Defs.h"
#include "ConfigDialogsDIL/API_Dialog.h"


#define USAGE_EXPORT
#include "CAN_Vector_XL_Extern.h"


/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Sets the application params.
 */
USAGEMODE HRESULT CAN_Vector_XL_SetAppParams(HWND /*hWndOwner*/, Base_WrapperErrorLogger* /*pILog*/)
{
    return WARN_DUMMY_API;
}


/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Unloads the driver library.
 */
USAGEMODE HRESULT CAN_Vector_XL_UnloadDriverLibrary(void)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Registers the buffer pBufObj to the client ClientID
 */
USAGEMODE HRESULT CAN_Vector_XL_ManageMsgBuf(BYTE /*bAction*/, DWORD /*ClientID*/, CBaseCANBufFSE* /*pBufObj*/)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Registers a client to the DIL. ClientID will have client id
 * which will be used for further client related calls  
 */
USAGEMODE HRESULT CAN_Vector_XL_RegisterClient(BOOL /*bRegister*/, DWORD& /*ClientID*/, TCHAR* /*pacClientName*/)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Returns the controller status. hEvent will be registered
 * and will be set whenever there is change in the controller
 * status.
 */
USAGEMODE HRESULT CAN_Vector_XL_GetCntrlStatus(const HANDLE& /*hEvent*/, UINT& /*unCntrlStatus*/)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Loads BOA related libraries. Updates BOA API pointers
 */
USAGEMODE HRESULT CAN_Vector_XL_LoadDriverLibrary(void)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Performs intial operations.
 * Initializes filter, queue, controller config
 * with default values.
 */
USAGEMODE HRESULT CAN_Vector_XL_PerformInitOperations(void)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Copies the controller config values into channel's
 * controller config structure.
 */
static BOOL bLoadDataFromContr(PSCONTROLER_DETAILS /*pControllerDetails*/)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Performs closure operations.
 */
USAGEMODE HRESULT CAN_Vector_XL_PerformClosureOperations(void)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets the time mode mapping of the hardware. CurrSysTime
 * will be updated with the system time ref.
 * TimeStamp will be updated with the corresponding timestamp.
 */
USAGEMODE HRESULT CAN_Vector_XL_GetTimeModeMapping(SYSTEMTIME& /*CurrSysTime*/, UINT64& /*TimeStamp*/, LARGE_INTEGER* /*QueryTickCount*/)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Lists the hardware interface available. sSelHwInterface
 * will contain the user selected hw interface.
 */
USAGEMODE HRESULT CAN_Vector_XL_ListHwInterfaces(INTERFACE_HW_LIST& /*asSelHwInterface*/, INT& /*nCount*/)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Selects the hardware interface selected by the user.
 */
USAGEMODE HRESULT CAN_Vector_XL_SelectHwInterface(const INTERFACE_HW_LIST& /*asSelHwInterface*/, INT /*nCount*/)
{   
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Deselects the selected hardware interface.
 */
USAGEMODE HRESULT CAN_Vector_XL_DeselectHwInterface(void)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Displays the controller configuration dialog.
 * Fields are initialized with values supplied by InitData.
 * InitData will be updated with the user selected values.
 */
USAGEMODE HRESULT CAN_Vector_XL_DisplayConfigDlg(PCHAR& /*InitData*/, INT& /*Length*/)
{
    return WARN_DUMMY_API;
}

/**
 * S_OK for success, S_FALSE for failure
 *
 * Sets the controller configuration data supplied by InitData.
 */
USAGEMODE HRESULT CAN_Vector_XL_SetConfigData(PCHAR /*pInitData*/, INT /*Length*/)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Starts the controller.
 */
USAGEMODE HRESULT CAN_Vector_XL_StartHardware(void)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Stops the controller.
 */
USAGEMODE HRESULT CAN_Vector_XL_StopHardware(void)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Resets the controller.
 */
USAGEMODE HRESULT CAN_Vector_XL_ResetHardware(void)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets the Tx queue configured.
 */
USAGEMODE HRESULT CAN_Vector_XL_GetTxMsgBuffer(BYTE*& /*pouFlxTxMsgBuffer*/)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Sends STCAN_MSG structure from the client dwClientID.   
 */
USAGEMODE HRESULT CAN_Vector_XL_SendMsg(DWORD /*dwClientID*/, const STCAN_MSG& /*sCanTxMsg*/)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets board info.
 */
USAGEMODE HRESULT CAN_Vector_XL_GetBoardInfo(s_BOARDINFO& /*BoardInfo*/)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets bus config info.
 */
USAGEMODE HRESULT CAN_Vector_XL_GetBusConfigInfo(BYTE* /*BusInfo*/)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets driver version info.
 */
USAGEMODE HRESULT CAN_Vector_XL_GetVersionInfo(VERSIONINFO& /*sVerInfo*/)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets last occured error and puts inside acErrorStr.
 */
USAGEMODE HRESULT CAN_Vector_XL_GetLastErrorString(CHAR* /*acErrorStr*/, INT /*nLength*/)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Applies FilterType(PASS/STOP) filter for corresponding
 * channel. Frame ids are supplied by punMsgIds.
 */
USAGEMODE HRESULT CAN_Vector_XL_FilterFrames(FILTER_TYPE /*FilterType*/, TYPE_CHANNEL /*Channel*/, UINT* /*punMsgIds*/, UINT /*nLength*/)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets the controller param eContrParam of the channel.
 * Value stored in lParam.
 */
USAGEMODE HRESULT CAN_Vector_XL_GetControllerParams(LONG& /*lParam*/, UINT /*nChannel*/, ECONTR_PARAM /*eContrParam*/)
{
    return WARN_DUMMY_API;
}

/**
 * \return S_OK for success, S_FALSE for failure
 *
 * Gets the error counter for corresponding channel.
 */
USAGEMODE HRESULT CAN_Vector_XL_GetErrorCount(SERROR_CNT& /*sErrorCnt*/, UINT /*nChannel*/, ECONTR_PARAM /*eContrParam*/)
{
    return WARN_DUMMY_API;
}
