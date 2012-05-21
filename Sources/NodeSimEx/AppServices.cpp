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
 * \file      AppServices.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "NodeSimEx_stdafx.h"
#include "ExecuteManager.h"
#include "GlobalObj.h"
#include "AppServices.h"
#include "DataTypes\BaseAppServices.h"

#include <afxdllx.h>

BOOL gbEnableDisableLog(BOOL bEnable)
{
    BOOL Result = FALSE;

    if (CGlobalObj::sm_hWndMDIParentFrame != NULL)
    {
        ::PostMessage(CGlobalObj::sm_hWndMDIParentFrame, WM_FROM_USER_DLL, (WPARAM)LOG_ENABLE_DISABLE,(LPARAM)bEnable);
        Result = TRUE;
    }

    return Result;
}
BOOL gbWriteToLog(char* pcString)
{
    BOOL Result = FALSE;

    if (CGlobalObj::sm_hWndMDIParentFrame != NULL)
    {
        ::SendMessage(CGlobalObj::sm_hWndMDIParentFrame, WM_FROM_USER_DLL, (WPARAM)WRITE_TO_LOGFILE,(LPARAM)pcString);
        Result = TRUE;
    }

    return Result;
}

/******************************************************************************
    Function Name    :  gbActivateDeactivateHandlers
    Input(s)         :  bActivate -> TRUE if Activate
    Output           :  Returned value from bActivateDeactivateHandlers
                        function will be returned
    Functionality    :  This function will call bActivateDeactivateHandlers which
                        is a member function of MainFrame and pass bActivate
                        parameter to it
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :
    Date Created     :
    Modifications    :
******************************************************************************/
BOOL gbActivateDeactivateHandlers(BOOL bActivate, HMODULE hModule)
{
    BOOL Result = FALSE;

    for (UINT i = 0; i < BUS_TOTAL; i++)
    {
        if (CExecuteManager::bIsExist((ETYPE_BUS)i) == TRUE)
        {
            //for removing C4800 WARNING
            bool bActuiveTemp = false;

            if(bActivate > 0)
            {
                bActuiveTemp = true;
            }

            if (CExecuteManager::ouGetExecuteManager((ETYPE_BUS)i).bActivateDeactivateHandlers(
                        bActuiveTemp, hModule) == TRUE)
            {
                CExecuteManager::ouGetExecuteManager((ETYPE_BUS)i).vUpdateHandlerDetailsInDetView();
                Result = TRUE;
                i = BUS_TOTAL;
            }
        }
    }

    return Result;
}

/******************************************************************************
    Function Name    :  gbSetResetTimer
    Input(s)         :  pcTimerFunctionName ->Name of the timer
                        type ->Type of the timer (cyclic or
                        bStart ->Flag.  TRUE->Start Timer, FALSE->Stop Timer
    Output           :  Returned value from bSetResetTimer function will be returned
    Functionality    :  This function will call bSetResetTimer which is a member
                        function of MainFrame and pass the same parameters to it.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :
    Date Created     :
    Modifications    :
******************************************************************************/

BOOL gbSetResetTimer(CHAR* pcTimerFunctionName, int type, BOOL bStart,
                     HMODULE hModule)
{
    BOOL Result = FALSE;

    for (UINT i = 0; i < BUS_TOTAL; i++)
    {
        if (CExecuteManager::bIsExist((ETYPE_BUS)i) == TRUE)
        {
            CExecuteFunc* pmCEexecuteFunc =
                CExecuteManager::ouGetExecuteManager((ETYPE_BUS)i).pmGetNodeObject(hModule);

            //If handler is found
            if (pmCEexecuteFunc != NULL)
            {
                CExecuteManager::ouGetExecuteManager((ETYPE_BUS)i).vManageTimerThreads();
                Result = pmCEexecuteFunc->bResetTimer(pcTimerFunctionName, !type, bStart);
                i = BUS_TOTAL;
            }
        }
    }

    return Result;
}

