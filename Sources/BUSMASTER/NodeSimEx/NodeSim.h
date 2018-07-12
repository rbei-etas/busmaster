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
#include "IBMNetWorkGetService.h"
#include "GlobalObj.h"
#include "ExecuteManager.h"
class CGlobalObj;

class CNodeSim : public CBaseNodeSim
{
public:
    CNodeSim()
    {

    }
    CNodeSim(ETYPE_BUS eBus, CGlobalObj* pGlobalObj, CExecuteManager* pExecuteMgr);
    ~CNodeSim();
    BOOL InitInstance(void);
    int ExitInstance(void);

    //gives the time of connection
    __int64 m_n64TimeElapsedSinceConnection;
    //INTERFACE FUNCTIONS STARTS
    HRESULT FE_CreateFuncEditorTemplate(HWND handle, S_EXFUNC_PTR& sExInitStruct);
    HRESULT FE_OpenFunctioneditorFile(CString omStrNewCFileName, HWND hMainFrame,
                                      S_EXFUNC_PTR& sExInitStruct);
    void NS_ManageOnKeyHandler(UCHAR ucKey);
    virtual void NS_ManageBusEventHandler(eBUSEVEHANDLER eBusEvent);
    void NS_ManageOnMessageHandler(void* psRxMsgInfo);
    void NS_ManageOnErrorHandler(ERROR_STATE eErrorCode, void* pvErrorVal);
    HRESULT NS_DLLBuildAll();
    HRESULT NS_DllUnloadAll(CStringArray* pomStrErrorFiles);
    HRESULT NS_DLLBuildLoadAllEnabled();
    HRESULT NS_DLLUnloadAllEnabled();



    //Set the MSG structure name and File name containing the structure def
    virtual void NS_UpdateFuncStructsNodeSimEx(PVOID pvFuncStructs, E_UPDATE_TYPE eUpdateType);
    //Provide the helptext for all the handlers in the order mentoned above
    void NS_SetHandlersHelpText(CStringArray* pomTextArray);
    //Load and save the Simsys config details in terms of bytes
    virtual bool NS_GetSimSysConfigData(xmlNodePtr& pNodePtr);
    void NS_SetSimSysConfigData(BYTE* pSrcBuffer, int nBuffSize);
    void NS_SetSimSysConfigData(xmlDocPtr pXmlDoc);
    virtual void NS_SetSimSysConfigData(xmlNodePtr pXmlNodePtr);
    BOOL NS_IsSimSysConfigChanged();
    int NS_nOnBusConnected(bool bConnected);
    void NS_SetJ1939ActivationStatus(bool bActivated);
    // Save simulation file

    virtual void NS_SetBmNetworkConfig(IBMNetWorkGetService* ouLINConfig, bool bModified = false);

protected:
    CGlobalObj* mpGlobalObj;
    //INTERFACE FUNCTIONS ENDS
private:
    ETYPE_BUS m_eBus;
    CExecuteManager* m_pExecuteMgr;

};
