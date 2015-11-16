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
#include "AppServicesLIN.h"

UINT gunSendMsg_LIN(STLIN_MSG* psTxMsg, HMODULE hModule)
{
    UINT Return = 1;

    VALIDATE_POINTER_RETURN_VAL(psTxMsg, Return);
    CExecuteFunc* pmCEexecuteFunc =
        CExecuteManager::ouGetExecuteManager(LIN).pmGetNodeObject(hModule);
    STLIN_MSG       sMsg  ;

    sMsg.m_ucMsgID = psTxMsg->m_ucMsgID;

    sMsg.m_ucChannel     = psTxMsg->m_ucChannel;
    sMsg.m_ucChksum      = psTxMsg->m_ucChksum;
    sMsg.m_ucChksumTyp   = psTxMsg->m_ucChksumTyp;
    sMsg.m_ucDataLen     = psTxMsg->m_ucDataLen;
    sMsg.m_ucMsgID       = psTxMsg->m_ucMsgID;
    if ( psTxMsg->m_ucMsgTyp == 1 )
    {
        sMsg.m_ucMsgTyp      = LIN_SLAVE_RESPONSE;
    }
    else
    {
        sMsg.m_ucMsgTyp      = LIN_MASTER_RESPONSE;
    }

    memset(sMsg.m_ucData, 0, sMsg.m_ucDataLen);
    for(int j=0; j < 8; j++)
    {
        sMsg.m_ucData[j] = psTxMsg->m_ucData[j];
    }

    if (pmCEexecuteFunc != nullptr)
    {
        BOOL bMsgTxFlag = pmCEexecuteFunc->bGetMsgTxFlag();
        if (bMsgTxFlag)
        {
            if (CGlobalObj::GetILINDIL()->DILL_SendMsg(pmCEexecuteFunc->dwGetNodeClientId(), sMsg) == S_OK)
            {
                Return = 0;
            }
        }
    }
    return Return;
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
BOOL gvSetControllerMode_LIN(BOOL /*bNewMode*/)
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

BOOL gbStartStopHardware_LIN(BOOL bState)
{
    ::PostMessage(CGlobalObj::sm_hWndMDIParentFrame, WM_FROM_USER_DLL, (WPARAM)LIN_DIS_CONNECT,(LPARAM)bState);
    return TRUE ;
}
/******************************************************************************
    Function Name    :  gdGetFirstCANdbName
    Input(s)         :  cBuffer to store the output string
                        size of the output buffer
    Output           :  Returns the DB name in cBuffer
    Functionality    :  Returns the first Database name connected
                        to BUSMASTER
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ashwin. R. Uchil
    Date Created     :  9-5-2012
******************************************************************************/
DWORD gdGetFirstLINdbName(char* cBuffer, DWORD /* size */)
{
    strcpy(cBuffer,"");
    //cBuffer = nullptr;

    if (CExecuteManager::bIsExist((ETYPE_BUS)LIN) == TRUE)
    {
        POSITION        MainPos = nullptr;
        //CAPL_DB_NAME_CHANGE
        //loop through the DB list to search in all the DB whether the message is present.

        MainPos =  CGlobalObj::ouGetObj((ETYPE_BUS)0).m_odMsgNameMsgCodeListDb.GetTailPosition();// get only CAN db
        if(MainPos != nullptr)         //if present stop searching
        {
            SDB_NAME_MSG&  sDbNameMsg = CGlobalObj::ouGetObj((ETYPE_BUS)0).
                                        m_odMsgNameMsgCodeListDb.GetAt(MainPos);
            strcpy(cBuffer, (LPCSTR)sDbNameMsg.m_omDbName);
            //cBuffer = sDbNameMsg.m_omDbName.GetBuffer(sDbNameMsg.m_omDbName.GetLength());

            return (DWORD)TRUE;
        }
    }

    return (DWORD)FALSE;
}
BOOL gbWtiteToLog_LIN(char* pcString)
{
    BOOL Result = FALSE;
    if (CGlobalObj::sm_hWndMDIParentFrame != nullptr)
    {
        ::SendMessage(CGlobalObj::sm_hWndMDIParentFrame, WM_FROM_USER_DLL, (WPARAM)WRITE_TO_LOGFILE_LIN,(LPARAM)pcString);
        Result = TRUE;
    }
    return Result;
}
BOOL gbEnableDisableLog_LIN(BOOL bEnable)
{
    BOOL Result = FALSE;
    if (CGlobalObj::sm_hWndMDIParentFrame != nullptr)
    {
        ::PostMessage(CGlobalObj::sm_hWndMDIParentFrame, WM_FROM_USER_DLL, (WPARAM)LOG_ENABLE_DISABLE_LIN,(LPARAM)bEnable);
        Result = TRUE;
    }
    return Result;
}