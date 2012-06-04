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
 * \file      Application.h
 * \brief     CCANMonitorApp class implementation file
 * \author    Anish kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CCANMonitorApp class implementation file
 */

#pragma once

// For DIL Interface Class
#include "Include/BaseDefs.h"
// License validator interface, needed to query for other interfaces
#include "PSDI_Extern.h"
#include "CommonClass/MsgContainerBase.h"
//#include "DataTypes/MsgBufAll_Datatypes.h"
//#include "DataTypes/DIL_Datatypes.h"
//#include "DIL_Interface/BaseDIL_CAN.h"

// For CAN logger interface
#include "include/ModuleID.h"
#include "FrameProcessor/FrameProcessor_extern.h"
#include "FrameProcessor/BaseFrameProcessor_CAN.h"
#include "Struct.h"
#include "Utility/Utility.h"
#include "BUSMASTER_Interface.h"
#include "BUSMASTER.h"
#include "MainFrm.h"
#include "DIL_interface/BaseDIL_CAN.h"

#include "InterfaceGetter.h"

extern CCANMonitorApp theApp;

extern SBUSSTATISTICS g_sBusStatistics[ defNO_OF_CHANNELS ];



class CApplication : public CCmdTarget
{
public:
    CApplication(void);
    ~CApplication(void);

    virtual void OnFinalRelease()
    {
        CCmdTarget::OnFinalRelease();
    }

    DECLARE_DYNCREATE(CApplication)
    DECLARE_MESSAGE_MAP()
    DECLARE_OLECREATE(CApplication)
    DECLARE_DISPATCH_MAP()
    DECLARE_INTERFACE_MAP()

    enum
    {
        dispidConnect               = 1L,
        dispidSetHwFilter           = 2L,
        dispidGetHwFilter           = 3L,
        dispidBuildAllDll           = 4L,
        dispidBuildLoadAllDll       = 5L,
        dispidLoadAllDll            = 6L,
        dispidUnLoadAllDll          = 7L,
        dispidStartTxMsgBlock       = 8L,
        dispidStopTxMsgBlock        = 9L,
        dispidReSetHW               = 10L,
        dispidReSetSW               = 11L,
        dispidStartLogging          = 12L,
        dispidDisplayWindow         = 13L,
        dispidSetBaudRate           = 14L,
        dispidSetControllerMode     = 15L,
        dispidGetMsgInfo            = 16L,
        dispidGetControllerMode     = 17L,
        dispidSendKeyValue          = 18L,
        dispidLoadConfiguration     = 19L,
        dispidImportDatabase        = 20L,
        dispidEnableDisableHandlers = 21L,
        dispidGetErrorCounter       = 22L,
        dispidSendCANMSg            = 23L,
        dispidGetNetworkStatistics  = 24L,
        dispidGetBaudRate           = 25L,
        dispidStopLogging           = 26L,
        dispidWriteToLogFile        = 27L,
        dispidSaveConfiguration     = 28L,
        dispidAddLoggingBlock       = 29L,
        dispidSaveConfigurationAs   = 30L,
        dispidAddTxBlock            = 31L,
        dispidGetTxBlockCount       = 32L,
        dispidGetTxBlock            = 33L,
        dispidDeleteTxBlock         = 34L,
        dispidClearTxBlockList      = 35L,
        dispidAddMsgToTxBlock       = 36L,
        dispidGetMsgCount           = 37L,
        dispidGetMsgFromTxBlock     = 38L,
        dispidDeleteMsgFromTxBlock  = 39L,
        dispidClearMsgList          = 40L,
        dispidAddFilterScheme       = 41L,
        dispidGetFilterScheme       = 42L,
        dispidGetFilterSchCount     = 43L,
        dispidUpdateFilterSch       = 44L,
        dispidGetFilterCountInSch   = 45L,
        dispidGetFilterInFilterSch  = 46L,
        dispidDeleteFilterInSch     = 47L,
        dispidEnableFilterSch       = 48L,
        dispidAddSimulatedSystem      = 49L,
        dispidGetSimulatedSystemCount = 50L,
        dispidGetSimulatedSystemName  = 51L,
        dispidDeleteSimulatedSystem   = 52L,
        dispidRemoveLoggingBlock      = 53L,
        dispidGetLoggingBlockCount    = 54L,
        dispidClearLoggingBlockList   = 55L,
        dispidGetLoggingBlock         = 56L
    };


