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
 * \file      AppServicesCAN.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "NodeSimEx_stdafx.h"
#include "Include/DIL_CommonDefs.h"
#include "ExecuteManager.h"
#include "GlobalObj.h"
#include "AppServicesCAN.h"

UINT gunSendMsg_CAN(STCAN_MSG* psTxMsg, HMODULE hModule)
{
    UINT Return = 1;
    VALIDATE_POINTER_RETURN_VAL(psTxMsg, Return);
    CExecuteFunc* pmCEexecuteFunc =
        CExecuteManager::ouGetExecuteManager(CAN).pmGetNodeObject(hModule);

    if (pmCEexecuteFunc != NULL)
    {
        BOOL bMsgTxFlag = pmCEexecuteFunc->bGetMsgTxFlag();

        if (bMsgTxFlag)
        {
            sNODEINFO sNode(CAN);
            pmCEexecuteFunc->vGetNodeInfo(sNode);

            if (CGlobalObj::GetICANDIL()->DILC_SendMsg(sNode.m_dwClientId, *psTxMsg) == S_OK)
            {
                Return = 0;
            }
        }
    }

    return Return;
}

void gvResetController_CAN(BOOL bEnable)
{
    ::SendMessage(CGlobalObj::sm_hWndMDIParentFrame, WM_FROM_USER_DLL, (WPARAM)RESET_HARDWARE_CONTROLLER,(LPARAM)bEnable);
}

/******************************************************************************
    Function Name    :  gvControllerMode
    Input(s)         :  bNewMode, new mode
    Output           :  Returned value from bSetControllerMode
                        function will be returned
    Functionality    :  This function will call bSetControllerMode which
                        is a member function of MainFrame and pass bNewMode
                        parameter to it
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  03-Mar-2003
    Modifications By :  Anish Kumar
    Modifications on :  13.09.2006, Added code for external evaluation copy
******************************************************************************/
BOOL gvSetControllerMode_CAN(BOOL /*bNewMode*/)
{
    BOOL bReturn = FALSE;
    return bReturn;
}

/******************************************************************************
    Function Name    :  gbDisconnect
    Input(s)         :  bState -> TRUE if Disconnect
    Output           :
    Functionality    :  This function will send a message to MainFrame that will
                        be handled by vDisconnect member of MainFrame
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ravikumar Patil
    Date Created     :  05-Mar-2003
    Modifications    :
******************************************************************************/

BOOL gbStartStopHardware_CAN(BOOL bState)
{
    ::SendMessage(CGlobalObj::sm_hWndMDIParentFrame, WM_FROM_USER_DLL, (WPARAM)DIS_CONNECT,(LPARAM)bState);
    return TRUE ;
}