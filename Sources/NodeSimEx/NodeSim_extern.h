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
 * \file      NodeSim_extern.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#if !defined NODESIM_EXTERN_H__INCLUDED_
#define NODESIM_EXTERN_H__INCLUDED_

#include "NodeSimEx_Struct.h"
#include "DataTypes/BaseAppServices.h"

#if defined USAGEMODE
#undef USAGEMODE
#endif

#if defined USAGE_EXPORT
#define USAGEMODE   __declspec(dllexport)
#else
#define USAGEMODE   __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif

#include "include/BaseDefs.h"
//There should not be any message defined as below, they are used for loading
//and saving of message wnd config
#define MSG_WND_GET_DATA	(WM_USER + 91)
#define MSG_WND_SET_DATA	(WM_USER + 92)
USAGEMODE void CreateFuncEditorTemplate(HWND handle, ETYPE_BUS eBUS, S_EXFUNC_PTR& sExInitStruct);
USAGEMODE HRESULT OpenFunctioneditorfile(CString omStrNewCFileName, HWND hMainFrame, ETYPE_BUS eBUS, S_EXFUNC_PTR& sExInitStruct);
USAGEMODE void vEnableAllHandlers(BOOL bEnable);
USAGEMODE void vEnableAllTimers(BOOL bEnable);
USAGEMODE void vEnableAllKeyHandler(BOOL bEnable);
USAGEMODE void vEnableAllMessageHandler(BOOL bEnable);
USAGEMODE void vEnableAllErrorHandler(BOOL bEnable);
USAGEMODE void vManageOnKeyHandler(UCHAR ucKey);
USAGEMODE void vManageOnMessageHandler(void *psRxMsgInfo);
USAGEMODE void vManageOnErrorHandler(eERROR_STATE eErrorCode,void* pvErrorVal);
USAGEMODE BOOL bDLLBuildAll(CStringArray *pomStrErrorFiles);
USAGEMODE BOOL bDllLoadAll(CStringArray *pomStrErrorFiles); 
USAGEMODE BOOL bDllUnloadAll(CStringArray *pomStrErrorFiles);
USAGEMODE BOOL bDLLBuildLoadAll(CStringArray *pomStrErrorFiles);
//Set the MSG structure name and File name containing the structure def
USAGEMODE void vUpdateFuncStructsNodeSimEx(PVOID pvFuncStructs, E_UPDATE_TYPE eUpdateType);
//Position of Function Editor's Help texts
#define MSG_HANDLER_TEXT_POS                  0
#define TIMER_HANDLER_TEXT_POS                1
#define KEY_HANDLER_TEXT_POS                  2
#define ERROR_HANDLER_TEXT_POS                3
#define DLL_HANDLER_TEXT_POS                  4
#define UTILITY_FUNC_TEXT_POS                 5
//Provide the helptext for all the handlers in the order mentoned above
USAGEMODE void vSetHandlersHelpText(CStringArray *pomTextArray);
//Find the value of flags which will tell the status of toolbar item
USAGEMODE BOOL bShouldToolBarBeEnabled(E_TB_FN_EDITOR eToolBarFlag);
//Find the value of flags which will tell the status of handler s
USAGEMODE BOOL bGetHandlerStatus(eSIMSYSFLAG eHandlerFlag);
//Load and save the Simsys config details in terms of bytes
USAGEMODE void vGetSimSysConfigData(BYTE*& pDesBuffer, int& nBuffSize);
USAGEMODE void vSetSimSysConfigData(BYTE* pSrcBuffer, int nBuffSize);
USAGEMODE BOOL bIsSimSysConfigChanged();
 
#ifdef __cplusplus
}
#endif

#endif
