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
 * \file      NodeSim.h
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "BaseNodeSim.h"
class CNodeSim : public CBaseNodeSim
{
public:
    CNodeSim(ETYPE_BUS eBus);
    ~CNodeSim();
    BOOL InitInstance(void);
    int ExitInstance(void);

    //INTERFACE FUNCTIONS STARTS
    HRESULT FE_CreateFuncEditorTemplate(HWND handle, S_EXFUNC_PTR& sExInitStruct);
    HRESULT FE_OpenFunctioneditorFile(CString omStrNewCFileName, HWND hMainFrame,
                                      S_EXFUNC_PTR& sExInitStruct);
    void NS_EnableAllHandlers(BOOL bEnable);
    void NS_EnableAllTimers(BOOL bEnable);
    void NS_EnableAllKeyHandler(BOOL bEnable);
    void NS_EnableAllMessageHandler(BOOL bEnable);
    void NS_EnableAllErrorHandler(BOOL bEnable);
    void NS_ManageOnKeyHandler(UCHAR ucKey);
    void NS_ManageOnMessageHandler(void* psRxMsgInfo);
    void NS_ManageOnErrorHandler(eERROR_STATE eErrorCode,void* pvErrorVal);
    HRESULT NS_DLLBuildAll(CStringArray* pomStrErrorFiles);
    HRESULT NS_DllLoadAll(CStringArray* pomStrErrorFiles);
    HRESULT NS_DllUnloadAll(CStringArray* pomStrErrorFiles);
    HRESULT NS_DLLBuildLoadAll(CStringArray* pomStrErrorFiles);
    //Set the MSG structure name and File name containing the structure def
    void NS_UpdateFuncStructsNodeSimEx(PVOID pvFuncStructs, E_UPDATE_TYPE eUpdateType);
    //Provide the helptext for all the handlers in the order mentoned above
    void NS_SetHandlersHelpText(CStringArray* pomTextArray);
    //Find the value of flags which will tell the status of toolbar item
    BOOL NS_ShouldToolBarBeEnabled(E_TB_FN_EDITOR eToolBarFlag);
    //Find the value of flags which will tell the status of handler s
    BOOL NS_GetHandlerStatus(eSIMSYSFLAG eHandlerFlag);
    //Load and save the Simsys config details in terms of bytes
    void NS_GetSimSysConfigData(BYTE*& pDesBuffer, int& nBuffSize);
    void NS_SetSimSysConfigData(BYTE* pSrcBuffer, int nBuffSize);
    BOOL NS_IsSimSysConfigChanged();
    //INTERFACE FUNCTIONS ENDS
private:
    ETYPE_BUS m_eBus;
};