/******************************************************************************
    Function Name    :  gbSetTimerVal
    Input(s)         :  pcTimerFunctionName ->Name of the timer
                        unTimeVal ->Timer value
    Output           :  Returned value from bSetTimerVal function will be returned
    Functionality    :  This function will call bSetTimerVal which is a member
                        function of MainFrame and pass the same parameters to it.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :
    Date Created     :
    Modifications    :
******************************************************************************/
BOOL gbSetTimerVal(CHAR* pcTimerFunctionName, UINT unTimeVal,HMODULE hModule)
{
    BOOL Result = FALSE;

    if (unTimeVal > 0)
    {
        for (UINT i = 0; i < BUS_TOTAL; i++)
        {
            if (CExecuteManager::bIsExist((ETYPE_BUS)i) == TRUE)
            {
                CExecuteFunc* pmCEexecuteFunc =
                    CExecuteManager::ouGetExecuteManager((ETYPE_BUS)i).pmGetNodeObject(hModule);

                //If handler is found
                if (pmCEexecuteFunc != NULL)
                {
                    Result = pmCEexecuteFunc->bResetTimerVal(pcTimerFunctionName, unTimeVal);
                    i = BUS_TOTAL; //break the loop
                }
            }
        }
    }

    return Result;
}

/******************************************************************************
    Function Name    :  gbEnableDisableMsgHandlers
    Input(s)         :  bEnable -> TRUE or FALSE
    Output           :  TRUE or FALSE
    Functionality    :  This function will call vEnableDisableHandlers which
                        is a member function of MainFrame.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :
    Date Created     :
    Modifications    :
******************************************************************************/
BOOL gbEnableDisableMsgHandlers(BOOL bEnable, HMODULE hModule)
{
    BOOL Result = FALSE;

    for (UINT i = 0; i < BUS_TOTAL; i++)
    {
        if (CExecuteManager::bIsExist((ETYPE_BUS)i) == TRUE)
        {
            CExecuteFunc* pmCEexecuteFunc=
                CExecuteManager::ouGetExecuteManager((ETYPE_BUS)i).pmGetNodeObject(hModule);

            //If handler is found
            if(pmCEexecuteFunc!=NULL)
            {
                Result = pmCEexecuteFunc->bEnableDisableMsgHandlers(bEnable);
                CExecuteManager::ouGetExecuteManager((ETYPE_BUS)i).vUpdateHandlerDetailsInDetView();
                i = BUS_TOTAL;
            }
        }
    }

    return Result;
}

/******************************************************************************
    Function Name    :  gbEnableDisableKeyHandlers
    Input(s)         :  bEnable -> TRUE or FALSE
    Output           :  TRUE or FALSE
    Functionality    :  This function will call vEnableDisableHandlers which
                        is a member function of MainFrame.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :
    Date Created     :
    Modifications    :
******************************************************************************/
BOOL gbEnableDisableKeyHandlers(BOOL bEnable, HMODULE hModule)
{
    BOOL Result = FALSE;

    for (UINT i = 0; i < BUS_TOTAL; i++)
    {
        if (CExecuteManager::bIsExist((ETYPE_BUS)i) == TRUE)
        {
            CExecuteFunc* pmCEexecuteFunc =
                CExecuteManager::ouGetExecuteManager((ETYPE_BUS)i).pmGetNodeObject(hModule);

            //If handler is found
            if (pmCEexecuteFunc != NULL)
            {
                Result = pmCEexecuteFunc->bEnableDisableKeyHandlers(bEnable);
                CExecuteManager::ouGetExecuteManager((ETYPE_BUS)i).vUpdateHandlerDetailsInDetView();
                i = BUS_TOTAL; //break the loop
            }
        }
    }

    return Result;
}

