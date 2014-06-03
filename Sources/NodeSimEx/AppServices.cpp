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
#include "NodeSim.h"
#include "Utility/Utility.h"

#include <afxdllx.h>
extern "C" HRESULT NS_GetInterface(ETYPE_BUS eBus,void** ppvInterface);

BOOL gbEnableDisableLog(BOOL bEnable)
{
    BOOL Result = FALSE;
    if (CGlobalObj::sm_hWndMDIParentFrame != nullptr)
    {
        ::PostMessage(CGlobalObj::sm_hWndMDIParentFrame, WM_FROM_USER_DLL, (WPARAM)LOG_ENABLE_DISABLE,(LPARAM)bEnable);
        Result = TRUE;
    }
    return Result;
}
BOOL gbWriteToLog(char* pcString)
{
    BOOL Result = FALSE;
    if (CGlobalObj::sm_hWndMDIParentFrame != nullptr)
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
            if (pmCEexecuteFunc != nullptr)
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
                if (pmCEexecuteFunc != nullptr)
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
            if(pmCEexecuteFunc!=nullptr)
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
            if (pmCEexecuteFunc != nullptr)
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
            if (pmCEexecuteFunc != nullptr)
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
            if (pmCEexecuteFunc != nullptr)
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
    HMODULE hModule = nullptr;
    CString omNode(pcNodeName);
    for (UINT i = 0; i < BUS_TOTAL; i++)
    {
        if (CExecuteManager::bIsExist((ETYPE_BUS)i) == TRUE)
        {
            hModule = CExecuteManager::ouGetExecuteManager((ETYPE_BUS)i).hReturnDllHandle(omNode);
            if (hModule != nullptr)
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
    if (nullptr != CGlobalObj::sm_pouITraceWndPtr)
    {
        return CGlobalObj::sm_pouITraceWndPtr->bWriteToTrace(pcOutStrTrace);
    }
    return TRUE;
}
/******************************************************************************
    Function Name    :  sg_KeyPressed
    Input(s)         :  NIL
    Output           :  int (ASCII code of key pressed)
                        function will be returned
    Functionality    :  This function will Send the AsCII code of key pressed
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :
    Date Created     :
******************************************************************************/
int sg_KeyPressed()
{
    bool            bCapsLockON = false,bShift = false;
    int             nChar =0;

    //Check the status of Caps lock and shift keys
    if(GetKeyState(VK_CAPITAL) & 0x0001)
    {
        bCapsLockON = true;
    }
    if((GetKeyState(VK_SHIFT) & 0x8000))
    {
        bShift = true;
    }

    //check the status of all the useful keys on the keyboard
    for (int iItr = 8; iItr < 223; iItr++)
    {
        if (GetKeyState(iItr) & 0x8000) //first bit indicates whether the key is pressed
        {
            //if Caps lock or shift ignore, since we are processing it seperately
            if((iItr == VK_CAPITAL)||(iItr==VK_LSHIFT)||(iItr==VK_SHIFT)||(iItr==VK_RSHIFT))
            {
                continue;
            }
            nChar = iItr;           //copy the virtual key code
        }
    }

    //for number pad
    if(nChar >= 96 && nChar <=105)
    {
        nChar -= 48;
        return nChar;
    }

    // if shift or caps lock is not pressed then convert to small
    if (!(bShift||bCapsLockON))
    {
        if(nChar >= 65 && nChar <=90)
        {
            nChar += 32;
            return nChar;
        }
    }

    switch(nChar)
    {
        case 0x38:                      //8 on keyboard, shift + 8 gives *
            if(!bShift)
            {
                break;
            }

        case VK_MULTIPLY:
            nChar = 42;
            break;

        case VK_OEM_PLUS:               //check for +/= key
            if(!bShift)
            {
                nChar = 61;     //ASCII code of "="
                break;
            }

        case VK_ADD:
            nChar = 43;
            break;

        case VK_OEM_MINUS:
            if(bShift)
            {
                nChar = 95;     //send ASCII code of "_"
                break;
            }

        case VK_SUBTRACT:
            nChar = 45;
            break;

        case VK_OEM_2:                  // "/" key on key board
            if(bShift)
            {
                nChar = 63;     //send ASCII code of "?"
                break;
            }
        case VK_DIVIDE:
            nChar = 47;         //send ASCII code for "/"
            break;
        case VK_OEM_3:
            if(bShift)
            {
                nChar = 126;    //send ASCII code of "~"
            }
            else
            {
                nChar = 96;    //send ASCII code of "`"
            }
            break;
        case 0x31:
            if(bShift)
            {
                nChar = 33;    //send ASCII code of "!"
            }
            break;
        case 0x32:
            if(bShift)
            {
                nChar = 64;    //send ASCII code of "@"
            }
            break;
        case 0x33:
            if(bShift)
            {
                nChar = 35;    //send ASCII code of "#"
            }
            break;
        case 0x34:
            if(bShift)
            {
                nChar = 36;    //send ASCII code of "$"
            }
            break;
        case 0x35:
            if(bShift)
            {
                nChar = 37;    //send ASCII code of "%"
            }
            break;
        case 0x36:
            if(bShift)
            {
                nChar = 94;    //send ASCII code of "^"
            }
            break;
        case 0x37:
            if(bShift)
            {
                nChar = 38;    //send ASCII code of "&"
            }
            break;
        case 0x39:
            if(bShift)
            {
                nChar = 40;    //send ASCII code of "("
            }
            break;
        case 0x30:
            if(bShift)
            {
                nChar = 41;    //send ASCII code of ")"
            }
            break;
        case VK_OEM_4:
            if(!bShift)
            {
                nChar = 91;    //send ASCII code of "["
            }
            else
            {
                nChar = 123;    //send ASCII code of "{"
            }
            break;
        case VK_OEM_5:
            if(!bShift)
            {
                nChar = 92;    //send ASCII code of "\"
            }
            else
            {
                nChar = 124;    //send ASCII code of "|"
            }
            break;
        case VK_OEM_6:
            if(!bShift)
            {
                nChar = 93;    //send ASCII code of "]"
            }
            else
            {
                nChar = 125;
            }    //send ASCII code of "}"
            break;
        case VK_OEM_1:
            if(!bShift)
            {
                nChar = 59;    //send ASCII code of ";"
            }
            else
            {
                nChar = 58;    //send ASCII code of ":"
            }
            break;
        case VK_OEM_7:
            if(!bShift)
            {
                nChar = 39;    //send ASCII code of "'"
            }
            else
            {
                nChar = 34;    //send ASCII code of """
            }
            break;
        case VK_OEM_COMMA:
            if(!bShift)
            {
                nChar = 44;    //send ASCII code of ","
            }
            else
            {
                nChar = 60;    //send ASCII code of "<"
            }
            break;
        case VK_OEM_PERIOD:
            if(!bShift || VK_DECIMAL)
            {
                nChar = 46;    //send ASCII code of "."
            }
            else
            {
                nChar = 62;    //send ASCII code of ">"
            }
            break;
        case VK_DECIMAL:
            nChar = 46;     //send ASCII code of "."
    }
    return nChar;

}
/******************************************************************************
    Function Name    :  sg_GetMessageName
    Input(s)         :  Message ID as DWORD,
                        Network name(dContext) as DWORD,
                        OutPut message name as char array pBuffer,
                        Length of output buffer as dSize.
    Output           :  Message name will be returned from database
    Functionality    :  This function return the message name of the message ID
                        from the databases attached
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :
    Date Created     :
******************************************************************************/
int sg_GetMessageName(DWORD dID, DWORD dContext, char* pBuffer,DWORD dSize)
{
    //CAPL_NEW_ADDITIONS
    strcpy(pBuffer, "");
    ETYPE_BUS m_eBus = (ETYPE_BUS)dContext;     //set the network name
    if(!((m_eBus == CAN)||(m_eBus == J1939) || (m_eBus == LIN)))
    {
        //CString    omErrorStr("Invalid channel");
        //MessageBox(omErrorStr);
        return 0;
    }

    POSITION        MainPos = nullptr, pos = nullptr;
    //CAPL_DB_NAME_CHANGE
    //loop through the DB list to search in all the DB whether the message is present.
    MainPos =  CGlobalObj::ouGetObj(m_eBus).m_odMsgNameMsgCodeListDb.GetHeadPosition();

    while(MainPos != nullptr)
    {
        SDB_NAME_MSG&  sDbNameMsg = CGlobalObj::ouGetObj(m_eBus).
                                    m_odMsgNameMsgCodeListDb.GetNext(MainPos);
        pos = sDbNameMsg.m_oMsgNameMsgCodeList.GetHeadPosition();
        while(pos != nullptr)          //if present stop searching
        {
            SMSG_NAME_CODE& sMsgNameCode =  sDbNameMsg.m_oMsgNameMsgCodeList.GetNext(pos);
            //Compare the IDs and send the message name  if it matches
            if(sMsgNameCode.m_dwMsgCode == dID)
            {
                if((int)dSize < sMsgNameCode.m_omMsgName.GetLength())
                {
                    //CString    omErrorStr("Buffer length is too small to store the message name");
                    //AfxMessageBox(omErrorStr);
                    return 0;
                }
                else
                {
                    try
                    {
                        strcpy(pBuffer, sMsgNameCode.m_omMsgName.GetBuffer(0));
                    }
                    catch(CException * /* e */)
                    {
                        return 0;
                    }
                    return 1;
                }
            }
        }
    }


    //POSITION pos = CGlobalObj::ouGetObj(m_eBus).m_odMsgNameMsgCodeList.GetHeadPosition();
    //   while (pos != nullptr)
    //   {
    //  //Get the individual message structure
    //       SMSG_NAME_CODE& sMsgNameCode = CGlobalObj::ouGetObj(m_eBus).m_odMsgNameMsgCodeList.
    //                                                       GetNext(pos);

    //
    //   }
    // }
    return 0;
}

/******************************************************************************
    Function Name    :  sg_GetTimeNow
    Input(s)         :  -
    Output           :  Returns the absolute time
    Functionality    :  Returns the absolute time(time from connection)
    Member of        :  None (Global function)
    Friend of        :  None
    Author(s)        :  Ashwin. R. Uchil
    Date Created     :  3-5-2012
******************************************************************************/
long sg_TimeNow()
{
    long        lAbsoluteTime = 0;

    if (CGlobalObj::sm_hWndMDIParentFrame != nullptr)          //check for mainframe
    {
        __int64                 nCurrentTime =0, nTimeElapsed =0;
        LARGE_INTEGER           f1;


        QueryPerformanceFrequency(&f1);

        nCurrentTime = gnGetCpuClocks();
        CNodeSim* Result = nullptr;


        if (NS_GetInterface(CAN, (void**) &Result) == S_OK)                 //get the interface of NodeSim to get the connection time
        {
            if(Result->m_n64TimeElapsedSinceConnection == 0)
            {
                return 0;
            }

            lAbsoluteTime = ((nCurrentTime - Result->m_n64TimeElapsedSinceConnection)*1000000)/f1.QuadPart;
        }

    }
    return lAbsoluteTime;
}