    /******************************************************************************
        Function Name    : Connect

        Input(s)         :  BOOL
        Output           :  -
        Functionality    :  Connect/disconnect the tool
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT Connect(BOOL bConnect)
    {
        HRESULT hResult = E_POINTER;
        CFlags* pFlags = GetIFlags();
        CMainFrame* pMainFrm = GetIMainFrame();

        if ((NULL != pFlags) && (NULL != pMainFrm))
        {
            if ((BOOL) pFlags->nGetFlagStatus(CONNECTED) != bConnect)
            {
                pMainFrm->COM_ConnectTool();
                hResult = S_OK;
            }
            else
            {
                hResult = E_FAIL;
            }
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  ResetHW

        Input(s)         :
        Output           :  -
        Functionality    :  reset hardware
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT ResetHW()
    {
        HRESULT hResult = E_POINTER;
        CMainFrame* pMainFrm = GetIMainFrame();

        if (NULL != pMainFrm)
        {
            pMainFrm->OnFunctionsResetHardware();
            hResult = S_OK;
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  GetErrorCounter

        Input(s)         :
        Output           :  -
        Functionality    :  Provide's the error count
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT GetErrorCounter(UCHAR* Tx, UCHAR* Rx, INT nChannel)
    {
        HRESULT hResult = E_POINTER;
        CBaseDIL_CAN* pDIL_CAN = GetICANDIL();

        if (NULL != pDIL_CAN)
        {
            SERROR_CNT sErrorCount;
            hResult = pDIL_CAN->DILC_GetErrorCount(sErrorCount, nChannel, ERR_CNT);

            if (S_OK == hResult)
            {
                *Tx = sErrorCount.m_ucTxErrCount;
                *Rx = sErrorCount.m_ucRxErrCount;
            }
            else
            {
                hResult = E_FAIL;
            }
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  SendCANMSg

        Input(s)         :  message details
        Output           :  -
        Functionality    :  sends the message
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT SendCANMSg(CAN_MSGS* psMsg)
    {
        HRESULT hResult = E_POINTER;
        CBaseDIL_CAN* pDIL_CAN = GetICANDIL();

        if ((NULL != pDIL_CAN) && (NULL != psMsg))
        {
            STCAN_MSG sTempMsg = {'\0'};
            sTempMsg.m_ucChannel  = psMsg->m_ucChannel;
            sTempMsg.m_ucDataLen  = psMsg->m_ucDataLen;
            sTempMsg.m_ucEXTENDED = psMsg->m_bEXTENDED;
            sTempMsg.m_ucRTR      = psMsg->m_bRTR;
            sTempMsg.m_unMsgID    = psMsg->m_unMsgID;

            // Copy the data bytes
            for (int i = 0; i < sTempMsg.m_ucDataLen; i++)
            {
                sTempMsg.m_ucData[i] = psMsg->m_ucData[i];
            }

            hResult = pDIL_CAN->DILC_SendMsg(0, sTempMsg);
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  GetMsgInfo

        Input(s)         :  message name
        Output           :  message structure
        Functionality    :  Provides the Message structure
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT GetMsgInfo(BSTR MsgName, sMESSAGESTRUCT* psMsgStruct)
    {
        HRESULT hResult = E_POINTER;
        CMsgSignal* pMsgSignal = GetIMsgDB();

        if (NULL != pMsgSignal)
        {
            char acStr[256] = {'\0'};
            BSTR_2_PCHAR(MsgName, acStr, 256);
            CString omMsgName(acStr);
            sMESSAGE* psMessage = pMsgSignal->psGetMessagePointer(omMsgName);

            if (NULL != psMessage)
            {
                psMsgStruct->m_dMessageCode = psMessage->m_unMessageCode;
                psMsgStruct->m_dNumberOfSignals = psMessage->m_unNumberOfSignals;
                psMsgStruct->m_dMessageLength = psMessage->m_unMessageLength;
                psMsgStruct->m_bMessageFrameFormat = psMessage->m_bMessageFrameFormat;
                psMsgStruct->m_nMsgDataFormat = psMessage->m_nMsgDataFormat;
                hResult = S_OK;
            }
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  GetNetworkStatistics

        Input(s)         :  Channel no., statistic structure
        Output           :  Statistics of the channel
        Functionality    :  Provides all the statistics
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT GetNetworkStatistics(int nChannel, sBUSSTATISTICS_USR* psStat)
    {
        HRESULT hResult = E_POINTER;
        CBaseBusStatisticCAN* pBusStatCAN = GetICANBusStat();

        if ((NULL != pBusStatCAN) && (NULL != psStat))
        {
            SBUSSTATISTICS sBusStat = {'\0'};
            hResult = pBusStatCAN->BSC_GetBusStatistics((UINT) nChannel, sBusStat);
            memcpy(psStat, &sBusStat, sizeof(SBUSSTATISTICS));
        }

        return hResult;
    }

    // NODE SIMULATION MODULE RELATED FUNCTIONS: START

    /******************************************************************************
        Function Name    :  LoadAllDll

        Input(s)         :
        Output           :  -
        Functionality    :  Post the message to CMainframe LoadAllDll
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT LoadAllDll()
    {
        HRESULT hResult = E_POINTER;
        CMainFrame* pMainFrm = GetIMainFrame();

        if (NULL != pMainFrm)
        {
            CStringArray omStrBuildFiles;

            if (pMainFrm->COM_bDllLoad(&omStrBuildFiles))
            {
                hResult = S_OK;
            }
            else
            {
                hResult = E_FAIL;
            }
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  UnLoadAllDll

        Input(s)         :
        Output           :  -
        Functionality    :  Post the message to CMainframe UnLoadAllDll
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT UnLoadAllDll()
    {
        HRESULT hResult = E_POINTER;
        CMainFrame* pMainFrm = GetIMainFrame();

        if (NULL != pMainFrm)
        {
            CStringArray omStrBuildFiles;

            if (pMainFrm->COM_bDllUnload(&omStrBuildFiles))
            {
                hResult = S_OK;
            }
            else
            {
                hResult = E_FAIL;
            }
        }

        return hResult;
    }
    /******************************************************************************
        Function Name    :  SendKeyValue

        Input(s)         :  Key's Ascii value
        Output           :  -
        Functionality    :  It post the key message to main frame
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT SendKeyValue(UCHAR KeyVal)
    {
        HRESULT hResult = E_POINTER;
        CMainFrame* pMainFrm = GetIMainFrame();

        if (NULL != pMainFrm)
        {
            pMainFrm->vKeyPressedInMsgWnd((WPARAM) KeyVal, (LPARAM) 0);
            hResult = S_OK;
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  EnableDisableHandlers

        Input(s)         :  handler type, bool enable/disable
        Output           :  -
        Functionality    :
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT EnableDisableHandlers(BOOL bEnable, eHandlerType eType)
    {
        HRESULT hResult = E_POINTER;
        CFlags* pFlags = GetIFlags();
        CMainFrame* pMainFrm = GetIMainFrame();

        if ((NULL != pFlags) && (NULL != pMainFrm))
        {
            if (pFlags->nGetFlagStatus(CONNECTED))
            {
                switch (eType)
                {
                    case all:
                    {
                        if (pFlags->nGetFlagStatus(ALL_HANDLER) != bEnable)
                        {
                            pMainFrm->COM_EnableAllHandlers(bEnable);
                        }

                        hResult = S_OK;
                    }
                    break;

                    case timers:
                    {
                        // If timer button is enabled then only call the function
                        if (pFlags->nGetFlagStatus(TIMERBUTTON) != bEnable)
                        {
                        }

                        hResult = S_OK;
                    }
                    break;

                    case key:
                    {
                        if (pFlags->nGetFlagStatus(KEY_HANDLER_ON) != bEnable)
                        {
                        }

                        hResult = S_OK;
                    }
                    break;

                    case message :
                    {
                        if (pFlags->nGetFlagStatus(MSGHANDLERBUTTON) != bEnable)
                        {
                        }

                        hResult = S_OK;
                    }
                    break;

                    case error :
                    {
                        if (pFlags->nGetFlagStatus(ERROR_HANDLER) != bEnable)
                        {
                        }

                        hResult = S_OK;
                    }
                    break;

                    default:
                    {
                        hResult = E_INVALIDARG;
                    }
                }
            }
        }

        return hResult;
    }
    // NODE SIMULATION MODULE RELATED FUNCTIONS: END

    // CONFIGURATION RELATED FUNCTIONS: START

    /******************************************************************************
        Function Name    :  LoadConfiguration

        Input(s)         :
        Output           :  -
        Functionality    :  Calls the mainframe class function to load the
                            configuration
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT LoadConfiguration(BSTR FileName)
    {
        HRESULT hResult = E_POINTER;
        CFlags* pFlags = GetIFlags();
        CMainFrame* pMainFrm = GetIMainFrame();

        if ((NULL != pFlags) && (NULL != pMainFrm))
        {
            if ((BOOL) pFlags->nGetFlagStatus(CONNECTED) == FALSE) // Loading of
            {
                // configuration is done iff the present node is disconnected.
                char acStr[_MAX_PATH] = {'\0'};
                BSTR_2_PCHAR(FileName, acStr, _MAX_PATH);
                CString omFilePath(acStr);

                if (pMainFrm->nLoadConfigFile(omFilePath) == 1)
                {
                    hResult = S_OK;
                }
                else
                {
                    hResult = E_FAIL;
                }
            }
            else
            {
                hResult = E_FAIL;
            }
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  ImportDatabase

        Input(s)         :  File path
        Output           :  -
        Functionality    :  import the database file
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT ImportDatabase(BSTR DBFilePath)
    {
        HRESULT hResult = E_POINTER;
        CMainFrame* pMainFrm = GetIMainFrame();

        if (NULL != pMainFrm)
        {
            char acStr[_MAX_PATH] = {'\0'};
            BSTR_2_PCHAR(DBFilePath, acStr, _MAX_PATH);
            CString omFilePath(acStr);
            hResult = pMainFrm->dLoadDataBaseFile(omFilePath, TRUE);
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  SaveConfiguration

        Input(s)         :  -
        Output           :  -
        Functionality    :  Save the Configurations in file
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  25.09.2006
        Modifications    :
    ******************************************************************************/
    HRESULT SaveConfiguration()
    {
        HRESULT hResult = E_POINTER;
        CMainFrame* pMainFrm = GetIMainFrame();

        if (NULL != pMainFrm)
        {
            if (pMainFrm->COM_SaveConfigFile())
            {
                hResult = S_OK;
            }
            else
            {
                hResult = E_FAIL;
            }
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  SaveConfigurationAs

        Input(s)         :  Config File path
        Output           :
        Functionality    :  Save the configuration to new file
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT SaveConfigurationAs(BSTR ConfigPath)
    {
        char acStr[_MAX_PATH] = {'\0'};
        BSTR_2_PCHAR(ConfigPath, acStr, _MAX_PATH);
        CString omConfigPath(acStr);
        HRESULT hResult = E_FAIL;

        if (theApp.COM_nSaveConfiguration(omConfigPath) == defCONFIG_FILE_SUCCESS)
        {
            hResult = S_OK;
        }

        return hResult;
    }
    // CONFIGURATION RELATED FUNCTIONS: END

    // TX BLOCK RELATED FUNCTIONS: START

    /******************************************************************************
        Function Name    :  StartTxMsgBlock

        Input(s)         :
        Output           :  -
        Functionality    :  Starts sending messages if tool is connected
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT StartTxMsgBlock()
    {
        HRESULT hResult = E_POINTER;
        CMainFrame* pMainFrm = GetIMainFrame();

        if (NULL != pMainFrm)
        {
            pMainFrm->COM_SendMessage();
            hResult = S_OK;
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  StopTxMsgBlock

        Input(s)         :
        Output           :  -
        Functionality    :  Stops sending messages
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT StopTxMsgBlock()
    {
        return StartTxMsgBlock();
    }

    /******************************************************************************
        Function Name    :  GetTxBlockCount

        Input(s)         :  -
        Output           :  No. of Tx blocks available
        Functionality    :  Get Tx block count
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT GetTxBlockCount (USHORT* Result)
    {
        *Result = (USHORT) TX_unGetTxBlockCount();
        return S_OK;
    }

    /******************************************************************************
        Function Name    :  DeleteTxBlock

        Input(s)         :  -
        Output           :
        Functionality    :  Delete Tx Block
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT DeleteTxBlock (USHORT /*BlockIndex*/)
    {
        return TX_vDeleteTxBlockMemory();
    }

    /******************************************************************************
        Function Name    :  ClearTxBlockList

        Input(s)         :  -
        Output           :
        Functionality    :  Clear Tx Block List
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT ClearTxBlockList(void)
    {
        return TX_vDeleteTxBlockMemory();
    }
    // TX BLOCK RELATED FUNCTIONS: END


    // FILTERING RELATED FUNCTIONS: START

    /******************************************************************************
        Function Name    :  EnableFilterSch

        Input(s)         :  -
        Output           :
        Functionality    :  Enables / disables a filter scheme
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT EnableFilterSch(EFILTERMODULE eModule, BOOL bEnable)
    {
        HRESULT hResult;

        switch (eModule)
        {
            case Log:
            {
                CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();

                if (NULL != pCANLogger)
                {
                    USHORT LogBlocks = pCANLogger->FPC_GetLoggingBlockCount();
                    hResult = S_OK;

                    for (USHORT i = 0; (i < LogBlocks) && (S_OK == hResult); i++)
                    {
                        hResult = pCANLogger->FPC_EnableFilter(i, bEnable);
                    }
                }
                else
                {
                    hResult = E_POINTER;
                }
            }
            break;

            case Display:
            {
                //hResult = PSDI_EnableFilterApplied(bEnable);
                CMsgContainerBase* pouMsgContainerIntrf = NULL;
                PSDI_GetInterface(CAN, (void**)&pouMsgContainerIntrf);

                if(pouMsgContainerIntrf)
                {
                    pouMsgContainerIntrf->EnableFilterApplied(bEnable);
                }
            }
            break;

            default:
            {
                hResult = E_INVALIDARG;
                ASSERT(FALSE);
            }
            break;
        }

        return hResult;
    }
    // FILTERING RELATED FUNCTIONS: END

    // LOG RELATED FUNCTIONS: START

    /******************************************************************************
        Function Name    :  StartLogging

        Input(s)         :
        Output           :  -
        Functionality    :  Start logging if it is stopped
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT StartLogging()
    {
        HRESULT hResult = E_POINTER;
        CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();

        if (NULL != pCANLogger)
        {
            hResult = pCANLogger->FPC_EnableLogging(TRUE);
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  StopLogging

        Input(s)         :
        Output           :  -
        Functionality    :  disable logging of messages
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT StopLogging()
    {
        HRESULT hResult = E_POINTER;
        CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();

        if (NULL != pCANLogger)
        {
            hResult = pCANLogger->FPC_EnableLogging(FALSE);
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  WriteToLogFile

        Input(s)         :  -
        Output           :
        Functionality    :  Writes a string to the log file specified
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT WriteToLogFile(USHORT /*BlockIndex*/, BSTR bstrStr)
    {
        HRESULT hResult = E_POINTER;
        CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();

        if (NULL != pCANLogger)
        {
            char acStr[256] = {'\0'};
            BSTR_2_PCHAR(bstrStr, acStr, 256);
            CString omStr(acStr);
            hResult = pCANLogger->FPC_LogString(omStr);
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  GetLoggingBlockCount

        Input(s)         :  -
        Output           :
        Functionality    :  Returns total number of logging blocks defined
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT GetLoggingBlockCount(USHORT* BlockCount)
    {
        HRESULT hResult = E_POINTER;
        CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();

        if (NULL != pCANLogger)
        {
            *BlockCount = pCANLogger->FPC_GetLoggingBlockCount();
            hResult = S_OK;
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  RemoveLoggingBlock

        Input(s)         :  -
        Output           :
        Functionality    :  Deletes a logging block
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT RemoveLoggingBlock(USHORT BlockIndex)
    {
        HRESULT hResult = E_POINTER;
        CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();

        if (NULL != pCANLogger)
        {
            hResult = pCANLogger->FPC_RemoveLoggingBlock(BlockIndex);
        }

        return hResult;
    }

    /******************************************************************************
        Function Name    :  ClearLoggingBlockList

        Input(s)         :  -
        Output           :
        Functionality    :  Clears the logging block list
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT ClearLoggingBlockList(void)
    {
        HRESULT hResult = E_POINTER;
        CBaseFrameProcessor_CAN* pCANLogger = GetICANLogger();

        if (NULL != pCANLogger)
        {
            hResult = pCANLogger->FPC_ClearLoggingBlockList();
        }

        return hResult;
    }
    // LOG RELATED FUNCTIONS: END

    /******************************************************************************
        Function Name    :  ResetSW

        Input(s)         :
        Output           :  -
        Functionality    :  REset software
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT ResetSW()
    {
        return E_NOTIMPL;
    }
    /******************************************************************************
        Function Name    :  DisplayWindow

        Input(s)         :
        Output           :  -
        Functionality    :  Display the windows in BUSMASTER
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  20.06.2006
        Modifications    :
    ******************************************************************************/
    HRESULT DisplayWindow(eWindow /*eWhichWindow*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
        Function Name    :  AddLoggingBlock

        Input(s)         :  Log file name,time mode,numeric mode,file mode for logging
        Output           :
        Functionality    :  Add a log file
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  06.10.2006
        Modifications    :
    ******************************************************************************/
    HRESULT AddLoggingBlock(SLOGGINGBLOCK_USR* /*psLoggingBlock*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
        Function Name    :  AddTxBlock

        Input(s)         :  Tx block details
        Output           :
        Functionality    :  Add transmission block
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT AddTxBlock(STXBLOCK_USR* /*psTxBlock*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
        Function Name    :  GetTxBlock

        Input(s)         :  -
        Output           :
        Functionality    :  Get Tx block details
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT GetTxBlock (USHORT /*BlockIndex*/, STXBLOCK_USR* /*psTxBlock*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
        Function Name    :  AddMsgToTxBlock

        Input(s)         :  -
        Output           :
        Functionality    :  Add Msg To TxBlock
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT AddMsgToTxBlock(USHORT /*BlockIndex*/, CAN_MSGS* /*psMsg*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
        Function Name    :  GetMsgCount

        Input(s)         :  -
        Output           :
        Functionality    :  Get Msg Count of a TxBlock
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT GetMsgCount(USHORT /*BlockIndex*/, USHORT* /*Result*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
        Function Name    :  GetMsgFromTxBlock

        Input(s)         :  -
        Output           :
        Functionality    :  Get Msg info From TxBlock
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT GetMsgFromTxBlock(USHORT /*BlockIndex*/, USHORT /*MsgIndex*/, CAN_MSGS* /*psMsg*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
        Function Name    :  DeleteMsgFromTxBlock

        Input(s)         :  -
        Output           :
        Functionality    :  Delete MsgFrom TxBlock
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT DeleteMsgFromTxBlock(USHORT /*BlockIndex*/, USHORT /*FrameIndex*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
        Function Name    :  ClearMsgList

        Input(s)         :  -
        Output           :
        Functionality    :  Clears the Msg list in a Tx block given its index
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT ClearMsgList(USHORT /*BlockIndex*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
        Function Name    :  AddFilterScheme

        Input(s)         :  -
        Output           :
        Functionality    :  Defines a filter scheme which is identified by its name.
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT AddFilterScheme (BSTR /*pcFilterName*/, VARIANT_BOOL /*FilterType*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
        Function Name    :  GetFilterScheme

        Input(s)         :  -
        Output           :
        Functionality    :  Given filterscheme index provide filter scheme name and type
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT GetFilterScheme(USHORT /*FilterSchINdex*/, BSTR /*pcFilterName*/,
                            VARIANT_BOOL* /*FilterType*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
        Function Name    :  GetFilterSchCount

        Input(s)         :  -
        Output           :
        Functionality    :  Get Filter Scheme Count
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT GetFilterSchCount(USHORT* /*pTotal*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
        Function Name    :  UpdateFilterSch

        Input(s)         :  -
        Output           :
        Functionality    :  Adds / sets a filter in a filter scheme
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT UpdateFilterSch(USHORT /*FilterSchIndex*/, SFILTER_USR* /*psFilter*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
        Function Name    :  GetFilterCountInSch

        Input(s)         :  -
        Output           :
        Functionality    :  Get Filter Count In a filter Scheme
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT GetFilterCountInSch(USHORT /*FilterSchIndex*/, USHORT* /*pTotal*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
        Function Name    :  GetFilterInFilterSch

        Input(s)         :  -
        Output           :
        Functionality    :  Getter for a filter, given index of the filter scheme
                            and index of the filter in the filter scheme
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT GetFilterInFilterSch(USHORT /*FilterSchIndex*/, USHORT /*FilterIndex*/,
                                 SFILTER_USR* /*psFilter*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
        Function Name    :  DeleteFilterInSch

        Input(s)         :  -
        Output           :
        Functionality    :  Deletes a filter from a filter scheme
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT DeleteFilterInSch(USHORT /*FilterSchIndex*/, USHORT /*FilterIndex*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
        Function Name    :  GetLoggingBlock

        Input(s)         :  -
        Output           :
        Functionality    :  Renders a logging block based on its index in the list
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT GetLoggingBlock(USHORT /*BlockIndex*/, void* /*psLoggingBlock*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
        Function Name    :  AddSimulatedSystem

        Input(s)         :  -
        Output           :
        Functionality    :  Add Simulated System in the configuration
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT AddSimulatedSystem(BSTR /*pcSimPath*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
        Function Name    :  GetSimulatedSystemCount

        Input(s)         :  -
        Output           :
        Functionality    :  Get Simulated System Count
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT GetSimulatedSystemCount(USHORT* /*pSimTotal*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
        Function Name    :  GetSimulatedSystemName

        Input(s)         :  -
        Output           :
        Functionality    :  Get the Simulated System Name
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT GetSimulatedSystemName(USHORT /*SimSysIndex*/, BSTR* /*pcSimPath*/)
    {
        return E_NOTIMPL;
    }

    /******************************************************************************
        Function Name    :  DeleteSimulatedSystem

        Input(s)         :  -
        Output           :
        Functionality    :  Delete the given Simulated System
        Member of        :  CApplication
        Author(s)        :  Anish kumar
        Date Created     :  01.06.2010
        Modifications    :
    ******************************************************************************/
    HRESULT DeleteSimulatedSystem(USHORT /*SimSysIndex*/)
    {
        return E_NOTIMPL;
    }


    ////////////////////////////////////////////////////////////////////////////////

    BEGIN_INTERFACE_PART(LocalClass, IApplication)
    STDMETHOD(GetTypeInfoCount)(UINT FAR* pctinfo);
    STDMETHOD(GetTypeInfo)(
        UINT itinfo,
        LCID lcid,
        ITypeInfo FAR* FAR* pptinfo);
    STDMETHOD(GetIDsOfNames)(
        REFIID riid,
        OLECHAR FAR* FAR* rgszNames,
        UINT cNames,
        LCID lcid,
        DISPID FAR* rgdispid);
    STDMETHOD(Invoke)(
        DISPID dispidMember,
        REFIID riid,
        LCID lcid,
        WORD wFlags,
        DISPPARAMS FAR* pdispparams,
        VARIANT FAR* pvarResult,
        EXCEPINFO FAR* pexcepinfo,
        UINT FAR* puArgErr);

    STDMETHOD(GetMsgInfo)(BSTR MsgName,sMESSAGESTRUCT*  sMsgStruct);
    STDMETHOD(GetNetworkStatistics)(int nChannel, sBUSSTATISTICS_USR* sStat);
    STDMETHOD(SendCANMSg)(CAN_MSGS* sMsg);
    STDMETHOD(GetErrorCounter)(UCHAR* Tx, UCHAR* Rx, INT nChannel);
    STDMETHOD(EnableDisableHandlers)(BOOL bEnable,eHandlerType eType);
    STDMETHOD(ImportDatabase)(BSTR DBFilePath);
    STDMETHOD(LoadConfiguration)(BSTR FileName);
    STDMETHOD(SendKeyValue)(UCHAR keyval);
    //STDMETHOD(GetControllerMode)(eControllerMode *eMode);
    //STDMETHOD(SetControllerMode)(eControllerMode eMode);
    //STDMETHOD(GetBaudRate)(int nChannelNo, BYTE *bBTR0,BYTE *bBTR1);
    //STDMETHOD(SetBaudRate)(int nChannelNo, BYTE bBTR0,BYTE bBTR1);
    STDMETHOD(DisplayWindow)(eWindow eWhichWindow);
    STDMETHOD(WriteToLogFile)(USHORT BlockIndex, BSTR pcStr);
    STDMETHOD(StopLogging)();
    STDMETHOD(StartLogging)();
    STDMETHOD(ResetSW)();
    STDMETHOD(ResetHW)();
    STDMETHOD(StopTxMsgBlock)();
    STDMETHOD(StartTxMsgBlock)();
    STDMETHOD(UnLoadAllDll)();
    STDMETHOD(LoadAllDll)();
    //STDMETHOD(BuildLoadAllDll)();
    //STDMETHOD(BuildAllDll)();
    //STDMETHOD(GetHwFilter)(BOOL *pnExtended, double *dBeginMsgId, double *dEndMsgId);
    //STDMETHOD(SetHwFilter)(BOOL pnExtended, double dBeginMsgId, double dEndMsgId);
    STDMETHOD(Connect)(BOOL bConnect);
    STDMETHOD(SaveConfiguration)();
    STDMETHOD(AddLoggingBlock)(SLOGGINGBLOCK_USR* psLoggingBlock);
    STDMETHOD(SaveConfigurationAs) (BSTR ConfigPath);
    STDMETHOD(AddTxBlock) (STXBLOCK_USR* psTxBlock);
    STDMETHOD(GetTxBlockCount) (USHORT* Result);
    STDMETHOD(GetTxBlock) (USHORT BlockIndex, STXBLOCK_USR* psTxBlock);
    STDMETHOD(DeleteTxBlock) (USHORT BlockIndex);
    STDMETHOD(ClearTxBlockList) ();
    STDMETHOD(AddMsgToTxBlock) (USHORT BlockIndex, CAN_MSGS* psMsg);
    STDMETHOD(GetMsgCount) (USHORT BlockIndex, USHORT* Result);
    STDMETHOD(GetMsgFromTxBlock)(USHORT BlockIndex, USHORT MsgIndex, CAN_MSGS* psMsg);
    STDMETHOD(DeleteMsgFromTxBlock) (USHORT BlockIndex, USHORT MsgIndex);
    STDMETHOD(ClearMsgList) (USHORT BlockIndex);
    STDMETHOD(AddFilterScheme) (BSTR pcFilterName, VARIANT_BOOL  FilterType);
    STDMETHOD(GetFilterScheme) (USHORT FilterSchINdex, BSTR pcFilterName, VARIANT_BOOL*  FilterType);
    STDMETHOD(GetFilterSchCount) (USHORT* pTotal);
    STDMETHOD(UpdateFilterSch) (USHORT FilterSchIndex, SFILTER_USR* psFilter);
    STDMETHOD(GetFilterCountInSch) (USHORT FilterSchIndex, USHORT* pTotal);
    STDMETHOD(GetFilterInFilterSch) (USHORT FilterSchIndex, USHORT FilterIndex, SFILTER_USR* psFilter);
    STDMETHOD(DeleteFilterInSch) (USHORT FilterSchIndex, USHORT FilterIndex);
    STDMETHOD(EnableFilterSch) (EFILTERMODULE eModule, BOOL Enable);
    STDMETHOD(AddSimulatedSystem)(BSTR pcSimPath);
    STDMETHOD(GetSimulatedSystemCount)(USHORT* pSimTotal);
    STDMETHOD(GetSimulatedSystemName)(USHORT SimSysIndex, BSTR* pcSimPath);
    STDMETHOD(DeleteSimulatedSystem)(USHORT SimSysIndex);
    STDMETHOD(RemoveLoggingBlock)(USHORT BlockIndex);
    STDMETHOD(GetLoggingBlockCount)(USHORT* BlockCount);
    STDMETHOD(ClearLoggingBlockList)(void);
    STDMETHOD(GetLoggingBlock)(USHORT BlockIndex, SLOGGINGBLOCK_USR* psLoggingBlock);
    END_INTERFACE_PART(LocalClass)
};