/******************************************************************************
    Function Name    :  gbEnableDisableErrorHandlers
    Input(s)         :  bEnable -> TRUE or FALSE
    Output           :  TRUE or FALSE
    Functionality    :  This function will call vEnableDisableHandlers which
                        is a member function of MainFrame.
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :
    Date Created     :
    Modifications    :
******************************************************************************/
BOOL gbEnableDisableErrorHandlers(BOOL bEnable, HMODULE hModule)
{
    BOOL Result = FALSE;

    for (UINT i = 0; i < BUS_TOTAL; i++)
    {
        if (CExecuteManager::bIsExist((ETYPE_BUS)i) == TRUE)
        {
            CExecuteFunc* pmCEexecuteFunc =
                CExecuteManager::ouGetExecuteManager((ETYPE_BUS)i).pmGetNodeObject(hModule);

            //If handler is found
            if (pmCEexecuteFunc != NULL)
            {
                Result = pmCEexecuteFunc->bEnableDisableErrorHandlers(bEnable);
                CExecuteManager::ouGetExecuteManager((ETYPE_BUS)i).vUpdateHandlerDetailsInDetView();
                i = BUS_TOTAL;
            }
        }
    }

    return Result;
}

/******************************************************************************
    Function Name    :  gbMsgTransmissionOnOff
    Input(s)         :  bActivate -> TRUE if Activate
    Output           :
    Functionality    :  This function will call call set MsgTxFlag of
                        CExecuteFunc to start/stop message transmission
                        from the DLL
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Anish kumar
    Date Created     :  05-01-2006
    Modifications    :
******************************************************************************/
BOOL gbMsgTransmissionOnOff(BOOL bOnOff, HMODULE hModule)
{
    BOOL Result = FALSE;

    for (UINT i = 0; i < BUS_TOTAL; i++)
    {
        if (CExecuteManager::bIsExist((ETYPE_BUS)i) == TRUE)
        {
            CExecuteFunc* pmCEexecuteFunc =
                CExecuteManager::ouGetExecuteManager((ETYPE_BUS)i).pmGetNodeObject(hModule);

            //If handler is found
            if (pmCEexecuteFunc != NULL)
            {
                pmCEexecuteFunc->vSetMsgTxFlag(bOnOff);
                Result = TRUE;
                i = BUS_TOTAL;
            }
        }
    }

    return Result;
}


/******************************************************************************
    Function Name    :  ghGetNodeDllHandler
    Input(s)         :  NodeName
    Output           :
    Functionality    :  This function will get the node's dll handler from
                        CExecuteManager
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Anish kumar
    Date Created     :  20-12-2006
    Modifications    :
******************************************************************************/
HMODULE ghGetNodeDllHandler(char* pcNodeName)
{
    HMODULE hModule = NULL;
    CString omNode(pcNodeName);

    for (UINT i = 0; i < BUS_TOTAL; i++)
    {
        if (CExecuteManager::bIsExist((ETYPE_BUS)i) == TRUE)
        {
            hModule = CExecuteManager::ouGetExecuteManager((ETYPE_BUS)i).hReturnDllHandle(omNode);

            if (hModule != NULL)
            {
                i = BUS_TOTAL;
            }
        }
    }

    return hModule;
}

BOOL gbGetProgramVersion(int* /*pnMajorVersion*/, int* /*pnMinorVersion*/)
{
    BOOL Result = FALSE;
    ASSERT(FALSE);
    return Result;
}

/******************************************************************************
    Function Name    :  gbSendStrToTrace
    Input(s)         :  pcOutStrTrace, string to be sent to trace
    Output           :  BOOL value (True always)
                        function will be returned
    Functionality    :  This function will Send a message and pass the string
                        to be printed
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :
    Date Created     :
******************************************************************************/
BOOL gbSendStrToTrace(char* pcOutStrTrace)
{
    return CGlobalObj::sm_pouITraceWndPtr->bWriteToTrace(pcOutStrTrace);
}