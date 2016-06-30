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
 * @brief Interface file containing Base class which should be implemented for Flexray devices from different vendors
 * @author Arunkumar Karri
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file containing Base class which should be implemented for Flexray devices from different vendors
 */

#pragma once
#include <windows.h>
#include "FlexRayDriverDefines.h"
#include "../../BusmasterDBNetwork/Include/IBMNetWorkGetService.h"
class CBaseDIL_FLEXRAY_Controller
{
public:
    CBaseDIL_FLEXRAY_Controller() {};
    virtual HRESULT FLEXRAY_PerformInitOperations(void) = 0;
    virtual HRESULT FLEXRAY_PerformClosureOperations(void) = 0;
    virtual HRESULT FLEXRAY_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp) = 0;
    virtual HRESULT FLEXRAY_ListHwInterfaces(INTERFACE_HW& sSelHwInterface) = 0;
    virtual HRESULT FLEXRAY_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface) = 0;
    virtual HRESULT FLEXRAY_DeselectHwInterface(void) = 0;

    virtual HRESULT FLEXRAY_SetConfigData(IBMNetWorkGetService* ouAbsSFibexContainer) = 0;
    virtual HRESULT FLEXRAY_SetAppParams(HWND hWndOwner) = 0;
    virtual HRESULT FLEXRAY_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseFLEXBufFSE* pBufObj) = 0;
    virtual HRESULT FLEXRAY_StartHardware(void) = 0;
    virtual HRESULT FLEXRAY_StopHardware(void) = 0;
    virtual HRESULT FLEXRAY_ResetHardware(void) = 0;
    virtual HRESULT FLEXRAY_SendFlxMsg(DWORD dwClientID, s_FLXDATAMSG* pouFlxTxMsg) = 0;
    virtual HRESULT FLEXRAY_SendPDU(DWORD dwClientID, s_FLXPDUDATAMSG* pouFlxPDU, bool bSetUpdateBit) = 0;
    virtual HRESULT FLEXRAY_DeleteFlxMsg(DWORD dwClientID, s_FLXDATAMSG* pouFlxTxMsg) = 0;
    virtual HRESULT FLEXRAY_GetVersionInfo(VERSIONINFO& sVerInfo) = 0;
    virtual HRESULT FLEXRAY_GetLastErrorString(std::string acErrorStr, HRESULT& nError) = 0;
    virtual HRESULT FLEXRAY_LoadDriverLibrary(void) = 0;
    virtual HRESULT FLEXRAY_UnloadDriverLibrary(void) = 0;
    virtual HRESULT FLEXRAY_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName) = 0;

    virtual HRESULT FLEXRAY_GetControllerParams(std::list<FlexRayControllerParams>&) = 0;

};
