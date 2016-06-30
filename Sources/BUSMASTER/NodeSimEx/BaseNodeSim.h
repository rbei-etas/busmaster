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
 * \file      BaseNodeSim.h
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "Utility/XMLUtils.h"
#include "NodeSimEx_Struct.h"
#include "DataTypes/BaseAppServices.h"
#include "BaseNodeSim.h"
#include "include/BaseDefs.h"
#include "IBMNetWorkGetService.h"
#include "CANDriverDefines.h"


class CBaseNodeSim
{
public:

    //INTERFACE FUNCTIONS STARTS
    virtual HRESULT FE_CreateFuncEditorTemplate(HWND handle, S_EXFUNC_PTR& sExInitStruct) = 0;
    virtual HRESULT FE_OpenFunctioneditorFile(CString omStrNewCFileName, HWND hMainFrame,
            S_EXFUNC_PTR& sExInitStruct) = 0;
    virtual void NS_ManageOnKeyHandler(UCHAR ucKey) = 0;
    virtual void NS_ManageBusEventHandler(eBUSEVEHANDLER eBusEvent) = 0;
    virtual void NS_ManageOnMessageHandler(void* psRxMsgInfo) = 0;
    virtual void NS_ManageOnErrorHandler(ERROR_STATE eErrorCode,void* pvErrorVal) = 0;
    virtual HRESULT NS_DLLBuildAll() = 0;
    virtual HRESULT NS_DllUnloadAll(CStringArray* pomStrErrorFiles) = 0;
    virtual HRESULT NS_DLLBuildLoadAllEnabled() = 0;
    virtual HRESULT NS_DLLUnloadAllEnabled() = 0;

    //Set the MSG structure name and File name containing the structure def
    virtual void NS_UpdateFuncStructsNodeSimEx(PVOID pvFuncStructs, E_UPDATE_TYPE eUpdateType) = 0;
    //Provide the helptext for all the handlers in the order mentoned above
    virtual void NS_SetHandlersHelpText(CStringArray* pomTextArray) = 0;
    //Load and save the Simsys config details in terms of bytes
    virtual bool NS_GetSimSysConfigData(xmlNodePtr& pNodePtr) = 0;
    virtual void NS_SetSimSysConfigData(BYTE* pSrcBuffer, int nBuffSize) = 0;
    virtual void NS_SetSimSysConfigData(xmlDocPtr pXmlDoc) = 0;
    virtual void NS_SetSimSysConfigData(xmlNodePtr pXmlNodePtr) = 0;
    virtual BOOL NS_IsSimSysConfigChanged() = 0;
    virtual int NS_nOnBusConnected(bool bConnected) = 0;
    virtual void NS_SetJ1939ActivationStatus(bool bActivated) =0;
    //INTERFACE FUNCTIONS ENDS

    // FOR Passing Cluster Config
    virtual void NS_SetBmNetworkConfig( IBMNetWorkGetService* ouLINConfig, bool bModified = false ) = 0;
};
